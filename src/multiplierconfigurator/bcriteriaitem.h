#ifndef BCRITERIAITEM_H
#define BCRITERIAITEM_H

#include "bmath.h"

#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QObject>
#include <QPen>
#include <QTextBlockFormat>
#include <QTextCursor>
class BCriteriaItem;

class BCriteriaLabelItem : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT
public:
    BCriteriaLabelItem(BCriteriaItem *pCriteriaItem);
public slots:
    void updatePosition();
protected:
    QGraphicsTextItem *mNameLabel;
    QGraphicsTextItem *mMultiplierLabel;
    QGraphicsRectItem *mRectangle;
    BCriteriaItem *mCriteriaItem;
};
class BCriteriaSceneItem : public QGraphicsItemGroup
{
public:
    BCriteriaSceneItem(BCriteriaItem *pCriteriaItem);

    void updateColor(QColor pColor);
protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *);
    void mousePressEvent(QGraphicsSceneMouseEvent *pEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *pEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *);

    BCriteriaItem *mCriteriaItem;
    QGraphicsEllipseItem *mEllipse;
};

class BCriteriaItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isLocked READ isLocked WRITE setIsLocked NOTIFY isLockedChanged)
    Q_PROPERTY(double angle READ angle WRITE setAngle NOTIFY angleChanged)
    Q_PROPERTY(double multiplier READ multiplier WRITE setMultiplier NOTIFY multiplierChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(int index READ index CONSTANT)


public:
    explicit BCriteriaItem(QString pName, QObject *parent = nullptr);

    bool isLocked() const;
    double angle() const;
    double multiplier() const;

    QGraphicsItemGroup *sceneItem();
    QGraphicsItemGroup *labelItem();

    QString name() const;

    int index() const;

    QPointF scenePos() const;

signals:
    void angleChanged(double angle);
    void isLockedChanged(bool isLocked);
    void positionChanged();
    void multiplierChanged(double multiplier);
    void nameChanged(QString name);

public slots:
    void setIsLocked(bool pIsLocked);
    void setAllowedToMove(bool pAllowedToMove);

    void updateColor();
    void setMultiplier(double pMultiplier);
    void setName(QString pName);

protected slots:
    void updatePosByMultiplier();

    void setAngle(double pAngle);
protected:
    bool mIsLocked;
    bool mAllowedToMove;
    bool mIsHovered;
    bool mIsPressed;

    double mAngle;
    int mIndex;

    QPointF mPressedPos;

    double mMultiplier;
    QString mName;

    static int mIndexCounter;

    BCriteriaSceneItem *mSceneItem;
    BCriteriaLabelItem *mLabel;

    friend class BScene;
    friend class BCriteriaLabelItem;
    friend class BCriteriaSceneItem;
};

#endif // BCRITERIAITEM_H
