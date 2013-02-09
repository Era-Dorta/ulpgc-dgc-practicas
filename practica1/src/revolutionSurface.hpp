#ifndef REVOLUTION_SURFACE_H
#define REVOLUTION_SURFACE_H

#include "vertex.hpp"
#include "drawableObject.hpp"

class RevolutionSurface : public DrawableObject{

    private:
        bool hasAllVertices_;
        int lineVerticesAmount;

    public:
        RevolutionSurface();
        void setVertex( Vertex &vertex );
        void noMoreVertices();
        virtual void draw();
        bool hasAllVertices() { return hasAllVertices_; }
};

#endif
// REVOLUTION_SURFACE_H
