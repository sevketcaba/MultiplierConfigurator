#ifndef BMATH_H
#define BMATH_H

#include <QPointF>
#include <QtMath>
#include <QObject>

class BMath
{
public:
    static QPointF getPointAtAngle(double pAngle);
    static QPointF getPointAtAngle(double pAngle, double pDistance);
    static QPointF getPointAtAngleMultiplier(double pAngle, double pMultiplier);
    static double getDistanceAtPoint(QPointF pPos);
    static double getMultiplierAtPoint(QPointF pPos);
    static QPointF scenePos(QPointF pPos);
};

class BConstants : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double maxDistance READ maxDistance WRITE setMaxDistance NOTIFY maxDistanceChanged)
    Q_PROPERTY(bool fixedMultiplierTotal READ fixedMultiplierTotal WRITE setFixedMultiplierTotal NOTIFY fixedMultiplierTotalChanged)
    Q_PROPERTY(double totalMultiplier READ totalMultiplier WRITE setTotalMultiplier NOTIFY totalMultiplierChanged)
    Q_PROPERTY(int criteriaCount READ criteriaCount WRITE setCriteriaCount NOTIFY criteriaCountChanged)
    BConstants(QObject *parent = 0);
public:
    static BConstants *instance();
    double maxDistance() const;
    bool fixedMultiplierTotal() const;
    double totalMultiplier() const;
    int criteriaCount() const;

public slots:
    void setMaxDistance(double pMaxDistance);
    void setFixedMultiplierTotal(bool pFixedMultiplierTotal);
    void setTotalMultiplier(double pTotalMultiplier);
    void setCriteriaCount(int pCriteriaCount);

signals:
    void maxDistanceChanged(double maxDistance);
    void fixedMultiplierTotalChanged(bool fixedMultiplierTotal);

    void totalMultiplierChanged(double totalMultiplier);

    void criteriaCountChanged(int criteriaCount);

private:
    double mMaxDistance;
    bool mFixedMultiplierTotal;
    double mTotalMultiplier;
    int mCriteriaCount;
};

#endif // BMATH_H
