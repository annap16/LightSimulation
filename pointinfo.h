#ifndef POINTINFO_H
#define POINTINFO_H
class QVector3D;

struct PointInfo
{
    PointInfo(QVector3D* _point, QVector3D* _tanVectorPu, QVector3D* _tanVectorPv, QVector3D* normalVector);
    ~PointInfo();
    QVector3D* point;
    QVector3D* tanVectorPu;
    QVector3D* tanVectorPv;
    QVector3D* normalVector;
};

#endif // POINTINFO_H
