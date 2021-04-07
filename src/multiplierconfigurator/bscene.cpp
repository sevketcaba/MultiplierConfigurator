#include "bcriteriaitem.h"
#include "bcriteriaitemmanager.h"
#include "bscene.h"

#include <QGraphicsRectItem>

#include <QDebug>
#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>

#include "bmath.h"


BScene::BScene(QObject *parent) :
    QGraphicsScene(parent)
  , mLineGroup(NULL)
{
    mPenWhite = QPen(Qt::white);
    mPenWhite.setCosmetic(true);
    mPenYellow = QPen(Qt::yellow);
    mPenYellow.setCosmetic(true);
    mPenGray = QPen(Qt::lightGray);
    mPenGray.setCosmetic(true);
    mBrushGray = QBrush(Qt::lightGray);

    QPen tPenTransparent(Qt::transparent);

    mRoot = new QGraphicsItemGroup;
    addItem(mRoot);

    mOuterPath = new QGraphicsPathItem(mRoot);
    mOuterPath->setPos(0,0);
    mOuterPath->setPen(mPenWhite);
    mOuterPath->setAcceptedMouseButtons(Qt::NoButton);
    mOuterPath->setFlag(QGraphicsItem::ItemIsFocusable, false);

    mInnerPath = new QGraphicsPathItem(mRoot);
    mInnerPath->setPos(0,0);
    mInnerPath->setPen(mPenGray);
    mInnerPath->setBrush(mBrushGray);
    mInnerPath->setOpacity(0.4);
    mInnerPath->setAcceptedMouseButtons(Qt::NoButton);
    mInnerPath->setFlag(QGraphicsItem::ItemIsFocusable, false);

    mBackgroundRect = new QGraphicsRectItem(mRoot);
    mBackgroundRect->setPos(0,0);
    mBackgroundRect->setPen(tPenTransparent);
    mBackgroundRect->setAcceptedMouseButtons(Qt::NoButton);
    mBackgroundRect->setFlag(QGraphicsItem::ItemIsFocusable, false);

    connect(BConstants::instance(), &BConstants::fixedMultiplierTotalChanged, this, [=](bool){
        itemPositionChanged(true);
    }, Qt::QueuedConnection);

    connect(BCriteriaItemManager::instance(), &BCriteriaItemManager::itemPositionChanged, this, [=](BCriteriaItem *){
        itemPositionChanged(false);
    }, Qt::QueuedConnection);

    connect(BCriteriaItemManager::instance(), &BCriteriaItemManager::itemAboutToRemove, this, [=](BCriteriaItem *pItem){
        removeItem(pItem->sceneItem());
        removeItem(pItem->labelItem());
        itemPositionChanged(true);
        refresh();
    }, Qt::QueuedConnection);

    connect(BCriteriaItemManager::instance(), &BCriteriaItemManager::itemAdded, this, [=](BCriteriaItem *pItem){
        addItem(pItem->sceneItem());
        addItem(pItem->labelItem());
        refresh();
    }, Qt::QueuedConnection);
}

QGraphicsItem *BScene::backgroundItem()
{
    return mBackgroundRect;
}

