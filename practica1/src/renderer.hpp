#pragma once

#ifndef RENDERER_H
#define RENDERER_H

#include "ofMain.h"
#include "vertex.hpp"
#include <string>
using namespace std;

class Renderer{

    private:
        int w;
        int h;
        float **zBuffer;
        bool perspective_;
        bool useZBuffer;
        ofColor currentColor;
        bool useLight;

    private:
        void range( float& val, const float& min, const float&  max) const;
        void triangleFillBotFlat(const Vertex& vertex0, const Vertex& vertex1, const Vertex& vertex2) const;
        void triangleFillTopFlat(const Vertex& vertex0, const Vertex& vertex1, const Vertex& vertex2) const;
        void triangleFillBotFlat(const Vertex& vertex0, const Vertex& vertex1,
            const Vertex& vertex2, const Vertex&normal, const Vertex& centroid, const float& distance ) const;
        void triangleFillTopFlat(const Vertex& vertex0, const Vertex& vertex1,
            const Vertex& vertex2, const Vertex&normal, const Vertex& centroid, const float& distance) const;
        Vertex applyPerspective(const Vertex& vertex) const;

    public:
        Renderer();
        Renderer( const int w, const int h );
        ~Renderer();
        void setup( const int w, const int h );
        void perspective( const bool activate );
        void rPixel(const float x, const float y, const float z) const;
        void rLine(const Vertex& vertex0, const Vertex& vertex1) const;
        void rLine(const float x0, const float y0, const float x1, const float y1) const;
        void rLine(const float x0, const float y0, const float z0, const float x1, const float y1, const float z1) const;
        void rTriangle(const Vertex& vertex0, const Vertex& vertex1, const Vertex& vertex2) const;
        void rTriangleFill(const Vertex& vertex0, const Vertex& vertex1, const Vertex& vertex2,
            const Vertex& normal, const Vertex& centroid, const float& distance) const;
        void drawLightSource() const;
        void rDrawBitmapString( const string tex, const Vertex& vertex) const;
        void rRect( const Vertex& vertex, const float w, const float h) const;
        void setZBuffer( const bool activate ){ useZBuffer = activate; };
        void resetZBuffer();
        ofColor getColor() const { return currentColor; };
        void setColor( const ofColor& newColor );
        bool getLight() const { return useLight; };
        void setLight( const bool& activate ){useLight = activate;};
};

#endif
// RENDERER_H
