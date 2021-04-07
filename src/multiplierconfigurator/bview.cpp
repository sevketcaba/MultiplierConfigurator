#include "bscene.h"
#include "bview.h"

#include <QWheelEvent>
#include <QDebug>

BView::BView(QWidget *parent) :
    QGraphicsView(parent)
{
    setBackgroundBrush(QBrush(Qt::black));

    mScene = new BScene;
    connect(mScene, &BScene::cursorPos, this, &BView::cursorPos);
    setScene(mScene);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setFocusPolicy(Qt::StrongFocus);

    setRenderHints(QPainter::Antialiasing|QPainter::SmoothPixmapTransform);
    setRenderHint(QPainter::NonCosmeticDefaultPen, false);


    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setDragMode(ScrollHandDrag);
    setMouseTracking(true);
}

void BView::refresh()
{
    mScene->refresh();
    fitInView(mScene->backgroundItem(), Qt::KeepAspectRatio);
}

void BView::resizeEvent(QResizeEvent *event)
{
    refresh();
    QGraphicsView::resizeEvent(event);
}

void BView::wheelEvent(QWheelEvent *pEvent)
{
    double tFactor = pEvent->angleDelta().y() > 0 ? 1.1 : 0.9;
    scale(tFactor, tFactor);
//    event->accept();
}

void BView::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);
}

void BView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
}

void BView::mouseMoveEvent(QMouseEvent *event)
{
    update();
    QGraphicsView::mouseMoveEvent(event);
}
