#include "vertex.h"
#include "pointinfo.h"
#include <cmath>
#include "pointinfo.h"
#include <QVector3D>

const double Vertex::binomialN3[4] = {1, 3, 3, 1};
const double Vertex::binomialN2[3] = {1, 2, 1};

Vertex::Vertex(double _u, double _v,  QVector<QVector3D*> _controlPoints)
{
    u = _u;
    v = _v;
    controlPoints = _controlPoints;
    QVector3D* pointTemp = calculatePoint();
    QVector3D* pointPuTemp = calculatePu();
    QVector3D* pointPvTemp = calculatePv();
    pointPuTemp->normalize();
    pointPvTemp->normalize();
    QVector3D normalTemp = QVector3D::crossProduct(*pointPuTemp, *pointPvTemp);
    pointAfterRot = new PointInfo(pointTemp, pointPuTemp, pointPvTemp, new QVector3D(normalTemp));
    pointBeforeRot = new PointInfo(new QVector3D(*pointTemp), new QVector3D(*pointPuTemp), new QVector3D(*pointPvTemp), new QVector3D(normalTemp));
}


QVector3D* Vertex::calculatePoint()
{
    QVector3D* point = new QVector3D(0, 0, 0);
    for(int i=0; i<4;i++)
    {
        for(int j=0; j<4; j++)
        {
            *point += (*controlPoints[i*4 + j])*berenstain(i, u, 3)*berenstain(j, v, 3);
        }
    }
    return point;
}

QVector3D* Vertex::calculatePu()
{
    QVector3D* point = new QVector3D(0, 0, 0);
    for(int i=0; i<3; i++)
    {
        for(int j=0;j<4; j++)
        {
            *point += (*controlPoints[(i+1)*4 + j] - *controlPoints[i*4+j])*berenstain(i, u, 2)*berenstain(j, v, 3);
        }
    }
    *point *=3;
    return point;
}

QVector3D* Vertex::calculatePv()
{
    QVector3D* point = new QVector3D(0, 0, 0);
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<3; j++)
        {
            *point += (*controlPoints[i*4 + j+1] - *controlPoints[i*4 + j])*berenstain(i, u, 3)*berenstain(j, v, 2);
        }
    }
    *point *= 3;
    return point;
}


double Vertex::berenstain(int i, double t, int N)
{
    if(N==3)
        return binomialN3[i]*std::pow(t, i)*std::pow(1 - t, 3-i);
    if(N==2)
        return binomialN2[i]*std::pow(t, i)*std::pow(1 - t, 2-i);
    return -1;
}

Vertex::~Vertex()
{
    if(pointBeforeRot)
    {
        delete pointBeforeRot;
        pointBeforeRot = nullptr;
    }
    if(pointAfterRot)
    {
        delete pointAfterRot;
        pointAfterRot = nullptr;
    }

}

