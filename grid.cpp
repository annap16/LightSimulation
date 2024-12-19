#include "grid.h"
#include <QImage>
#include <QColor>
#include <QPainter>
#include <tuple>
#include <QGraphicsScene>
#include <mainwindow.h>
#include <QQuaternion>
#include <QColor>
#include <QBitmap>
#include <QTimer>
#include <QCoreApplication>
#include <QDir>

#include "vertex.h"
#include "pointinfo.h"
#include "triangle.h"
#include "paintingparams.h"
//#include "lightanimation.h"


Grid::Grid(QGraphicsScene* _scene, QVector<QVector3D*> points)
{
    scene = _scene;
    QVector3D* lightPosPom = new QVector3D(50, 0, 100);
    lightAnimation = new LightAnimation(this, lightPosPom);
    QObject::connect(lightAnimation, &LightAnimation::lightPositionUpdated, this, &Grid::updateLightPosition);
    image = new QImage(scene->width(), scene->height(), QImage::Format_RGB32);
    controlPoints  = points;
    paintParams = new PaintingParams(Qt::blue, Qt::white, 0.8, 0.5, 50.0, nullptr, nullptr, lightPosPom);
    bezierGridSize = 18;
    step = 1.0/bezierGridSize;
    vertexList = createVertexList();
    trianglesList = createTrianglesList();
    slider2Value = 45;
    slider3Value = 0;
    drawGrid = true;
    triangleFilling = false;
    paintParams->refelctors = false;
    paintParams->light = false;
    scene->addItem(this);
    this->update();
}

void Grid::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if (image)
    {
        QPainter imagePainter(image);
        imagePainter.translate(image->width() / 2, image->height() / 2);
        image->fill(Qt::white);

        if(triangleFilling || paintParams->image || paintParams->normalMap)
        {
            for(int i=0; i<trianglesList.size(); i++)
            {
                trianglesList[i]->paint(&imagePainter, nullptr, nullptr);
            }
        }

        if(drawGrid)
        {
            imagePainter.setPen(Qt::black);
            for(int i=0; i<trianglesList.size(); i++)
            {
                trianglesList[i]->paintOutline(&imagePainter);
            }
        }

        int r = 4;
        imagePainter.setBrush(Qt::white);
        QRectF rect(paintParams->lightPos->x() - r, paintParams->lightPos->y() - r, 2 * r, 2 * r);
        imagePainter.drawEllipse(rect);

        painter->drawImage(0, 0, *image);
    }

}

void Grid::updateLightPosition()
{
    update();
}


void Grid::resizeImage(int width, int height)
{
    prepareGeometryChange();
    delete image;
    image = new QImage(width, height, QImage::Format_RGB32);
    this->update();
}

QRectF Grid::boundingRect() const
{
    return QRectF(0, 0, image->width(), image->height());
}

QVector<Vertex*> Grid::createVertexList()
{
    double uPom=0;
    double vPom=0;
    QVector<Vertex*> ret;
    for(int i=0; i<bezierGridSize; i++)
    {
        vPom=0;
        for(int j=0; j<bezierGridSize; j++)
        {
            Vertex* vertexPom = new Vertex(uPom, vPom, controlPoints);
            ret.append(vertexPom);
            vPom+=step;
        }
        uPom+=step;
    }
    return ret;
}

QVector<Triangle*> Grid::createTrianglesList()
{
    QVector<Triangle*> ret;
    for(int i=0; i<bezierGridSize-1; i++)
    {
        for(int j=0; j<bezierGridSize-1; j++)
        {
            Triangle* newTriangle = new Triangle(vertexList[i*bezierGridSize + j], vertexList[i*bezierGridSize+j+1], vertexList[(i+1)*bezierGridSize+j], paintParams);
            Triangle* newTriangle2 = new Triangle(vertexList[i*bezierGridSize+j+1], vertexList[(i+1)*bezierGridSize+j], vertexList[(i+1)*bezierGridSize+j+1], paintParams);
            ret.append(newTriangle);
            ret.append(newTriangle2);
        }
    }
    return ret;
}

void Grid::onSliderChangedPrecision(int value)
{
    step = 1.0/(value-1);
    bezierGridSize = value;
    for (Vertex* vertex : vertexList)
    {
        delete vertex;
    }
    vertexList.clear();
    for (Triangle* triangle : trianglesList)
    {
        delete triangle;
    }
    trianglesList.clear();

    vertexList = createVertexList();
    trianglesList = createTrianglesList();
    onSliderChangedAlpha(slider2Value);
    onSliderChangedBetha(slider3Value);
    this->update();
}

void Grid::onSliderChangedAlpha(int value)
{
    slider2Value = value;
    rotateAround();
}

void Grid::onSliderChangedBetha(int value)
{
    slider3Value = value;
    rotateAround();
}
void Grid::onSliderChangedML(int value)
{
    paintParams->mL = value;
    update();
}


