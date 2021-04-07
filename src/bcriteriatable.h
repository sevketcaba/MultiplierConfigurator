#ifndef BCRITERIATABLE_H
#define BCRITERIATABLE_H

#include <QWidget>
#include <QQmlListProperty>
#include <QQuickWidget>

#include "bcriteriaitem.h"

class BCriteriaTable : public QQuickWidget
{
    Q_OBJECT
public:
    explicit BCriteriaTable(QWidget *parent = nullptr);

signals:
    void itemsChanged();
    void addRequested();
    void removeRequested(int pIndex);

public slots:

    void requestAdd();
    void requestRemove(int pIndex);

};

#endif // BCRITERIATABLE_H
