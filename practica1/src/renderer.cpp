#include "renderer.hpp"
#include "ofMain.h"
#include <algorithm>    // std::sort
#include <vector>       // std::vector
#include <cfloat>       // std::FLT_MAX
#include <climits>      //std::INT_MIN
using namespace std;

const int k = 400;
const float invK = 1.0/k;

//--------------------------------------------------------------
Renderer::Renderer(const int w_, const int h_ ){
    perspective_ = false;
    w = w_;
    h = h_;
    zBuffer = new float*[w];
    for(int i = 0; i < w; i++){
        zBuffer[i] = new float[h];
        for(int j = 0; j < h; j++){
            zBuffer[i][j] = INT_MIN;
        }
    }
}

//--------------------------------------------------------------
Renderer::~Renderer(){
    for(int i = 0; i < w; i++){
        delete[] zBuffer[i];

    }
    delete[] zBuffer;
}

void Renderer::setup( const int w_, const int h_ ){
    perspective_ = false;
    useZBuffer = false;

    for(int i = 0; i < w; i++){
        delete[] zBuffer[i];

    }
    delete[] zBuffer;

    w = w_;
    h = h_;

    zBuffer = new float*[w];
    for(int i = 0; i < w; i++){
        zBuffer[i] = new float[h];
        for(int j = 0; j < h; j++){
            zBuffer[i][j] = INT_MIN;
        }
    }
}

//--------------------------------------------------------------
void Renderer::perspective( const bool activate ){
    perspective_ = activate;
}

//--------------------------------------------------------------
Vertex Renderer::applyPerspective(const Vertex& vertex){
        Vertex res = vertex;
        if(perspective_){
            res.setX(vertex.getX() /( 1 - vertex.getZ() * invK));
            res.setY(vertex.getY() /( 1 - vertex.getZ() * invK));
        }
        return res;
}

//--------------------------------------------------------------
void Renderer::rPixel(const float x, const float y, const float z){
    //Give some offset, arbitrary x is chosen
    //Otherwise the line would not be drawn
    if(useZBuffer){
        int x_ = (int)x + center.getX();
        int y_ = (int)y + center.getY();
        if(x_ > 1023 || x_ < 0 ||  y_ > 767 || y_ < 0 ){
            //Trying to draw outside the window
            return;
        }
        if(z >= zBuffer[x_][y_]){
            zBuffer[x_][y_] = z;
            rLine(x, y, x + 1, y);
        }
    }else{
        rLine(x, y, x + 1, y);
    }
}

//--------------------------------------------------------------
void Renderer::rLine(const Vertex& vertex0, const Vertex& vertex1){
    float x0 = vertex0.getX();
    float y0 = vertex0.getY();
    float z0 = vertex0.getZ();
    float x1 = vertex1.getX();
    float y1 = vertex1.getY();
    float z1 = vertex1.getZ();

    rLine(x0, y0, z0, x1, y1, z1);
}

//--------------------------------------------------------------
void Renderer::rLine(const float x0, const float y0, const float x1, const float y1){
    ofLine(x0 + center.getX(),y0 + center.getY(),x1 + center.getX(), y1 + center.getY());
}

//--------------------------------------------------------------
void Renderer::rLine(const float x0, const float y0, const float z0, const float x1, const float y1, const float z1){
    if(perspective_){
        rLine(x0 /( 1 - z0 * invK), y0 /( 1 - z0 * invK),x1 /( 1 - z1 * invK),y1 /( 1 - z1 * invK));
    }else{
        rLine(x0, y0, x1, y1);
    }
}

//--------------------------------------------------------------
void Renderer::rTriangle(const Vertex& vertex0, const Vertex& vertex1, const Vertex& vertex2){
    rLine(vertex0, vertex1);
    rLine(vertex0, vertex2);
    rLine(vertex1, vertex2);
}

/*
   v0
   /\
  /__\
v1    v2
*/
//--------------------------------------------------------------
void Renderer::triangleFillBotFlat(const Vertex& vertex0, const Vertex& vertex1, const Vertex& vertex2){
    if( vertex0.getY() == vertex1.getY() || vertex1.getX() == vertex2.getX() ){
        return;
    }
    float inv_m01, inv_m02, x_i, x_f, inv_z01, inv_z02, z_i, z_f, z_p, inv_mzp;

    inv_m01 = vertex1.getY() - vertex0.getY();
    if(inv_m01){
        inv_m01 = (vertex1.getX() - vertex0.getX())/inv_m01;
    }

    inv_m02 = vertex2.getY() - vertex0.getY();
    if(inv_m02){
        inv_m02 = (vertex2.getX() - vertex0.getX())/inv_m02;
    }

    inv_z01 = vertex1.getZ() - vertex0.getZ();
    if(inv_z01){
        inv_z01 = (vertex1.getY() - vertex0.getY())/inv_z01;
    }

    inv_z02 = vertex2.getZ() - vertex0.getZ();
    if(inv_z02){
        inv_z02 = (vertex2.getY() - vertex0.getY())/inv_z02;
    }

    x_i = vertex0.getX();
    x_f = vertex0.getX();
    z_i = vertex0.getZ();
    z_f = vertex0.getZ();
    inv_mzp = 0;
    z_p = z_i;
    for(int j = vertex0.getY(); j <= vertex1.getY(); j++){
        for(int i = x_i; i <= x_f; i++){
            rPixel(i, j, z_p);
            z_p += inv_mzp;
        }
        x_i += inv_m01;
        x_f += inv_m02;
        z_i += inv_z01;
        z_f += inv_z02;
        inv_mzp = (z_f - z_i)/(x_f - x_i);
        z_p = z_i;
    }
}

