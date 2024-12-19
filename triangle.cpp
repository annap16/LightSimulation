#include "triangle.h"
#include "vertex.h"
#include "pointinfo.h"
#include <QVector3D>
#include <QDebug>
#include <QPainter>
#include <tuple>
#include <QMatrix3x3>
#include "aetstruct.h"
#include "paintingparams.h"

Triangle::Triangle(Vertex* _a, Vertex* _b, Vertex* _c, PaintingParams* _paintParams)
{
    vertexList.append(_a);
    vertexList.append(_b);
    vertexList.append(_c);
    sortList(_a, _b, _c);
    paintParams = _paintParams;
}

void Triangle::sortList(Vertex* _a, Vertex* _b, Vertex* _c)
{
    QVector<std::pair<double, int>> vertices =
    {
        {_a->pointAfterRot->point->y(), 0},
        {_b->pointAfterRot->point->y(), 1},
        {_c->pointAfterRot->point->y(), 2}
    };

    std::sort(vertices.begin(), vertices.end(), [](const auto& v1, const auto& v2) {
        return v1.first < v2.first;
    });

    vertexSorted.clear();
    for (const auto& v : vertices) {
        vertexSorted.append(v.second);
    }

}

QRectF Triangle::boundingRect() const
{
    double minX = vertexList[0]->pointAfterRot->point->x();
    double maxX = minX;
    double minY = vertexList[0]->pointAfterRot->point->y();
    double maxY = minY;

    for (int i = 1; i < vertexList.size(); ++i)
    {
        double x = vertexList[i]->pointAfterRot->point->x();
        double y = vertexList[i]->pointAfterRot->point->y();
        if (x < minX) minX = x;
        if (x > maxX) maxX = x;
        if (y < minY) minY = y;
        if (y > maxY) maxY = y;
    }

    return QRectF(minX, minY, maxX - minX, maxY - minY);
}

void Triangle::paintOutline(QPainter* painter)
{
    painter->drawLine(vertexList[0]->pointAfterRot->point->x(), vertexList[0]->pointAfterRot->point->y(),
                         vertexList[1]->pointAfterRot->point->x(), vertexList[1]->pointAfterRot->point->y());

    painter->drawLine(vertexList[0]->pointAfterRot->point->x(), vertexList[0]->pointAfterRot->point->y(),
                      vertexList[2]->pointAfterRot->point->x(), vertexList[2]->pointAfterRot->point->y());

    painter->drawLine(vertexList[1]->pointAfterRot->point->x(), vertexList[1]->pointAfterRot->point->y(),
                          vertexList[2]->pointAfterRot->point->x(), vertexList[2]->pointAfterRot->point->y());
}


void Triangle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    vertexSorted.clear();
    sortList(vertexList[0], vertexList[1], vertexList[2]);

    int indx = 1;
    int startY = vertexList[vertexSorted[0]]->pointAfterRot->point->y() + 1;
    int lastY = vertexList[vertexSorted[vertexList.size()-1]]->pointAfterRot->point->y();
    int sizeList= vertexList.size();

    Vertex* firstVert = vertexList[vertexSorted[0]];
    Vertex* prevVert = vertexList[getPrevInd(vertexSorted[0])];
    addToAET(firstVert,prevVert,  firstVert->pointAfterRot->point->x(), prevVert->pointAfterRot->point->y());

    Vertex* nextVert = vertexList[getNextInd((vertexSorted[0]))];
    addToAET(firstVert, nextVert, firstVert->pointAfterRot->point->x(), nextVert->pointAfterRot->point->y());

    for(int i=startY; i<=lastY; i++)
    {
        int doubleConv;
        if(indx<sizeList)
        {
            doubleConv = doubleToInt(vertexList[vertexSorted[indx]]->pointAfterRot->point->y());
        }
        while(indx<sizeList && doubleConv==i-1)
        {
            Vertex* prevVert = vertexList[getPrevInd(vertexSorted[indx])];
            Vertex* currentVert = vertexList[vertexSorted[indx]];
            if(prevVert->pointAfterRot->point->y()> currentVert->pointAfterRot->point->y())
            {
                addToAET(prevVert, currentVert, currentVert->pointAfterRot->point->x(), prevVert->pointAfterRot->point->y());
            }
            else
            {
                deleteFromAET(doubleConv);
            }

            Vertex* nextVert = vertexList[getNextInd(vertexSorted[indx])];
            if(nextVert->pointAfterRot->point->y()> currentVert->pointAfterRot->point->y())
            {
                addToAET(currentVert, nextVert, currentVert->pointAfterRot->point->x(), nextVert->pointAfterRot->point->y());
            }
            else
            {
                deleteFromAET(doubleConv);
            }
            indx++;
            if(indx<sizeList)
            {
                doubleConv = doubleToInt(currentVert->pointAfterRot->point->y());
            }
        }
        updateAET(painter, i);
    }
    AETList.clear();
}

