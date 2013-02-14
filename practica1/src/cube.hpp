#ifndef CUBE_H
#define CUBE_H

#include "vertex.hpp"
#include "drawableObject.hpp"

class Cube : public DrawableObject{

    public:
        Cube( ofColor color_ = ofColor::white );
        Cube( Vertex &vertex0, Vertex &vertex1 );
        void setVertices( Vertex &vertex0, Vertex &vertex1 );
        virtual void draw(Renderer* renderer);
};

#endif
// CUBE_H
