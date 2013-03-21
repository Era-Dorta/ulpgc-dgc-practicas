#ifndef LIGHT_SOURCE_H
#define LIGHT_SOURCE_H

#include "vertex.hpp"
#include "drawableObject.hpp"

class LightSource : public DrawableObject{

    public:
        LightSource( const ofColor color_ = ofColor::white );
        LightSource( const Vertex &vertex );
        void setVertex( const Vertex &vertex );
        virtual void draw(Renderer* const renderer) const;
        virtual void setDrawTriangles( const bool activate ) { drawTriangles_ = false; };
        virtual bool getDrawTriangles() const { return drawTriangles_; };
        virtual void setNormals( const bool activate ) { drawNormals_ = false; };
        virtual bool getNormals() const { return drawNormals_; };
        virtual void setFillTriangles( const bool activate ) { drawFillTriangles_ = false; };
        virtual bool getFillTriangles() const { return drawFillTriangles_; };
};

#endif
// LIGHT_SOURCE_H