// v0___v1
//   \ /
//    \/
//     v2
//--------------------------------------------------------------
void Renderer::triangleFillTopFlat(const Vertex& vertex0, const Vertex& vertex1, const Vertex& vertex2){
    if( vertex0.getY() == vertex2.getY() || vertex0.getX() == vertex1.getX() ){
        return;
    }
    float inv_m20, inv_m21, x_i, x_f, inv_z20, inv_z21, z_i, z_f, z_p, inv_mzp;

    inv_m20 = vertex0.getY() - vertex2.getY();
    if(inv_m20){
        inv_m20 = (vertex0.getX() - vertex2.getX())/inv_m20;
    }

    inv_m21 = vertex1.getY() - vertex2.getY();
    if(inv_m21){
        inv_m21 = (vertex1.getX() - vertex2.getX())/inv_m21;
    }

    inv_z20 = vertex0.getZ() - vertex2.getZ();
    if(inv_z20){
        inv_z20 = (vertex0.getY() - vertex2.getY())/inv_z20;
    }

    inv_z21 = vertex1.getZ() - vertex2.getZ();
    if(inv_z21){
        inv_z21 = (vertex1.getY() - vertex2.getY())/inv_z21;
    }

    x_i = vertex2.getX();
    x_f = vertex2.getX();
    z_i = vertex2.getZ();
    z_f = vertex2.getZ();
    inv_mzp = 0;
    z_p = z_i;
    for(int j = vertex2.getY(); j >= vertex0.getY(); j--){
        for(int i = x_i; i <= x_f; i++){
            rPixel(i, j, z_p);
            z_p += inv_mzp;
        }
        x_i -= inv_m20;
        x_f -= inv_m21;
        z_i -= inv_z20;
        z_f -= inv_z21;
        inv_mzp = (z_f - z_i)/(x_f - x_i);
        z_p = z_i;
    }
}

//--------------------------------------------------------------
void Renderer::rTriangleFill(const Vertex& vertex0, const Vertex& vertex1, const Vertex& vertex2){
    vector<Vertex> vertices;
    vertices.reserve(3);
    vertices.push_back(applyPerspective(vertex0));
    vertices.push_back(applyPerspective(vertex1));
    vertices.push_back(applyPerspective(vertex2));

    Vertex v3;
    sort (vertices.begin(), vertices.end(), Vertex::compareYX);
    // El corte de v2 con la linea v1,v3
    v3 = vertices[1];
    float x0 =  vertices[0].getX();
    float x2 =  vertices[2].getX();
    float y0 =  vertices[0].getY();
    float y1 =  vertices[1].getY();
    float y2 =  vertices[2].getY();
    //Interpolate where vertex1.x cuts the line vertex0-vertex2
    v3.setX( x2*((y0-y1)/(y0-y2)) + x0*(1-(y0-y1)/(y0-y2)) );
    if(v3 == vertices[0]){
        triangleFillTopFlat(vertices[0], vertices[1], vertices[2]);
    }else{
        if(v3 == vertices[2]){
            triangleFillBotFlat(vertices[0], vertices[1], vertices[2]);
        }else{
            if(v3.getX() > vertices[1].getX()){
                triangleFillBotFlat(vertices[0], vertices[1], v3);
                triangleFillTopFlat(vertices[1], v3, vertices[2]);
            }else{
                triangleFillBotFlat(vertices[0], v3, vertices[1]);
                triangleFillTopFlat(v3, vertices[1], vertices[2]);
            }
        }
    }
}

//--------------------------------------------------------------
void Renderer::rDrawBitmapString( const string tex, const Vertex& vertex){
    ofDrawBitmapString(tex, vertex.getX() + center.getX(), vertex.getY() + center.getY());
}

//--------------------------------------------------------------
void Renderer::rRect( const Vertex& vertex, const float w, const float h){
    ofRect(vertex.getX() + center.getX(), vertex.getY() + center.getY(), w, h);
}

//--------------------------------------------------------------
void Renderer::resetZBuffer(){

    if(useZBuffer && zBuffer){
        for(int i = 0; i < w; i++){
            for(int j = 0; j < h; j++){
                zBuffer[i][j] = INT_MIN;
            }
        }
    }
}
