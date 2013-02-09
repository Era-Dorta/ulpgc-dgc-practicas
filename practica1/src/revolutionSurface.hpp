#ifndef REVOLUTION_SURFACE_H
#define REVOLUTION_SURFACE_H

#include "vertex.hpp"
#include "drawableObject.hpp"

class RevolutionSurface : public DrawableObject{

    public:
        RevolutionSurface();
        RevolutionSurface( Vertex vertex0, Vertex vertex1 );
        virtual void setVertices( Vertex vertex0, Vertex vertex1 );
        virtual void draw();
};

#endif
// REVOLUTION_SURFACE_H
