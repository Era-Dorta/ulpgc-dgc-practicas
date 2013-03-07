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
Vertex Renderer::applyPerspective(const Vertex& vertex) const{
        Vertex res = vertex;
        if(perspective_){
            res.setX(vertex.getX() /( 1 - vertex.getZ() * invK));
            res.setY(vertex.getY() /( 1 - vertex.getZ() * invK));
        }
        return res;
}

//--------------------------------------------------------------
void Renderer::rPixel(const float x, const float y, const float z) const{
    //Give some offset, arbitrary x is chosen
    //Otherwise the line would not be drawn
    if(useZBuffer){
        //Plus 0.5 to ensure correct conversion
        int x_ = (int)(x + center.getX() + 0.5);
        int y_ = (int)(y + center.getY() + 0.5);
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
void Renderer::rLine(const Vertex& vertex0, const Vertex& vertex1) const{
    rLine(vertex0.getX(), vertex0.getY(), vertex0.getZ(), vertex1.getX(), vertex1.getY(), vertex1.getZ());
}

//--------------------------------------------------------------
void Renderer::rLine(const float x0, const float y0, const float x1, const float y1) const{
    ofLine(x0 + center.getX(),y0 + center.getY(),x1 + center.getX(), y1 + center.getY());
}

//--------------------------------------------------------------
void Renderer::rLine(const float x0, const float y0, const float z0, const float x1, const float y1, const float z1) const{
    if(perspective_){
        rLine(x0 /( 1 - z0 * invK), y0 /( 1 - z0 * invK),x1 /( 1 - z1 * invK),y1 /( 1 - z1 * invK));
    }else{
        rLine(x0, y0, x1, y1);
    }
}

//--------------------------------------------------------------
void Renderer::rTriangle(const Vertex& vertex0, const Vertex& vertex1, const Vertex& vertex2) const{
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
void Renderer::triangleFillBotFlat(const Vertex& vertex0, const Vertex& vertex1, const Vertex& vertex2) const{
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

    inv_z01 = vertex1.getY() - vertex0.getY();
    if(inv_z01){
        inv_z01 = (vertex1.getZ() - vertex0.getZ())/inv_z01;
    }

    inv_z02 = vertex2.getY() - vertex0.getY();
    if(inv_z02){
        inv_z02 = (vertex2.getZ() - vertex0.getZ())/inv_z02;
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
void Renderer::triangleFillTopFlat(const Vertex& vertex0, const Vertex& vertex1, const Vertex& vertex2) const{
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

    inv_z20 = vertex0.getY() - vertex2.getY();
    if(inv_z20){
        inv_z20 = (vertex0.getZ() - vertex2.getZ())/inv_z20;
    }

    inv_z21 = vertex1.getY() - vertex2.getY();
    if(inv_z21){
        inv_z21 = (vertex1.getZ() - vertex2.getZ())/inv_z21;
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
void Renderer::rTriangleFill(const Vertex& vertex0, const Vertex& vertex1, const Vertex& vertex2) const{
    vector<Vertex> vertices;
    vertices.reserve(3);
    vertices.push_back(applyPerspective(vertex0));
    vertices.push_back(applyPerspective(vertex1));
    vertices.push_back(applyPerspective(vertex2));


    sort (vertices.begin(), vertices.end(), Vertex::compareYX);
// Vertex v0 = vertices[0],v1 = vertices[1],v2= vertices[2];
    //The triangle is already Top Flat or Bottom Flat
    if(vertices[0].getY() == vertices[1].getY()){
        triangleFillTopFlat(vertices[0], vertices[1], vertices[2]);
        return;
    }else{
        if(vertices[1].getY() == vertices[2].getY()){
            triangleFillBotFlat(vertices[0], vertices[1], vertices[2]);
            return;
        }
    }

    Vertex v3;
    v3 = vertices[1];
    //Interpolate where vertex1.x cuts the line vertex0-vertex2
    v3.setX( vertices[2].getX()*( (vertices[0].getY() - vertices[1].getY()) / (vertices[0].getY() - vertices[2].getY()) ) +
        vertices[0].getX()*( 1 - (vertices[0].getY() - vertices[1].getY()) / (vertices[0].getY() - vertices[2].getY())) );
    //Interpolate where vertex1.z cuts the line vertex0-vertex2
    v3.setZ( vertices[2].getZ()*( (vertices[0].getY() - vertices[1].getY()) / (vertices[0].getY() - vertices[2].getY()) ) +
        vertices[0].getZ()*( 1 - (vertices[0].getY() - vertices[1].getY()) / (vertices[0].getY() - vertices[2].getY())) );
    if(v3.getX() > vertices[1].getX()){
        //Triangle is d shape
        triangleFillBotFlat(vertices[0], vertices[1], v3);
        triangleFillTopFlat(vertices[1], v3, vertices[2]);
    }else{
        //Triangle is b shape
        triangleFillBotFlat(vertices[0], v3, vertices[1]);
        triangleFillTopFlat(v3, vertices[1], vertices[2]);
    }
}

//--------------------------------------------------------------
void Renderer::rDrawBitmapString( const string tex, const Vertex& vertex) const{
    ofDrawBitmapString(tex, vertex.getX() + center.getX(), vertex.getY() + center.getY());
}

//--------------------------------------------------------------
void Renderer::rRect( const Vertex& vertex, const float w, const float h) const{
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
