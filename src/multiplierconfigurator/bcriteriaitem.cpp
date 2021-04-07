#include "bcriteriaitem.h"
#include "bmath.h"

#include <QPen>
#include <QDebug>
#include <QtMath>
#include <QGraphicsSceneMouseEvent>
#include <QFont>
#include <QTextBlockFormat>
#include <QTextCursor>
int BCriteriaItem::mIndexCounter = 1;
BCriteriaItem::BCriteriaItem(QString pName, QObject *parent) :
    QObject(parent)
  , mIsLocked(false)
  , mIsPressed(false)
  , mIsHovered(false)
  , mAllowedToMove(true)
  , mAngle(0.1)
  , mName(pName)
  , mMultiplier(0.0)
{
    mIndex = mIndexCounter++;

    mSceneItem = new BCriteriaSceneItem(this);
    mLabel = new BCriteriaLabelItem(this);

    updateColor();
}

bool BCriteriaItem::isLocked() const
{
    return mIsLocked;
}

double BCriteriaItem::angle() const
{
    return mAngle;
}

double BCriteriaItem::multiplier() const
{
    return mMultiplier;
}

QGraphicsItemGroup *BCriteriaItem::sceneItem()
{
    return mSceneItem;
}

QGraphicsItemGroup *BCriteriaItem::labelItem()
{
    return mLabel;
}

QString BCriteriaItem::name() const
{
    return mName;
}

int BCriteriaItem::index() const
{
    return mIndex;
}

QPointF BCriteriaItem::scenePos() const
{
    return mSceneItem->scenePos();
}

void BCriteriaItem::setIsLocked(bool pIsLocked)
{
    if (mIsLocked == pIsLocked)
        return;

    mIsLocked = pIsLocked;
    updateColor();

    emit isLockedChanged(mIsLocked);
}

void BCriteriaItem::setAllowedToMove(bool pAllowedToMove)
{
    mAllowedToMove = pAllowedToMove;
    updateColor();
}

void BCriteriaItem::updateColor()
{
    QColor tColor;

    if(mIsLocked)
        tColor = Qt::red;
    else if(!mAllowedToMove)
        tColor = Qt::yellow;
    else if(mIsHovered)
        tColor = Qt::green;
    else if(mIsPressed)
        tColor = Qt::blue;
    else
        tColor = Qt::white;
    mSceneItem->updateColor(tColor);
}

void BCriteriaItem::setMultiplier(double pMultiplier)
{
    if (qFuzzyCompare(mMultiplier, pMultiplier))
        return;

    mMultiplier = pMultiplier;
    updatePosByMultiplier();
    emit multiplierChanged(mMultiplier);
}

void BCriteriaItem::setName(QString pName)
{
    if (mName == pName)
        return;

    mName = pName;
    emit nameChanged(mName);
}

void BCriteriaItem::updatePosByMultiplier()
{
    if(mMultiplier < 0)
    {
        mMultiplier = 0.0;
        updatePosByMultiplier();
        return;
    }

    double tMax = BConstants::instance()->totalMultiplier();
    if(!BConstants::instance()->fixedMultiplierTotal())
        tMax = BConstants::instance()->totalMultiplier() / BConstants::instance()->criteriaCount();

    if(mMultiplier > tMax)
    {
        mMultiplier = tMax;
        updatePosByMultiplier();
        return;
    }

    QPointF tNewPos = BMath::getPointAtAngleMultiplier(mAngle, mMultiplier);
    mSceneItem->setPos(tNewPos);
}

void BCriteriaItem::setAngle(double pAngle)
{
    if(qFuzzyCompare(mAngle, pAngle))
        return;
    mAngle = pAngle;
    emit angleChanged(pAngle);
    updatePosByMultiplier();
}

void BCriteriaSceneItem::hoverEnterEvent(QGraphicsSceneHoverEvent *pEvent) {
    if(mCriteriaItem->mIsLocked)
        return;
    mCriteriaItem->mIsHovered = true;
    mCriteriaItem->updateColor();
    QGraphicsItemGroup::hoverEnterEvent(pEvent);
}

void BCriteriaSceneItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *pEvent) {
    if(mCriteriaItem->mIsLocked)
        return;
    mCriteriaItem->mIsHovered = false;
    mCriteriaItem->updateColor();
    QGraphicsItemGroup::hoverLeaveEvent(pEvent);
}

void BCriteriaSceneItem::mousePressEvent(QGraphicsSceneMouseEvent *pEvent) {
    if(pEvent->button() == Qt::RightButton)
        mCriteriaItem->setIsLocked(!mCriteriaItem->mIsLocked);
    if(mCriteriaItem->mIsLocked)
    {
        qDebug() << "its locked";
        return;
    }
    if(!mCriteriaItem->mAllowedToMove)
    {
        qDebug() << "no move";
        return;
    }
    mCriteriaItem->mIsPressed = true;
    mCriteriaItem->updateColor();
}

