#ifndef BWIDGET_H
#define BWIDGET_H

#include <QLabel>
#include <QMainWindow>
#include <QWidget>

class BView;
class BCriteriaTable;

class BWidget : public QMainWindow
{
    Q_OBJECT

public:
    BWidget(QWidget *parent = 0);
    ~BWidget();

protected:
    BView *mView;
    BCriteriaTable *mCriteriaTable;

    QLabel *mLabelCoords;
    QStatusBar *mStatusBar;

};

#endif // BWIDGET_H
