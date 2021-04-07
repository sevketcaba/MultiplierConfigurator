#include "bcriteriatable.h"
#include "bcriteriaitem.h"
#include "bscene.h"
#include "bview.h"
#include "bwidget.h"

#include <QDockWidget>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QStatusBar>
#include <bcriteriaitemmanager.h>

#include "bmath.h"

BWidget::BWidget(QWidget *parent)
    : QMainWindow(parent)
{
    setMinimumSize(640,480);
    setFocusPolicy(Qt::StrongFocus);
    mView = new BView;
    setCentralWidget(mView);

    mLabelCoords = new QLabel;
    mStatusBar = new QStatusBar;
    mStatusBar->addWidget(mLabelCoords);
    setStatusBar(mStatusBar);

    connect(mView, &BView::cursorPos, this, [=](QPointF pPos){
       mLabelCoords->setText(QString("%1 %2").arg(pPos.x(), 0, 'f', 4).arg(pPos.y(), 0, 'f', 4));
    });

    mCriteriaTable = new BCriteriaTable;
    connect(mCriteriaTable, &BCriteriaTable::removeRequested,this, [=](int pIndex){
        qDebug() << "Remove At" << pIndex;
        BCriteriaItem *tItem = BCriteriaItemManager::instance()->itemAt(pIndex);
        BCriteriaItemManager::instance()->removeItem(tItem);
    });

    connect(mCriteriaTable, &BCriteriaTable::addRequested,this, [=](){
        bool tOk = false;
        QString tName;
        do
        {
            tName = QInputDialog::getText(centralWidget(), "Criteria Name", "Name", QLineEdit::Normal, "", &tOk);
            if(tOk)
            {
                if(!tName.isEmpty())
                    break;
            }
            else
                return;
        }
        while(true);

        BCriteriaItem *tItem = new BCriteriaItem(tName);
        BCriteriaItemManager::instance()->addItem(tItem);
    });

    QDockWidget *tWidget = new QDockWidget;
    tWidget->setWidget(mCriteriaTable);
    addDockWidget(Qt::LeftDockWidgetArea, tWidget);

    QStringList tCriterias;
    tCriterias << "Criteria A" << "Criteria B" << "Criteria C";
    double tMultiplier = BConstants::instance()->totalMultiplier() / tCriterias.size();
    double tMultiplierSoFar = 0.0;
    int tIndex = 0;
    for(QString tCriteria : tCriterias)
    {
        ++tIndex;
        BCriteriaItem *tItem = new BCriteriaItem(tCriteria);
        if(tIndex == tCriterias.size())
            tItem->setMultiplier(BConstants::instance()->totalMultiplier() - tMultiplierSoFar);
        else
        {
            tMultiplierSoFar += tMultiplier;
            tItem->setMultiplier(tMultiplier);
        }

        BCriteriaItemManager::instance()->addItem(tItem);
    }
    mView->refresh();

}

BWidget::~BWidget()
{

}
