#ifndef BVIEW_H
#define BVIEW_H

#include <QWidget>
#include <QGraphicsView>

class BScene;
class BCriteriaItem;

class BView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit BView(QWidget *parent = nullptr);

signals:
    void cursorPos(QPointF);
public slots:
    void refresh();
protected:
    void resizeEvent(QResizeEvent *event);
    void wheelEvent(QWheelEvent *event);

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
protected:
    BScene *mScene;
};

#endif // BVIEW_H
