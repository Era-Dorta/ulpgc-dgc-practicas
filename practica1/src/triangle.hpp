#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vertex.hpp"
#include "drawableObject.hpp"

class Triangle : public DrawableObject{

    protected:
        virtual void calculateNormals();

    public:
        Triangle( const ofColor color_ = ofColor::white );
        Triangle( const Vertex &vertex0, const Vertex &vertex1 );
        void setVertices( const Vertex &vertex0, const Vertex &vertex1 );
        void setVertices( const Vertex &vertex0, const Vertex &vertex1, const Vertex &vertex2 );
        virtual void draw(Renderer* const renderer) const;
        virtual void setDrawTriangles( const bool activate ){};
        virtual bool getDrawTriangles() const { return drawTriangles_; };
};

#endif
// TRIANGLE_H
