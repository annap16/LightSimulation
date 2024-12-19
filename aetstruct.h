#ifndef AETSTRUCT_H
#define AETSTRUCT_H

class Vertex;

struct AETStruct
{
public:
    AETStruct(double _yMax, double _x, double _mInv, Vertex* _a, Vertex* _b);
    double yMax;
    double x;
    double mInv;
    Vertex* a;
    Vertex *b;
};

#endif // AETSTRUCT_H
