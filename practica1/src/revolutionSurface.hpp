#ifndef REVOLUTION_SURFACE_H
#define REVOLUTION_SURFACE_H

#include "vertex.hpp"
#include "drawableObject.hpp"

class RevolutionSurface : public DrawableObject{

    private:
        bool hasAllVertices_;
        int lineVerticesAmount;
        Vertex pmouse;
    public:
        RevolutionSurface();
        void setVertex( Vertex &vertex );
        void noMoreVertices();
        void setDrawHelper( Vertex &mouse );
        virtual void draw();
        bool hasAllVertices() { return hasAllVertices_; }
};

#endif
// REVOLUTION_SURFACE_H