void Grid::rotateAround()
{
    QVector3D axisZ(0, 0, 1);
    QVector3D axisX(1, 0, 0);
    QQuaternion rotationZ = QQuaternion::fromAxisAndAngle(axisZ, slider2Value-45);
    QQuaternion rotationX = QQuaternion::fromAxisAndAngle(axisX, slider3Value);
    for(int i=0; i<vertexList.size(); i++)
    {
        *(vertexList[i]->pointAfterRot->point) = rotationX.rotatedVector(*(vertexList[i]->pointBeforeRot->point));
        *(vertexList[i]->pointAfterRot->tanVectorPu) = rotationX.rotatedVector(*(vertexList[i]->pointBeforeRot->tanVectorPu));
        *(vertexList[i]->pointAfterRot->tanVectorPv) = rotationX.rotatedVector(*(vertexList[i]->pointBeforeRot->tanVectorPv));

        *(vertexList[i]->pointAfterRot->point) = rotationZ.rotatedVector(*(vertexList[i]->pointAfterRot->point));
        *(vertexList[i]->pointAfterRot->tanVectorPu) = rotationZ.rotatedVector(*(vertexList[i]->pointAfterRot->tanVectorPu));
        *(vertexList[i]->pointAfterRot->tanVectorPv) = rotationZ.rotatedVector(*(vertexList[i]->pointAfterRot->tanVectorPv));

        *(vertexList[i]->pointAfterRot->normalVector) = QVector3D::crossProduct(*(vertexList[i]->pointAfterRot->tanVectorPu), *(vertexList[i]->pointAfterRot->tanVectorPv));
    }
    this->update();
}

void Grid::onCheckBoxStateChanged(int state)
{
    if (state == Qt::Checked)
    {
        drawGrid = true;
    }
    else if (state == Qt::Unchecked)
    {
        drawGrid = false;
    }
    this->update();
}

void Grid::onCheckBox2StateChanged(int state)
{
    if (state == Qt::Checked)
    {
        triangleFilling = true;
    }
    else if (state == Qt::Unchecked)
    {
        triangleFilling = false;
    }
    this->update();
}

void Grid::onCheckBox3StateChanged(int state)
{
    if(state==Qt::Checked)
    {
        if(paintParams->image)
        {
            delete paintParams->image;
            paintParams->image = nullptr;
        }
        QString imagePath = QDir::currentPath() + QDir::separator() + "165.jpg";
        paintParams->image = new QImage(imagePath);
        if (paintParams->image->isNull())
        {
            qDebug() << "Unable to read a texture from a file";
            delete paintParams->image;
            paintParams->image = nullptr;
        }
    }
    else
    {
        if(paintParams->image)
        {
            delete paintParams->image;
        }
        paintParams->image = nullptr;
    }
    this->update();
}

void Grid::onCheckBoxNormalMapStateChanged(int state)
{
    if(state==Qt::Checked)
    {
        QString normalMapPath = QDir::currentPath() + QDir::separator() + "165_norm.jpg";
        if(paintParams->normalMap)
        {
            delete paintParams->normalMap;
            paintParams->normalMap = nullptr;
        }
        paintParams->normalMap = new QImage(normalMapPath);
        if(paintParams->normalMap->isNull())
        {
            qDebug() << "Unable to read a normal map from a file";
            delete paintParams->normalMap;
            paintParams->normalMap = nullptr;
        }
    }
    else
    {
        if(paintParams->normalMap)
        {
            delete paintParams->normalMap;
        }
        paintParams->normalMap = nullptr;
    }
    this->update();
}
void Grid::onCheckBoxLightStateChanged(int state)
{
    if(state==Qt::Checked)
    {
        lightAnimation->moving = true;
        lightAnimation->startTimer();
    }
    else
    {
        lightAnimation->moving = false;
        lightAnimation->stopTimer();
    }
}

void Grid::onCheckBoxTurnOffLightStateChanged(int state)
{
    if(state==Qt::Checked)
    {
        paintParams->light = true;
    }
    else
    {
        paintParams->light = false;
    }
    update();

}

void Grid::onCheckBoxReflectorsStateChanged(int state)
{
    if(state==Qt::Checked)
    {
        paintParams->refelctors = true;
    }
    else
    {
        paintParams->refelctors = false;
    }
    update();

}





void Grid::onSliderChangedKd(int value)
{
    paintParams->Kd = value/100.0;
    update();
}

void Grid::onSliderChangedKs(int value)
{
    paintParams->Ks = value/100.0;
    update();
}

void Grid::onSliderChangedM(int value)
{
    paintParams->m = value;
    update();
}

void Grid::onSliderChangedZ(int value)
{
    paintParams->lightPos->setZ(value);
    update();
}

Grid::~Grid()
{
    for(int i=0; i<trianglesList.size();i++)
    {
        delete trianglesList[i];
        trianglesList[i] = nullptr;
    }
    for(int i=0; i<vertexList.size(); i++)
    {
        delete vertexList[i];
        vertexList[i] = nullptr;
    }
    for(int i=0; i<controlPoints.size(); i++)
    {
        delete controlPoints[i];
        controlPoints[i] = nullptr;
    }
    delete paintParams;
    paintParams = nullptr;
    if(lightAnimation->moving)
    {
        lightAnimation->moving= false;
        lightAnimation->stopTimer();
    }
    delete lightAnimation;
    lightAnimation = nullptr;
    delete image;
    image = nullptr;
}








