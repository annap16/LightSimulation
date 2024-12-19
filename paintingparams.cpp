#include "paintingparams.h"
#include <QImage>
#include <QVector3D>

PaintingParams::PaintingParams(QColor _gridColor, QColor _lightColor, double _Kd, double _Ks, double _m, QImage* _image, QImage* _normalMap, QVector3D* _lightPos): gridColor(_gridColor), lightColor(_lightColor), Kd(_Kd), Ks(_Ks), m(_m), image(_image), normalMap(_normalMap), lightPos(_lightPos)
{
    mL = 5;
}
PaintingParams::~PaintingParams()
{
    if(normalMap)
    {
        delete normalMap;
        normalMap = nullptr;
    }
    if(image)
    {
        delete image;
        image = nullptr;
    }
    delete lightPos;
    lightPos = nullptr;
}


