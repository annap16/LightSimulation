#include "pointinfo.h"
#include <QVector3D>

PointInfo::PointInfo(QVector3D* _point, QVector3D* _tanVectorPu, QVector3D* _tanVectorPv, QVector3D* _normalVector)
{
    point = _point;
    tanVectorPu = _tanVectorPu;
    tanVectorPv = _tanVectorPv;
    normalVector = _normalVector;
}

PointInfo::~PointInfo()
{
    if(point)
    {
        delete point;
        point = nullptr;
    }
    if(tanVectorPu)
    {
        delete tanVectorPu;
        tanVectorPu = nullptr;
    }
    if(tanVectorPv)
    {
        delete tanVectorPv;
        tanVectorPv = nullptr;
    }
    if(normalVector)
    {
        delete normalVector;
        normalVector = nullptr;
    }
}


