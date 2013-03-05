#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vertex.hpp"
#include "drawableObject.hpp"

class Triangle : public DrawableObject{

    public:
        Triangle( const ofColor color_ = ofColor::white );
        Triangle( const Vertex &vertex0, const Vertex &vertex1 );
        void setVertices( const Vertex &vertex0, const Vertex &vertex1 );
        virtual void draw(const Renderer* renderer) const;
};

#endif
// TRIANGLE_H
