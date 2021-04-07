#include "bmath.h"

QPointF BMath::getPointAtAngle(double pAngle, double pDistance)
{
    QPointF tPoint;
    double tDistance = pDistance;

    tPoint.setX( tDistance * qSin(qDegreesToRadians(pAngle)) );
    tPoint.setY( -tDistance * qCos(qDegreesToRadians(pAngle)) );

    return tPoint;
}

QPointF BMath::getPointAtAngle(double pAngle)
{
    return getPointAtAngleMultiplier(pAngle, BConstants::instance()->totalMultiplier());
}

double BMath::getDistanceAtPoint(QPointF pPos)
{
    return qSqrt(qPow(pPos.x(), 2) + qPow(pPos.y(), 2));
}

QPointF BMath::getPointAtAngleMultiplier(double pAngle, double pMultiplier)
{
    double tPow = 1.0; //BConstants::instance()->criteriaCount() - 2.0;
    double tMultiplier = qPow(pMultiplier, 1.0/tPow);
    return getPointAtAngle(pAngle, tMultiplier*BConstants::instance()->maxDistance());
}

double BMath::getMultiplierAtPoint(QPointF pPos)
{
    double tPow = 1.0; //BConstants::instance()->criteriaCount() - 2.0;
    return qPow(qSqrt(qPow(pPos.x(), 2) + qPow(pPos.y(), 2)) / BConstants::instance()->maxDistance(), tPow);
}

QPointF BMath::scenePos(QPointF pPos)
{
    return QPointF(pPos.x(), -pPos.y());
}

BConstants::BConstants(QObject *parent) :
    QObject(parent)
  , mMaxDistance(1000.0)
  , mFixedMultiplierTotal(true)
  , mTotalMultiplier(1.0)
  , mCriteriaCount(0)
{

}

BConstants *BConstants::instance()
{
    static BConstants tInstance;
    return &tInstance;
}

double BConstants::maxDistance() const
{
    return mMaxDistance;
}

bool BConstants::fixedMultiplierTotal() const
{
    return mFixedMultiplierTotal;
}

double BConstants::totalMultiplier() const
{
    return mTotalMultiplier;
}

int BConstants::criteriaCount() const
{
    return mCriteriaCount;
}

void BConstants::setMaxDistance(double pMaxDistance)
{
    if (qFuzzyCompare(mMaxDistance, pMaxDistance))
        return;

    mMaxDistance = pMaxDistance;
    emit maxDistanceChanged(mMaxDistance);
}

void BConstants::setFixedMultiplierTotal(bool pFixedMultiplierTotal)
{
    if (mFixedMultiplierTotal == pFixedMultiplierTotal)
        return;

    mFixedMultiplierTotal = pFixedMultiplierTotal;
    emit fixedMultiplierTotalChanged(mFixedMultiplierTotal);
}

void BConstants::setTotalMultiplier(double pTotalMultiplier)
{
    if (qFuzzyCompare(mTotalMultiplier, pTotalMultiplier))
        return;

    mTotalMultiplier = pTotalMultiplier;
    emit totalMultiplierChanged(mTotalMultiplier);
}

void BConstants::setCriteriaCount(int pCriteriaCount)
{
    if (mCriteriaCount == pCriteriaCount)
        return;

    mCriteriaCount = pCriteriaCount;
    emit criteriaCountChanged(mCriteriaCount);
}
