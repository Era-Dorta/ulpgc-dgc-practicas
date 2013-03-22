#ifndef CUBE_H
#define CUBE_H

#include "vertex.hpp"
#include "drawableObject.hpp"

class Cube : public DrawableObject{

    protected:
        virtual void calculateNormals();

    public:
        Cube( const ofColor color_ = ofColor::white );
        Cube( const Vertex &vertex0, const Vertex &vertex1 );
        void setVertices( const Vertex &vertex0, const Vertex &vertex1 );
        virtual void draw(Renderer* const renderer) const;
};

#endif
// CUBE_H
