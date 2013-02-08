#ifndef CUBE_H
#define CUBE_H

#include "vertex.hpp"
#include "drawableObject.hpp"

class Cube : public DrawableObject{

    private:
        Vertex vertices[8];
        Vertex transVertices[8];

    public:
        Cube();
        Cube( Vertex vertex0, Vertex vertex1 );
        void setVertices( Vertex vertex0, Vertex vertex1 );
        virtual void draw();
        virtual void rotate( Axis axis, double amount, int permanent);
        virtual void translate( double tX, double tY, int permanent);
};

#endif
// CUBE_H
