#ifndef TRIANGLE_H
#define TRIANGLE_H

class Vertex;
class QVector3D;
class QPainter;
class PaintingParams;
#include <QVector>
#include <QGraphicsItem>
#include "aetstruct.h"

class Triangle: public QGraphicsItem
{
public:
    Triangle(Vertex* _a, Vertex* _b, Vertex* _c, PaintingParams* _paintParams);
    ~Triangle()=default;
    QVector<Vertex*> vertexList;
    QVector<AETStruct> AETList;
    QVector<int> vertexSorted;
    PaintingParams* paintParams;
    void paintOutline(QPainter* painter);
    void sortList(Vertex* _a, Vertex* _b, Vertex* _c);
    int getPrevInd(int indx);
    int getNextInd(int indx);
    void addToAET(Vertex* a, Vertex* b, double x, double yMax);
    void deleteFromAET(double yMax);
    void updateAET(QPainter *painter, int yScanLine);
    void fillScanLine(QPainter *painter, int yScanLine);
    int doubleToInt(double x);

    QVector3D normalizeVector(QVector3D& vec);
    double cosineAngle(QVector3D& vec1, QVector3D& vec2);
    QColor calculateFillColor(QVector3D pointPosition,
                               QVector3D lightPosition,
                               QVector3D normalVector,
                               QVector3D objectColor,
                               QVector3D lightColor,
                              double kd, double ks, double m
                              );
    std::tuple<double, double, double> calculateBarycentricCoords(QVector3D vectorP);
    QVector3D interpolateNormalVector(QVector3D pointPosition);
    QVector3D interpolateNormalVectorFromMap(QVector3D pointPosition, QVector3D normalVecGrid);
    QVector3D calculatePointPosition(int x, int y);
    QVector2D calculatePointPositionFromSurface(int x, int y, QImage* surface);
    QVector3D getColorFromSurface(double x, double y, QImage* surface);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    QRectF boundingRect() const override;
    friend class Grid;
};

#endif // TRIANGLE_H
