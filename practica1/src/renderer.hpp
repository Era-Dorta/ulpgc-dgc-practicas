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
        void rDrawBitmapString( string tex, const Vertex& vertex);
};

#endif
// RENDERER_H