int Triangle::doubleToInt(double x)
{
    if(x>0)
    {
        return std::floor(x);
    }
    else
    {
        return std::ceil(x);
    }
}
int Triangle::getPrevInd(int indx)
{
    return (indx-1 + 3)%3;
}
int Triangle::getNextInd(int indx)
{
    return (indx+1)%3;
}

void Triangle::addToAET(Vertex* a, Vertex* b, double x, double yMax)
{
    double mInv;
    if(b->pointAfterRot->point->y() == a->pointAfterRot->point->y())
    {
        mInv = 0;
    }
    else
    {
        mInv= (b->pointAfterRot->point->x() - a->pointAfterRot->point->x())/(b->pointAfterRot->point->y() - a->pointAfterRot->point->y());
    }
    AETList.append(AETStruct(yMax, x, mInv, a, b));
}

void Triangle::deleteFromAET(double yMax)
{
    for(int i=0; i<AETList.size(); i++)
    {
        int doubleConv = doubleToInt(AETList[i].yMax);

        if(doubleConv== yMax)
        {
            AETList.remove(i);
            return;
        }
    }
}

void Triangle::updateAET(QPainter *painter, int yScanLine)
{
    std::sort(AETList.begin(), AETList.end(), [](AETStruct a, AETStruct b) {return a.x < b.x; });
    fillScanLine(painter, yScanLine);
    for(int i=0; i<AETList.size(); i++)
    {
        AETList[i].x+=AETList[i].mInv;
    }
}

void Triangle::fillScanLine(QPainter *painter, int yScanLine)
{
    QVector3D objectColor(paintParams->gridColor.redF(), paintParams->gridColor.greenF(), paintParams->gridColor.blueF());
    QVector3D lightColor(paintParams->lightColor.redF(), paintParams->lightColor.greenF(), paintParams->lightColor.blueF());
    double kd = paintParams->Kd;
    double ks = paintParams->Ks;
    int m = paintParams->m;
    for(int i=0;i<AETList.size()-1; i+=2)
    {
        int xStart = AETList[i].x;
        int xEnd = AETList[i + 1].x;
        for(int j=xStart; j<=xEnd; j++)
        {
            QVector3D pointPosition;
            QVector2D coordsInSurface;

            pointPosition = calculatePointPosition(j, yScanLine);


            if(paintParams->image)
            {
                coordsInSurface = calculatePointPositionFromSurface(j, yScanLine, paintParams->image);
                objectColor = getColorFromSurface(coordsInSurface.x(), coordsInSurface.y(), paintParams->image);
            }

            QVector3D normalVector= interpolateNormalVector(pointPosition);
            normalVector.normalize();
            if(paintParams->normalMap)
            {
                normalVector = interpolateNormalVectorFromMap(QVector3D(j, yScanLine, 0), normalVector);
            }

            QColor fillColor = calculateFillColor(pointPosition, *(paintParams->lightPos), normalVector, objectColor, lightColor, kd, ks, m);
            painter->setPen(fillColor);
            painter->drawPoint(j, yScanLine);
        }
    }
}

QVector3D Triangle::normalizeVector(QVector3D& vec)
{
    return vec.normalized();
}

