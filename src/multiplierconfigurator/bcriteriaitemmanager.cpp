#include "bcriteriaitemmanager.h"

BCriteriaItemManager::BCriteriaItemManager(QObject *parent) :
    QObject(parent)
{
    qRegisterMetaType<BCriteriaItem*>();
    connect(BConstants::instance(), &BConstants::fixedMultiplierTotalChanged, this, [=](){
        updateTotalMultiplier();
    }, Qt::DirectConnection);
}

BCriteriaItemManager *BCriteriaItemManager::instance()
{
    static BCriteriaItemManager tInstance;
    return &tInstance;
}

QQmlListProperty<BCriteriaItem> BCriteriaItemManager::itemList()
{
    return QQmlListProperty<BCriteriaItem>(this, mItems);
}

int BCriteriaItemManager::itemCount()
{
    return mItems.size();
}

BCriteriaItem *BCriteriaItemManager::itemAt(int pIndex)
{
    return mItems[pIndex];
}

QList<BCriteriaItem*> BCriteriaItemManager::items()
{
    return mItems;
}

void BCriteriaItemManager::addItem(BCriteriaItem *pItem)
{
    connect(pItem, &BCriteriaItem::positionChanged, this, [=](){
        emit itemPositionChanged(pItem);
    }, Qt::DirectConnection);
    connect(pItem, &BCriteriaItem::isLockedChanged, this, [=](bool pIsLocked){
        updateLockStates();
        emit itemLockChanged(pItem, pIsLocked);
    }, Qt::DirectConnection);
    mItems.push_back(pItem);

    BConstants::instance()->setCriteriaCount(mItems.size());

    emit itemAdded(pItem);
    emit itemsChanged();
}

void BCriteriaItemManager::removeItem(BCriteriaItem *pItem)
{
    emit itemAboutToRemove(pItem);
    disconnect(pItem, 0, 0, 0);
    mItems.removeOne(pItem);
    BConstants::instance()->setCriteriaCount(mItems.size());

    emit itemRemoved(pItem);
    emit itemsChanged();

    pItem->deleteLater();
}

void BCriteriaItemManager::updateTotalMultiplier()
{
    if(BConstants::instance()->fixedMultiplierTotal())
        BConstants::instance()->setTotalMultiplier(1.0);
    else
        BConstants::instance()->setTotalMultiplier((double)mItems.size());
}

void BCriteriaItemManager::updateLockStates()
{
    int tUnlockedItemCount = 0;
    for(BCriteriaItem *tItem : mItems)
        if(!tItem->isLocked())
            ++tUnlockedItemCount;
    bool tAllowToMove = tUnlockedItemCount > 1;
    for(BCriteriaItem *tItem : mItems)
        tItem->setAllowedToMove(tAllowToMove);
}
