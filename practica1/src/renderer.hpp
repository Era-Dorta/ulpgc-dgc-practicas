#pragma once

#ifndef RENDERER_H
#define RENDERER_H

#include "vertex.hpp"
#include <string>
using namespace std;

class Renderer{

    private:
        bool perspective_;

    private:
        void triangleFillBotFlat(const Vertex& vertex0, const Vertex& vertex1, const Vertex& vertex2);
        void triangleFillTopFlat(const Vertex& vertex0, const Vertex& vertex1, const Vertex& vertex2);
        Vertex applyPerspective(const Vertex& vertex);

    public:
        Renderer(){perspective_ = false;};
        void perspective( const bool activate );
        void rPixel(const float x, const float y);
        void rLine(const Vertex& vertex0, const Vertex& vertex1);
        void rLine(const float x0, const float y0, const float x1, const float y1);
        void rLine(const float x0, const float y0, const float z0, const float x1, const float y1, const float z1);
        void rTriangle(const Vertex& vertex0, const Vertex& vertex1, const Vertex& vertex2);
        void rTriangleFill(const Vertex& vertex0, const Vertex& vertex1, const Vertex& vertex2);
        void rDrawBitmapString( string tex, const Vertex& vertex);
        void rRect( const Vertex& vertex, const float w, const float h);
};

#endif
// RENDERER_H
