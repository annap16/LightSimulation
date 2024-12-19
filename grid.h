#ifndef GRID_H
#define GRID_H
#include <QGraphicsItem>
#include <QVector>
#include <QVector3D>
#include <QObject>
#include <QDateTime>
#include <lightanimation.h>

class PaintingParams;
class Vector;
class QImage;
class QGraphicsScene;
class Triangle;
class Vertex;
class QBitmap;

class Grid:public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    Grid(QGraphicsScene* scene, QVector<QVector3D*> points);
    ~Grid();
    QImage* image;
    QGraphicsScene* scene;
    QVector<Triangle*> trianglesList;
    QVector<Vertex*> vertexList;
    double step;
    int bezierGridSize;
    double slider2Value;
    double slider3Value;
    bool drawGrid;
    bool triangleFilling;
    PaintingParams* paintParams;
    LightAnimation* lightAnimation;
    bool reflectorLight;
    bool turnOffLight;
    QVector<Vertex*> createVertexList();
    QVector<Triangle*> createTrianglesList();
    void resizeImage(int width, int height);
    void rotateAround();

public slots:
    void onSliderChangedPrecision(int value);
    void onSliderChangedAlpha(int value);
    void onSliderChangedBetha(int value);
    void onSliderChangedKd(int value);
    void onSliderChangedKs(int value);
    void onSliderChangedM(int value);
    void onSliderChangedZ(int value);
    void onSliderChangedML(int value);
    void onCheckBoxStateChanged(int state);
    void onCheckBox2StateChanged(int state);
    void onCheckBox3StateChanged(int state);
    void onCheckBoxNormalMapStateChanged(int state);
    void onCheckBoxLightStateChanged(int state);
    void updateLightPosition();
    void onCheckBoxTurnOffLightStateChanged(int state);
    void onCheckBoxReflectorsStateChanged(int state);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    QRectF boundingRect() const override;
    QVector<QVector3D*> controlPoints;
};

#endif // GRID_H
