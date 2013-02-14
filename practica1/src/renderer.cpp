#include "renderer.hpp"
#include "ofMain.h"

int k = 400;
//--------------------------------------------------------------
void Renderer::perspective( const bool activate ){
    perspective_ = activate;
}

//--------------------------------------------------------------
void Renderer::rLine(const Vertex& vertex0, const Vertex& vertex1){
    double x0 = vertex0.getX();
    double y0 = vertex0.getY();
    double z0 = vertex0.getZ();
    double x1 = vertex1.getX();
    double y1 = vertex1.getY();
    double z1 = vertex1.getZ();

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
void Renderer::rDrawBitmapString( string tex, const Vertex& vertex){
    ofDrawBitmapString(tex, vertex.getX() + center.getX(), vertex.getY() + center.getY());
}

//--------------------------------------------------------------
void Renderer::rRect( const Vertex& vertex, const float w, const float h){
    ofRect(vertex.getX() + center.getX(), vertex.getY() + center.getY(), w, h);
}
