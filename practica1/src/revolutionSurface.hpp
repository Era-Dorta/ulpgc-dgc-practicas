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
        RevolutionSurface( const ofColor color_ = ofColor::white );
        void setVertex( const Vertex &vertex );
        void noMoreVertices();
        void setDrawHelper( const Vertex &mouse );
        virtual void draw(Renderer* const renderer) const;
        bool hasAllVertices() const { return hasAllVertices_; }
};

#endif
// REVOLUTION_SURFACE_H
