#include "bcriteriatable.h"

#include <QQmlContext>
#include <bcriteriaitemmanager.h>
#include "bmath.h"

BCriteriaTable::BCriteriaTable(QWidget *parent) :
    QQuickWidget(parent)
{
    qmlRegisterType<BCriteriaItem>();
    qmlRegisterType<BCriteriaTable>();
    qmlRegisterType<BConstants>();
    setResizeMode(QQuickWidget::SizeRootObjectToView);
    rootContext()->setContextProperty("itemmanager", BCriteriaItemManager::instance());
    rootContext()->setContextProperty("table", this);
    rootContext()->setContextProperty("constants", BConstants::instance());

    setSource(QUrl(QStringLiteral("qrc:/ui/qml/main.qml")));
}

void BCriteriaTable::requestAdd()
{
    emit addRequested();
}

void BCriteriaTable::requestRemove(int pIndex)
{
    emit removeRequested(pIndex);
}
