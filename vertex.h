#ifndef VERTEX_H
#define VERTEX_H
#include <QVector>
class PointInfo;
class QVector3D;
class Vertex
{
public:
    Vertex(double _u, double _v,  QVector<QVector3D*> _controlPoints);
    ~Vertex();
    const PointInfo* pointBeforeRot;
    PointInfo* pointAfterRot;
    double u;
    double v;
    QVector<QVector3D*> controlPoints;
    const static double binomialN3[4];
    const static double binomialN2[3];
    QVector3D* calculatePoint();
    QVector3D* calculatePu();
    QVector3D* calculatePv();
    double berenstain(int i, double t, int N);

};

#endif // VERTEX_H