void BScene::refresh()
{
     BCriteriaItemManager::instance()->blockSignals(true);
    int tUnlockedItemCount = 0;
    if(mLineGroup)
    {
        mRoot->removeFromGroup(mLineGroup);
        removeItem(mLineGroup);
        mLineGroup = NULL;
    }

    mLineGroup = new QGraphicsItemGroup;
    mRoot->addToGroup(mLineGroup);

    int tItemCount = BCriteriaItemManager::instance()->itemCount();

    double tAngle = 0.0;
    double tAngleStep = 360.0 / tItemCount;

    QPainterPath tOutPath;
    QPainterPath tInPath;

    for(int i=0;i<tItemCount;++i) {
        BCriteriaItem *tItem = BCriteriaItemManager::instance()->itemAt(i);
        QPointF tPointAtMax = BMath::getPointAtAngle(tAngle);
        QPointF tPointAtMult = BMath::getPointAtAngleMultiplier(tAngle, tItem->multiplier());
        if(i == 0)
        {
            tOutPath.moveTo(tPointAtMax);
            tInPath.moveTo(tPointAtMult);
        }
        else
        {
            tOutPath.lineTo(tPointAtMax);
            tInPath.lineTo(tPointAtMult);
        }

        QGraphicsLineItem *tLineItem = new QGraphicsLineItem(mRoot);
        tLineItem->setLine(QLineF(QPointF(0,0), tPointAtMax));
        tLineItem->setPen(mPenWhite);
        mLineGroup->addToGroup(tLineItem);

        tItem->setAngle(tAngle);
        tItem->updatePosByMultiplier();
        if(!tItem->isLocked())
            ++tUnlockedItemCount;

        tAngle += tAngleStep;
    }
    tOutPath.closeSubpath();
    tInPath.closeSubpath();

    mOuterPath->setPath(tOutPath);
    mInnerPath->setPath(tInPath);
    double tXMargin  = BConstants::instance()->maxDistance() / 1.5;
    double tYMargin  = BConstants::instance()->maxDistance() / 3.0;
    QRectF tOuterRect(mOuterPath->boundingRect());
    QRectF tBgRect(tOuterRect.x() - tXMargin,
                   tOuterRect.y() - tYMargin,
                   tOuterRect.width() + (tXMargin * 2.0),
                   tOuterRect.height() + (tYMargin * 2.0));
    mBackgroundRect->setRect(tBgRect);

    bool tMoveAvailable = tUnlockedItemCount > 1;
    for(int i=0;i<BCriteriaItemManager::instance()->itemCount();++i) {
        BCriteriaItem *tItem = BCriteriaItemManager::instance()->itemAt(i);
        tItem->setAllowedToMove(tMoveAvailable);
    }
    BCriteriaItemManager::instance()->blockSignals(false);
}

void BScene::itemPositionChanged(bool pUseLockedItems)
{
    BCriteriaItemManager::instance()->blockSignals(true);
    int tItemCount = BCriteriaItemManager::instance()->itemCount();
    if(BConstants::instance()->fixedMultiplierTotal())
    {

        // remove the diff from unlocked items equally
        double tTotalCurrent = 0.0;
        double tTotalHasToBe = BConstants::instance()->totalMultiplier();
        int tUnlockedItemCount = 0;
        for(int i=0;i<tItemCount;++i) {
            BCriteriaItem *tItem = BCriteriaItemManager::instance()->itemAt(i);
            tTotalCurrent += tItem->multiplier();
            if(!tItem->isLocked())
                ++tUnlockedItemCount;
        }

        double tDiff = tTotalHasToBe - tTotalCurrent;

        double tStep = tDiff / tUnlockedItemCount;
        if(pUseLockedItems)
            tStep = tDiff / tItemCount;

        for(int i=0;i<tItemCount;++i) {
            BCriteriaItem *tItem = BCriteriaItemManager::instance()->itemAt(i);
            if(tItem->isLocked() && !pUseLockedItems)
                continue;
            tItem->setMultiplier(tItem->multiplier() + tStep);
        }
    }

    QPainterPath tPath;
    for(int i=0;i<tItemCount;++i)
    {
        BCriteriaItem *tItem = BCriteriaItemManager::instance()->itemAt(i);
        if(i == 0)
            tPath.moveTo(tItem->scenePos());
        else
            tPath.lineTo(tItem->scenePos());
    }
    mInnerPath->setPath(tPath);
    BCriteriaItemManager::instance()->blockSignals(false);
}

void BScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);
}

void BScene::drawForeground(QPainter *painter, const QRectF &rect)
{
    QGraphicsScene::drawForeground(painter, rect);
}

void BScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseReleaseEvent(event);
}

void BScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    emit cursorPos(BMath::scenePos(event->scenePos()));
    QGraphicsScene::mouseMoveEvent(event);
}

