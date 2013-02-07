#ifndef DRAWABLE_OBJECT_H
#define DRAWABLE_OBJECT_H

#include "vertex.hpp"
#include <cmath>
using namespace std;

enum Axis { X, Y, Z };

class DrawableObject {

    protected:
        double transMatrix[4][4];
        double auxMatrix[4][4];
        void multiplyMatrix( double matrix0[4][4], double matrix1[4][4], int firstSave = 1 );

    public:
        virtual void draw(){};
        virtual void rotate( Axis axis, double amount, int permanent){};
        virtual void translate( double tX, double tY, int permanent){};
        virtual void resetTransMatrix();
        virtual void resetAuxMatrix();
};

#endif
// DRAWABLE_OBJECT_H
