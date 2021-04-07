#ifndef BUNDOMANAGER_H
#define BUNDOMANAGER_H

#include <QObject>

class BCriteriaItemManager;

struct UndoAction {
    enum UndoActionType {
        CriteriaAdd = 1,
        CriteriaRemove,
        PositionUpdate
    };
    QByteArray mUndoData;
};

class BUndoFactory : public QObject
{
    Q_OBJECT
    BUndoFactory(BCriteriaItemManager *pItemManager, QObject *parent = nullptr);
public:
    static BUndoFactory *init(BCriteriaItemManager *pItemManager);
    static BUndoFactory *instance();
    UndoAction createUndo(UndoAction::UndoActionType pType);

protected:
    BCriteriaItemManager *mCriteriaItemManager;
    static BUndoFactory *mInstance;
    friend class BUndoFactory;
};

class BUndoManager : public QObject
{
    Q_OBJECT
    explicit BUndoManager(BCriteriaItemManager *pItemManager, QObject *parent = nullptr);
public:
    static BUndoManager *init(BCriteriaItemManager *pItemManager);
    static BUndoManager *instance();
signals:
    void doUndo(UndoAction pAction);
protected:
    BCriteriaItemManager *mCriteriaItemManager;
    QList<UndoAction> mActions;
    static BUndoManager *mInstance;
    friend class BUndoManager;
};

#endif // BUNDOMANAGER_H
