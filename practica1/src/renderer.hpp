#pragma once

#ifndef RENDERER_H
#define RENDERER_H

#include "vertex.hpp"
#include <string>
using namespace std;

class Renderer{

    private:
        bool perspective_;

    public:
        Renderer(){perspective_ = false;};
        void perspective( const bool activate );
        void rLine(const Vertex& vertex0, const Vertex& vertex1);
        void rTriangle(const Vertex& vertex0, const Vertex& vertex1, const Vertex& vertex3);
        void rTriangleFill(const Vertex& vertex0, const Vertex& vertex1, const Vertex& vertex3);
        void rDrawBitmapString( string tex, const Vertex& vertex);
        void rRect( const Vertex& vertex, const float w, const float h);
};

#endif
// RENDERER_H
