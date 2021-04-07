#ifndef BCRITERIAITEMMANAGER_H
#define BCRITERIAITEMMANAGER_H

#include <QObject>
#include <QQmlListProperty>

#include "bcriteriaitem.h"

class BCriteriaItemManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<BCriteriaItem> itemList READ itemList NOTIFY itemsChanged)
    explicit BCriteriaItemManager(QObject *parent = nullptr);
public:
    static BCriteriaItemManager *instance();
    QQmlListProperty<BCriteriaItem> itemList();
    QList<BCriteriaItem*> items();

    int itemCount();

    BCriteriaItem *itemAt(int pIndex);
signals:
    void itemsChanged();
    void itemAdded(BCriteriaItem*);
    void itemAboutToRemove(BCriteriaItem*);
    void itemRemoved(BCriteriaItem*);
    void itemPositionChanged(BCriteriaItem*);
    void itemLockChanged(BCriteriaItem*,bool);
public slots:
    void addItem(BCriteriaItem *pItem);
    void removeItem(BCriteriaItem *pItem);

protected slots:
    void updateTotalMultiplier();
    void updateLockStates();
protected:
    QList<BCriteriaItem*> mItems;
};

#endif // BCRITERIAITEMMANAGER_H