double Triangle::cosineAngle(QVector3D& vec1, QVector3D& vec2)
{
    double cosTheta = QVector3D::dotProduct(vec1, vec2);
    return std::max(0.0d, cosTheta);
}

std::tuple<double, double, double> Triangle::calculateBarycentricCoords(QVector3D vectorP)
{
    QVector3D v0 = *(vertexList[1]->pointAfterRot->point) - *(vertexList[0]->pointAfterRot->point);
    QVector3D v1 = *(vertexList[2]->pointAfterRot->point) - *(vertexList[0]->pointAfterRot->point);
    QVector3D v2 = vectorP - *(vertexList[0]->pointAfterRot->point);

    double d00 = QVector3D::dotProduct(v0, v0);
    double d01 = QVector3D::dotProduct(v0, v1);
    double d11 = QVector3D::dotProduct(v1, v1);
    double d20 = QVector3D::dotProduct(v2, v0);
    double d21 = QVector3D::dotProduct(v2, v1);

    double denom = d00*d11 - d01*d01;
    double u, w, v;
    v =  (d11 * d20 - d01 * d21) / denom;
    w  = (d00 * d21 - d01 * d20) / denom;
    u =  1.0f - v - w;
    return std::make_tuple(u, v, w);

}

QVector3D Triangle::calculatePointPosition(int x, int y)
{

    auto barycentricCoord = calculateBarycentricCoords(QVector3D(x, y, 0.0));
    double u = std::get<0>(barycentricCoord);
    double v = std::get<1>(barycentricCoord);
    double w = std::get<2>(barycentricCoord);

    double interpolatedZ  = vertexList[0]->pointAfterRot->point->z()*u + vertexList[1]->pointAfterRot->point->z()*v + vertexList[2]->pointAfterRot->point->z()*w;

    return QVector3D(x, y, interpolatedZ);
}

QVector2D Triangle::calculatePointPositionFromSurface(int x, int y, QImage* surface)
{
    auto barycentricCoord = calculateBarycentricCoords(QVector3D(x, y, 0));
    double barA = std::get<0>(barycentricCoord);
    double barB = std::get<1>(barycentricCoord);
    double barC = std::get<2>(barycentricCoord);

    Vertex* a = vertexList[0];
    Vertex* b = vertexList[1];
    Vertex* c = vertexList[2];

    double u = a->u*barA + b->u*barB+ c->u*barC;
    double v = a->v*barA + b->v*barB + c->v*barC;

    int surfaceX = u*surface->width();
    int surfaceY =  v*surface->height();
    surfaceX = (surfaceX < 0) ? 0 : (surfaceX >= surface->width() ? surface->width() - 1 : surfaceX);
    surfaceY = (surfaceY < 0) ? 0 : (surfaceY >= surface->height() ? surface->height() - 1 : surfaceY);

    return QVector2D(surfaceX, surfaceY);

}

QVector3D Triangle::interpolateNormalVector(QVector3D pointPosition)
{
    auto barycentricCoord = calculateBarycentricCoords(pointPosition);
    double u = std::get<0>(barycentricCoord);
    double v = std::get<1>(barycentricCoord);
    double w = std::get<2>(barycentricCoord);

    QVector3D normalA = *vertexList[0]->pointAfterRot->normalVector;
    QVector3D normalB = *vertexList[1]->pointAfterRot->normalVector;
    QVector3D normalC = *vertexList[2]->pointAfterRot->normalVector;

    QVector3D interpolatedNormal = u * normalA + v * normalB + w * normalC;
    interpolatedNormal.normalize();

    return interpolatedNormal;
}

