#include "bcriteriaitemmanager.h"
#include "bundomanager.h"

#include <QDebug>

BUndoFactory *BUndoFactory::mInstance = nullptr;
BUndoManager *BUndoManager::mInstance = nullptr;

BUndoManager::BUndoManager(BCriteriaItemManager *pItemManager, QObject *parent) :
    QObject(parent)
  , mCriteriaItemManager(pItemManager)
{

}

BUndoManager *BUndoManager::init(BCriteriaItemManager *pItemManager)
{
    if(!mInstance)
        mInstance = new BUndoManager(pItemManager);
    return mInstance;
}

BUndoManager *BUndoManager::instance()
{
    if(!mInstance) {
        qCritical() << "init manager first!";
    }
    return mInstance;
}

BUndoFactory::BUndoFactory(BCriteriaItemManager *pItemManager, QObject *parent) :
    QObject(parent)
  , mCriteriaItemManager(pItemManager)
{

}

BUndoFactory *BUndoFactory::init(BCriteriaItemManager *pItemManager)
{
    if(!mInstance)
        mInstance = new BUndoFactory(pItemManager);
    return mInstance;
}

BUndoFactory *BUndoFactory::instance()
{
    if(!mInstance) {
        qCritical() << "init factory first!";
    }
    return mInstance;
}

UndoAction BUndoFactory::createUndo(UndoAction::UndoActionType pType)
{
    Q_UNUSED(pType)
    UndoAction tAction;
    return tAction;
}
