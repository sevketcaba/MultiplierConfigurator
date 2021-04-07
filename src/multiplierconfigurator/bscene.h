#ifndef BSCENE_H
#define BSCENE_H

#include <QGraphicsScene>
#include <QObject>

class BCriteriaItem;

class BScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit BScene(QObject *parent = nullptr);

    QGraphicsItem *backgroundItem();
signals:
    void cursorPos(QPointF);
public slots:

    void itemPositionChanged(bool pUseLockedItems = false);

    void refresh();
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    void drawForeground(QPainter *painter, const QRectF &rect);

    QGraphicsItemGroup *mRoot;
    QGraphicsItemGroup *mLineGroup;
    QGraphicsItemGroup *mLabelGroup;
    QGraphicsPathItem *mOuterPath;
    QGraphicsPathItem *mInnerPath;
    QGraphicsRectItem *mBackgroundRect;

    QBrush mBrushGray;
    QPen mPenGray;
    QPen mPenYellow;
    QPen mPenWhite;
};

#endif // BSCENE_H