QVector3D Triangle::interpolateNormalVectorFromMap(QVector3D pointPosition, QVector3D normalVecGrid)
{
    auto baricentricCoord = calculateBarycentricCoords(pointPosition);
    double u = std::get<0>(baricentricCoord);
    double v = std::get<1>(baricentricCoord);
    double w = std::get<2>(baricentricCoord);
    QVector3D pu = u*(*(vertexList[0]->pointAfterRot->tanVectorPu)) + v*(*(vertexList[1]->pointAfterRot->tanVectorPu)) + w*(*(vertexList[2]->pointAfterRot->tanVectorPu));
    QVector3D pv = u*(*(vertexList[0]->pointAfterRot->tanVectorPv)) + v*(*(vertexList[1]->pointAfterRot->tanVectorPv)) + w*(*(vertexList[2]->pointAfterRot->tanVectorPv));
    pu.normalize();
    pv.normalize();
    QVector2D mapCoords = calculatePointPositionFromSurface(pointPosition.x(), pointPosition.y(), paintParams->normalMap);
    QVector3D nTexture = getColorFromSurface(mapCoords.x(), mapCoords.y(), paintParams->normalMap);
    nTexture = (nTexture * QVector3D(255, 255, 255) - QVector3D(127, 127, 127)) / QVector3D(128, 128, 128);
    float M[3][3] = {
        {pu.x(), pv.x(), normalVecGrid.x()},
        {pu.y(), pv.y(), normalVecGrid.y()},
        {pu.z(), pv.z(), normalVecGrid.z()}
    };
    QVector3D N;
    N.setX(M[0][0] * nTexture.x() + M[0][1] * nTexture.y() + M[0][2] * nTexture.z());
    N.setY(M[1][0] * nTexture.x() + M[1][1] * nTexture.y() + M[1][2] * nTexture.z());
    N.setZ(M[2][0] * nTexture.x() + M[2][1] * nTexture.y() + M[2][2] * nTexture.z());
    return N.normalized();
}



QColor Triangle::calculateFillColor(
    QVector3D pointPosition,
    QVector3D lightPosition,
    QVector3D normalVector,
    QVector3D objectColor,
    QVector3D lightColor,
    double kd, double ks, double m
    )
{
    QVector3D L = (lightPosition - pointPosition);
    L.normalize();

    QVector3D N = normalizeVector(normalVector);
    double cosNL = cosineAngle(N, L);

    QVector3D V(0, 0, 1);
    QVector3D R = 2 * cosNL * N - L;
    R.normalize();

    double cosVRPowM = std::pow(cosineAngle(V, R), m);
    QVector3D I = kd*lightColor*objectColor*cosNL + ks*lightColor*objectColor*cosVRPowM;



    if(paintParams->light)
    {
        I = QVector3D(0, 0 ,0);
    }

    if(paintParams->refelctors)
    {

        QVector<QVector3D> reflectors;
        reflectors.append(QVector3D(-150.0, -150.0, lightPosition.z()));
        reflectors.append(QVector3D(150.0, -150.0, lightPosition.z()));
        reflectors.append(QVector3D(-150.0, 150.0, lightPosition.z()));

        int ml = paintParams->mL;

        QVector<QVector3D> colors;
        colors.append(QVector3D(1, 0 ,0));
        colors.append(QVector3D(0, 1 ,0));
        colors.append(QVector3D(0, 0 , 1));

        for(int i=0; i<reflectors.size();i++)
        {
            QVector3D L1 = reflectors[i] - pointPosition;
            L1.normalize();
            QVector3D LDir = reflectors[i];
            LDir.normalize();
            double cosLLDir = cosineAngle(L1, LDir);
            double pow = std::pow(cosLLDir, ml);
            QVector3D IL = colors[i]*pow;
            //IL.normalize();
            double cosNL1 = cosineAngle(N, L1);
            QVector3D R1 = 2 * cosNL1 * N - L1;
            R1.normalize();
            double cosVRPowM1 = std::pow(cosineAngle(V, R1), m);

            I += kd*IL*objectColor*cosNL1 + ks*IL*objectColor*cosVRPowM1;
        }


    }

    int R_final = std::min(static_cast<int>(I.x() * 255.0), 255);
    int G_final = std::min(static_cast<int>(I.y() * 255.0), 255);
    int B_final = std::min(static_cast<int>(I.z() * 255.0), 255);

    return QColor(R_final, G_final, B_final);
}

QVector3D Triangle::getColorFromSurface(double x, double y, QImage* surface)
{
    QColor color = surface->pixelColor(x, y);

    float r = color.redF();
    float g = color.greenF();
    float b = color.blueF();

    return QVector3D(r, g, b);
}


