void BCriteriaSceneItem::mouseMoveEvent(QGraphicsSceneMouseEvent *pEvent) {
    if(!mCriteriaItem->mIsPressed)
        return;

    QPointF tPos = BMath::scenePos(pEvent->scenePos());

    double tDirectionAngle = 90.0 - qRadiansToDegrees(qAtan2(tPos.y(), tPos.x())); // 90.0- because 0 is north not east
    double tDiffAngle = mCriteriaItem->mAngle - tDirectionAngle;
    double tLength = qSqrt(qPow(tPos.x(), 2) + qPow(tPos.y(), 2));
    double tActualLength = tLength * qCos(qDegreesToRadians(tDiffAngle));
    if(tActualLength < 0)
        return;
    QPointF tNewPos = BMath::getPointAtAngle(mCriteriaItem->mAngle, tActualLength);
    double tMultiplier = BMath::getMultiplierAtPoint(tNewPos);
    mCriteriaItem->setMultiplier(tMultiplier);

    emit mCriteriaItem->positionChanged();
}

void BCriteriaSceneItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *) {
    mCriteriaItem->mIsPressed = false;
    mCriteriaItem->updateColor();
}

BCriteriaLabelItem::BCriteriaLabelItem(BCriteriaItem *pCriteriaItem)  :
    QGraphicsItemGroup()
  , mCriteriaItem(pCriteriaItem)
{
    QPen tPen(Qt::white);
    tPen.setCosmetic(true);
    QBrush tBrush(Qt::lightGray);

    QString tIndexAndName = QString("%2").arg(mCriteriaItem->name());

    double tWidth = BConstants::instance()->maxDistance()/3;
    double tHeight = BConstants::instance()->maxDistance()/6;
    double tMinusX = -tWidth / 2.0;
    double tMinusY = -tHeight / 2.0;

    mRectangle = new QGraphicsRectItem(this);
    mRectangle->setRect(tMinusX, tMinusY, tWidth, tHeight);

    mNameLabel = new QGraphicsTextItem(this);
    mNameLabel->setFont(QFont("Arial",50));
    mNameLabel->setPlainText(tIndexAndName);
    mNameLabel->setDefaultTextColor(Qt::white);
    mNameLabel->setPos(tMinusX, tMinusY);

    mMultiplierLabel = new QGraphicsTextItem(this);
    mMultiplierLabel->setFont(QFont("Arial",50));
    mMultiplierLabel->setPlainText(QString("%1").arg(mCriteriaItem->multiplier(), 0, 'f', 2));
    mMultiplierLabel->setDefaultTextColor(Qt::white);
    mMultiplierLabel->setPos(tMinusX, 0);
    connect(mCriteriaItem, &BCriteriaItem::multiplierChanged, this, [=](double pMultiplier){
        mMultiplierLabel->setPlainText(QString("%1").arg(pMultiplier, 0, 'f', 2));
    });
    connect(mCriteriaItem, &BCriteriaItem::angleChanged, this, [=](){
        updatePosition();
    });
}

void BCriteriaLabelItem::updatePosition()
{
    QPointF tCenter = boundingRect().center();
    double tLength = qSqrt(qPow(tCenter.x(), 2) + qPow(tCenter.y(), 2));
    double tMargin = BConstants::instance()->maxDistance() / 5.0;
    tLength += tMargin + BConstants::instance()->maxDistance();

    QPointF tCenterShouldBeAt = BMath::getPointAtAngle(mCriteriaItem->angle(),  tLength);
    setPos(tCenterShouldBeAt);
}

BCriteriaSceneItem::BCriteriaSceneItem(BCriteriaItem *pCriteriaItem)  :
    QGraphicsItemGroup()
  , mCriteriaItem(pCriteriaItem)
{
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton|Qt::RightButton);

    double tRadius = BConstants::instance()->maxDistance() / 15.0;
    double tRadius2 = tRadius / 2.0;

    mEllipse = new QGraphicsEllipseItem;//(this);
    mEllipse->setRect(-tRadius2, -tRadius2, tRadius, tRadius);
    addToGroup(mEllipse);
}

void BCriteriaSceneItem::updateColor(QColor pColor)
{
    QPen tPen(pColor);
    tPen.setCosmetic(true);
    mEllipse->setPen(tPen);
    QBrush tBrush(pColor);
    mEllipse->setBrush(tBrush);
    update();
}
