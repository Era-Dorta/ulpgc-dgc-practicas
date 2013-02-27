#include "renderer.hpp"
#include "ofMain.h"
#include <algorithm>    // std::sort
#include <vector>       // std::vector
using namespace std;

int k = 400;
//--------------------------------------------------------------
void Renderer::perspective( const bool activate ){
    perspective_ = activate;
}

//--------------------------------------------------------------
void Renderer::rLine(const Vertex& vertex0, const Vertex& vertex1){
    float x0 = vertex0.getX();
    float y0 = vertex0.getY();
    float z0 = vertex0.getZ();
    float x1 = vertex1.getX();
    float y1 = vertex1.getY();
    float z1 = vertex1.getZ();

    if(perspective_){
        x0 = x0 /( 1 - z0 / k);
        y0 = y0 /( 1 - z0 / k);
        x1 = x1 /( 1 - z1 / k);
        y1 = y1 /( 1 - z1 / k);
    }
    x0 = x0 + center.getX();
    y0 = y0 + center.getY();
    x1 = x1 + center.getX();
    y1 = y1 + center.getY();

    ofLine(x0,y0,x1,y1);
}

//--------------------------------------------------------------
void Renderer::rTriangle(const Vertex& vertex0, const Vertex& vertex1, const Vertex& vertex2){
    rLine(vertex0, vertex1);
    rLine(vertex0, vertex2);
    rLine(vertex1, vertex2);
}

//--------------------------------------------------------------
void Renderer::rTriangleFill(const Vertex& vertex0, const Vertex& vertex1, const Vertex& vertex2){
    vector<Vertex> vertices;
    vertices.push_back(vertex0);
    vertices.push_back(vertex1);
    vertices.push_back(vertex2);
    Vertex v4 = vertex0;

    sort (vertices.begin(), vertices.end(), Vertex::compareY);
    // El corte de v2 con la linea v1,v3
    v4 = vertices[1];

    //Relleno tipo a
    if(v4.getX() > vertices[1].getX()){
        //rellenoA(v1,v2,v4)
        //rellenoB(v2, v4, v3)
    }else{
        //rellenoA(v1,v4,v2)
        //rellenoB(v4, v2, v3)
    }
}

//--------------------------------------------------------------
void Renderer::rDrawBitmapString( string tex, const Vertex& vertex){
    ofDrawBitmapString(tex, vertex.getX() + center.getX(), vertex.getY() + center.getY());
}

//--------------------------------------------------------------
void Renderer::rRect( const Vertex& vertex, const float w, const float h){
    ofRect(vertex.getX() + center.getX(), vertex.getY() + center.getY(), w, h);
}
