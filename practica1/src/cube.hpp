#ifndef CUBE_H
#define CUBE_H

#include "vertex.hpp"
#include "drawableObject.hpp"

class Cube : public DrawableObject{

    public:
        Cube();
        Cube( Vertex &vertex0, Vertex &vertex1 );
        void setVertices( Vertex &vertex0, Vertex &vertex1 );
        virtual void draw();
};

#endif
// CUBE_H
