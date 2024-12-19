#ifndef PAINTINGPARAMS_H
#define PAINTINGPARAMS_H
#include <QColor>
class QImage;
class QVector3D;

struct PaintingParams
{
public:
    PaintingParams(QColor _gridColor, QColor _lightColor, double _Kd, double _Ks, double _m, QImage* _image, QImage* _normalMap, QVector3D* _lightPos);
    ~PaintingParams();
    QColor gridColor;
    QColor lightColor;
    double Kd;
    double Ks;
    double m;
    bool refelctors;
    bool light;
    int mL;
    QImage* image;
    QImage* normalMap;
    QVector3D* lightPos;
};

#endif // PAINTINGPARAMS_H
