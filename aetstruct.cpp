#include "aetstruct.h"
#include "vertex.h"

AETStruct::AETStruct(double _yMax, double _x, double _mInv, Vertex* _a, Vertex* _b)
{
    yMax = _yMax;
    x = _x;
    mInv = _mInv;
    a = _a;
    b = _b;
}
