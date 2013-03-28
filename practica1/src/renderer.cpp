#include "renderer.hpp"
#include "ofMain.h"
#include "lightSource.hpp"
#include <algorithm>    // std::sort
#include <vector>       // std::vector
#include <cfloat>       // std::FLT_MAX
#include <climits>      //std::INT_MIN

#define R 0
#define G 1
#define B 2

using namespace std;

const int k = 400;
const float invK = 1.0/k;
const float kD = 100;
const float kA = 0.2;
const float kS = 10;
const Vertex observer(0,0,k);
const float n = 2;
const ofColor lightColor = ofColor::white;

//--------------------------------------------------------------
Renderer::Renderer(){
    zBuffer = NULL;
    perspective_ = false;
    w = 0;
    h = 0;
    useZBuffer = false;
    lightingMode = NONE;
    lightSources = NULL;
    nLightSources = 0;
}

//--------------------------------------------------------------
Renderer::Renderer(const int w_, const int h_ ){
    perspective_ = false;
    w = w_;
    h = h_;
    currentColor = ofColor::white;
    lightingMode = NONE;
    zBuffer = new float*[w];
    for(int i = 0; i < w; i++){
        zBuffer[i] = new float[h];
        for(int j = 0; j < h; j++){
            zBuffer[i][j] = INT_MIN;
        }
    }
    lightSources = NULL;
    nLightSources = 0;
}

//--------------------------------------------------------------
Renderer::~Renderer(){
    for(int i = 0; i < w; i++){
        delete[] zBuffer[i];

    }
    delete[] zBuffer;
    delete[] lightSources;
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

//--------------------------------------------------------------
void Renderer::range( float& val, const float& min, const float&  max) const{
    if( val < min){
        val = min;
        return;
    }
    if( val > max){
        val = max;
        return;
    }
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
    float inv_m01, inv_m02, x_i, x_f, inv_z01, inv_z02, z_i, z_f, z_p, inv_mzp, z_max, z_min, x_max, x_min;

    z_max = vertex0.getZ();
    z_min = vertex1.getZ();
    x_max = vertex2.getX();
    x_min = vertex1.getX();

    if(x_max < vertex0.getX()){
        x_max = vertex0.getX();
    }

    if(x_min > vertex0.getX()){
        x_min = vertex0.getX();
    }

    if(z_max < vertex1.getZ()){
        z_min = vertex0.getZ();
        z_max = vertex1.getZ();
        if(z_max < vertex2.getZ()){
            z_max = vertex2.getZ();
        }
    }

    if(z_min > vertex2.getZ()){
        z_min = vertex2.getZ();
    }

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

    for(int j = vertex0.getY() - 0.5; j <= vertex1.getY() + 0.5; j++){
        for(int i = x_i - 0.5; i <= x_f + 0.5; i++){
            range(z_p, z_min, z_max);
            rPixel(i, j, z_p);
            z_p += inv_mzp;
        }

        x_i += inv_m01;

        if(x_i < x_min){
            x_i = x_min;
        }

        x_f += inv_m02;

        if(x_f > x_max){
            x_f = x_max;
        }

        z_i += inv_z01;
        range(z_i, z_min, z_max);

        z_f += inv_z02;
        range(z_f, z_min, z_max);

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
    float inv_m20, inv_m21, x_i, x_f, inv_z20, inv_z21, z_i, z_f, z_p, inv_mzp, z_max, z_min, x_max, x_min;

    z_max = vertex0.getZ();
    z_min = vertex1.getZ();
    x_max = vertex1.getX();
    x_min = vertex0.getX();

    if(x_max < vertex2.getX()){
        x_max = vertex2.getX();
    }

    if(x_min > vertex2.getX()){
        x_min = vertex2.getX();
    }

    if(z_max < vertex1.getZ()){
        z_min = vertex0.getZ();
        z_max = vertex1.getZ();

    }

    if(z_max < vertex2.getZ()){
        z_max = vertex2.getZ();
    }

    if(z_min > vertex2.getZ()){
        z_min = vertex2.getZ();
    }

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

    for(int j = vertex2.getY() + 0.5; j >= vertex0.getY() - 0.5; j--){
        for(int i = x_i - 0.5; i <= x_f + 0.5; i++){
            range(z_p, z_min, z_max);
            rPixel(i, j, z_p);
            z_p += inv_mzp;
        }

        x_i -= inv_m20;

        if(x_i < x_min){
            x_i = x_min;
        }

        x_f -= inv_m21;

        if(x_f > x_max){
            x_f = x_max;
        }

        z_i -= inv_z20;
        range(z_i, z_min, z_max);

        z_f -= inv_z21;
        range(z_f, z_min, z_max);

        inv_mzp = (z_f - z_i)/(x_f - x_i);
        z_p = z_i;
    }
}

//--------------------------------------------------------------
void Renderer::triangleFillBotFlat(const Vertex& vertex0, const Vertex& vertex1, const Vertex& vertex2,
    const Vertex&normal, const Vertex& centroid ) const{
    if( vertex0.getY() == vertex1.getY() || vertex1.getX() == vertex2.getX() ){
        return;
    }
    float inv_m01, inv_m02, x_i, x_f, inv_z01, inv_z02, z_i, z_f, z_p, inv_mzp, z_max, z_min, x_max, x_min;
    Vertex lightVector, h, s;
    float cosNL, cosNH, distance = 0;
    float auxR = 0, auxG = 0, auxB = 0;

    for(int i = 0; i < nLightSources;i++){
        lightVector = lightSources[i]->getLightPosition() - centroid;
        lightVector.normalize();
        s = centroid - observer;
        s.normalize();
        h = (lightVector + s ) * 0.5;

        cosNL = normal.dot(lightVector);
        range(cosNL, 0, 1);
        cosNL = kD*cosNL;

        cosNH = normal.dot(h);
        range(cosNH, 0, 1);
        cosNH = kS*pow(cosNH, n);

        distance = 1.0/(lightSources[i]->getLightPosition().distance(centroid) + 0.5);

        auxR += distance*(cosNL*currentColor.r + cosNH*lightColor.r);
        auxG += distance*(cosNL*currentColor.g + cosNH*lightColor.g);
        auxB += distance*(cosNL*currentColor.b + cosNH*lightColor.b);
    }

    auxR += currentColor.r*kA;
    auxG += currentColor.g*kA;
    auxB += currentColor.b*kA;

    range(auxR, 0, 255);
    range(auxG, 0, 255);
    range(auxB, 0, 255);

    ofSetColor(auxR, auxG, auxB);

    z_max = vertex0.getZ();
    z_min = vertex1.getZ();
    x_max = vertex2.getX();
    x_min = vertex1.getX();

    if(x_max < vertex0.getX()){
        x_max = vertex0.getX();
    }

    if(x_min > vertex0.getX()){
        x_min = vertex0.getX();
    }

    if(z_max < vertex1.getZ()){
        z_min = vertex0.getZ();
        z_max = vertex1.getZ();
        if(z_max < vertex2.getZ()){
            z_max = vertex2.getZ();
        }
    }

    if(z_min > vertex2.getZ()){
        z_min = vertex2.getZ();
    }

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



    for(int j = vertex0.getY() - 0.5; j <= vertex1.getY() + 0.5; j++){
        for(int i = x_i - 0.5; i <= x_f + 0.5; i++){
            range(z_p, z_min, z_max);
            rPixel(i, j, z_p);
            z_p += inv_mzp;
        }

        x_i += inv_m01;

        if(x_i < x_min){
            x_i = x_min;
        }

        x_f += inv_m02;

        if(x_f > x_max){
            x_f = x_max;
        }

        z_i += inv_z01;
        range(z_i, z_min, z_max);

        z_f += inv_z02;
        range(z_f, z_min, z_max);

        inv_mzp = (z_f - z_i)/(x_f - x_i);
        z_p = z_i;
    }
}

//--------------------------------------------------------------
void Renderer::triangleFillTopFlat(const Vertex& vertex0, const Vertex& vertex1, const Vertex& vertex2,
    const Vertex&normal, const Vertex& centroid) const{
    if( vertex0.getY() == vertex2.getY() || vertex0.getX() == vertex1.getX() ){
        return;
    }
    float inv_m20, inv_m21, x_i, x_f, inv_z20, inv_z21, z_i, z_f, z_p, inv_mzp, z_max, z_min, x_max, x_min;
    Vertex lightVector, h, s;
    float cosNL, cosNH, distance = 0;
    float auxR = 0, auxG = 0, auxB = 0;

    for(int i = 0; i < nLightSources;i++){
        lightVector = lightSources[i]->getLightPosition() - centroid;
        lightVector.normalize();
        s = centroid - observer;
        s.normalize();
        h = (lightVector + s ) * 0.5;

        cosNL = normal.dot(lightVector);
        range(cosNL, 0, 1);
        cosNL = kD*cosNL;

        cosNH = normal.dot(h);
        range(cosNH, 0, 1);
        cosNH = kS*pow(cosNH, n);

        distance = 1.0/(lightSources[i]->getLightPosition().distance(centroid) + 0.5);

        auxR += distance*(cosNL*currentColor.r + cosNH*lightColor.r);
        auxG += distance*(cosNL*currentColor.g + cosNH*lightColor.g);
        auxB += distance*(cosNL*currentColor.b + cosNH*lightColor.b);
    }

    auxR += currentColor.r*kA;
    auxG += currentColor.g*kA;
    auxB += currentColor.b*kA;

    range(auxR, 0, 255);
    range(auxG, 0, 255);
    range(auxB, 0, 255);

    ofSetColor(auxR, auxG, auxB);

    z_max = vertex0.getZ();
    z_min = vertex1.getZ();
    x_max = vertex1.getX();
    x_min = vertex0.getX();

    if(x_max < vertex2.getX()){
        x_max = vertex2.getX();
    }

    if(x_min > vertex2.getX()){
        x_min = vertex2.getX();
    }

    if(z_max < vertex1.getZ()){
        z_min = vertex0.getZ();
        z_max = vertex1.getZ();

    }

    if(z_max < vertex2.getZ()){
        z_max = vertex2.getZ();
    }

    if(z_min > vertex2.getZ()){
        z_min = vertex2.getZ();
    }

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

    for(int j = vertex2.getY() + 0.5; j >= vertex0.getY() - 0.5; j--){
        for(int i = x_i - 0.5; i <= x_f + 0.5; i++){
            range(z_p, z_min, z_max);
            rPixel(i, j, z_p);
            z_p += inv_mzp;
        }

        x_i -= inv_m20;

        if(x_i < x_min){
            x_i = x_min;
        }

        x_f -= inv_m21;

        if(x_f > x_max){
            x_f = x_max;
        }

        z_i -= inv_z20;
        range(z_i, z_min, z_max);

        z_f -= inv_z21;
        range(z_f, z_min, z_max);

        inv_mzp = (z_f - z_i)/(x_f - x_i);
        z_p = z_i;
    }
}

/*
   v0
   /\
  /__\
v1    v2
*/
//--------------------------------------------------------------
void Renderer::triangleFillBotFlatGouraud(const Vertex& vertex0,const Vertex& normal0,
            const Vertex& vertex1, const Vertex& normal1,
            const Vertex& vertex2, const Vertex& normal2 ) const{
    if( vertex0.getY() == vertex1.getY() || vertex1.getX() == vertex2.getX() ){
        return;
    }
    float inv_m01, inv_m02, x_i, x_f, inv_z01, inv_z02, z_i, z_f, z_p, inv_mzp, z_max, z_min, x_max, x_min;
    Vertex lightVector, h, s;
    float cosNL, cosNH, distance = 0;
    //Rows belong to vertices and columns to RGB
    float auxColor[3][3];
    float iI[3], iF[3], iP[3], invColor01[3], invColor02[3], invColorP[3];
    const Vertex* vertices[3] = {&vertex0, &vertex1, &vertex2},* normals[3] = {&normal0, &normal1, &normal2};

    for(int i = 0; i < 3; i++){
        for(int j = R; j <= B; j++){
            auxColor[i][j] = 0;
        }
    }

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < nLightSources;j++){
            lightVector = lightSources[j]->getLightPosition() - (*vertices[i]);
            lightVector.normalize();
            s = (*vertices[i]) - observer;
            s.normalize();
            h = (lightVector + s ) * 0.5;

            cosNL = (*normals[i]).dot(lightVector);
            range(cosNL, 0, 1);
            cosNL = kD*cosNL;

            cosNH = (*normals[i]).dot(h);
            range(cosNH, 0, 1);
            cosNH = kS*pow(cosNH, n);

            distance = 1.0/(lightSources[j]->getLightPosition().distance(*vertices[i]) + 0.5);

            for(int k = R; k <= B; k++){
                auxColor[i][k] += distance*(cosNL*currentColor[k] + cosNH*lightColor[k]);
            }
        }

        for(int k = R; k <= B; k++){
            auxColor[i][k] += currentColor[k]*kA;
            range(auxColor[i][k], 0, 255);
        }
    }

    z_max = vertex0.getZ();
    z_min = vertex1.getZ();
    x_max = vertex2.getX();
    x_min = vertex1.getX();

    if(x_max < vertex0.getX()){
        x_max = vertex0.getX();
    }

    if(x_min > vertex0.getX()){
        x_min = vertex0.getX();
    }

    if(z_max < vertex1.getZ()){
        z_min = vertex0.getZ();
        z_max = vertex1.getZ();
        if(z_max < vertex2.getZ()){
            z_max = vertex2.getZ();
        }
    }

    if(z_min > vertex2.getZ()){
        z_min = vertex2.getZ();
    }

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

    for(int i = R; i <= B; i++){
        invColor01[i] = vertex1.getY() - vertex0.getY();
        if(invColor01[i]){
            invColor01[i] = (auxColor[1][i] - auxColor[0][i])/invColor01[i];
        }

        invColor02[i] = vertex2.getY() - vertex0.getY();
        if(invColor02[i]){
            invColor02[i] = (auxColor[2][i] - auxColor[0][i])/invColor02[i];
        }
    }

    x_i = vertex0.getX();
    x_f = vertex0.getX();
    z_i = vertex0.getZ();
    z_f = vertex0.getZ();
    inv_mzp = 0;
    z_p = z_i;

    for(int i = R; i <= B; i++){
        iI[i] = auxColor[0][i];
        iF[i] = auxColor[0][i];
        iP[i] = auxColor[0][i];
        invColorP[i] = 0;
    }

    for(int j = vertex0.getY() - 0.5; j <= vertex1.getY() + 0.5; j++){
        for(int i = x_i - 0.5; i <= x_f + 0.5; i++){
            range(z_p, z_min, z_max);

            ofSetColor(iP[R], iP[G], iP[B]);

            rPixel(i, j, z_p);
            z_p += inv_mzp;

            for(int i = R; i <= B; i++){
                iP[i] += invColorP[i];
                range(iP[i], 0, 255);
            }
        }

        x_i += inv_m01;

        if(x_i < x_min){
            x_i = x_min;
        }

        x_f += inv_m02;

        if(x_f > x_max){
            x_f = x_max;
        }

        z_i += inv_z01;
        range(z_i, z_min, z_max);

        z_f += inv_z02;
        range(z_f, z_min, z_max);

        inv_mzp = (z_f - z_i)/(x_f - x_i);
        z_p = z_i;

        for(int i = R; i <= B; i++){
            iI[i] += invColor01[i];
            iF[i] += invColor02[i];
            iP[i] = iI[i];
        }

        for(int i = R; i <= B; i++){
            invColorP[i] = (iF[i] - iI[i])/(x_f - x_i);
        }

    }
}

// v0___v1
//   \ /
//    \/
//     v2
//--------------------------------------------------------------
void Renderer::triangleFillTotFlatGouraud(const Vertex& vertex0,const Vertex& normal0,
            const Vertex& vertex1, const Vertex& normal1,
            const Vertex& vertex2, const Vertex& normal2 ) const{
    if( vertex0.getY() == vertex2.getY() || vertex0.getX() == vertex1.getX() ){
        return;
    }
    float inv_m20, inv_m21, x_i, x_f, inv_z20, inv_z21, z_i, z_f, z_p, inv_mzp, z_max, z_min, x_max, x_min;
    Vertex lightVector, h, s;
    float cosNL, cosNH, distance = 0;
    //Rows belong to vertices and columns to RGB
    float auxColor[3][3];
    float iI[3], iF[3], iP[3], invColor20[3], invColor21[3], invColorP[3], colorMax[3], colorMin[3];
    const Vertex* vertices[3] = {&vertex0, &vertex1, &vertex2},* normals[3] = {&normal0, &normal1, &normal2};

    for(int i = 0; i < 3; i++){
        for(int j = R; j <= B; j++){
            auxColor[i][j] = 0;
        }
    }

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < nLightSources;j++){
            lightVector = lightSources[j]->getLightPosition() - (*vertices[i]);
            lightVector.normalize();
            s = (*vertices[i]) - observer;
            s.normalize();
            h = (lightVector + s ) * 0.5;

            cosNL = (*normals[i]).dot(lightVector);
            range(cosNL, 0, 1);
            cosNL = kD*cosNL;

            cosNH = (*normals[i]).dot(h);
            range(cosNH, 0, 1);
            cosNH = kS*pow(cosNH, n);

            distance = 1.0/(lightSources[j]->getLightPosition().distance(*vertices[i]) + 0.5);

            for(int k = R; k <= B; k++){
                auxColor[i][k] += distance*(cosNL*currentColor[k] + cosNH*lightColor[k]);
            }
        }

        for(int k = R; k <= B; k++){
            auxColor[i][k] += currentColor[k]*kA;
            range(auxColor[i][k], 0, 255);
        }
    }

    z_max = vertex0.getZ();
    z_min = vertex1.getZ();
    x_max = vertex1.getX();
    x_min = vertex0.getX();

    for(int i = R; i <= B; i++){
        colorMax[i] = auxColor[0][i];
        colorMin[i] = auxColor[1][i];
    }

    if(x_max < vertex2.getX()){
        x_max = vertex2.getX();
    }

    if(x_min > vertex2.getX()){
        x_min = vertex2.getX();
    }

    if(z_max < vertex1.getZ()){
        z_min = vertex0.getZ();
        z_max = vertex1.getZ();

    }

    if(z_max < vertex2.getZ()){
        z_max = vertex2.getZ();
    }

    if(z_min > vertex2.getZ()){
        z_min = vertex2.getZ();
    }

    for(int i = R; i <= B; i++){
        if( colorMax[i] < auxColor[1][i]){
            colorMax[i] = auxColor[1][i];
            colorMin[i] = auxColor[0][i];
        }

        if( colorMax[i] < auxColor[2][i]){
            colorMax[i] = auxColor[2][i];
        }

        if( colorMin[i] > auxColor[2][i]){
            colorMin[i] = auxColor[2][i];
        }
    }

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

    for(int i = R; i <= B; i++){
        invColor20[i] = vertex0.getY() - vertex2.getY();
        if(invColor20[i]){
            invColor20[i] = (auxColor[0][i] - auxColor[2][i])/invColor20[i];
        }

        invColor21[i] = vertex1.getY() - vertex2.getY();
        if(invColor21[i]){
            invColor21[i] = (auxColor[1][i] - auxColor[2][i])/invColor21[i];
        }
    }

    x_i = vertex2.getX();
    x_f = vertex2.getX();
    z_i = vertex2.getZ();
    z_f = vertex2.getZ();
    inv_mzp = 0;
    z_p = z_i;

    for(int i = R; i <= B; i++){
        iI[i] = auxColor[0][i];
        iF[i] = auxColor[0][i];
        iP[i] = auxColor[0][i];
        invColorP[i] = 0;
    }

    for(int j = vertex2.getY() + 0.5; j >= vertex0.getY() - 0.5; j--){
        for(int i = x_i - 0.5; i <= x_f + 0.5; i++){
            range(z_p, z_min, z_max);
            ofSetColor(iP[R], iP[G], iP[B]);
            rPixel(i, j, z_p);
            z_p += inv_mzp;

            for(int i = R; i <= B; i++){
                iP[i] += invColorP[i];
                range(iP[i], colorMin[i], colorMax[i]);
            }
        }

        x_i -= inv_m20;

        if(x_i < x_min){
            x_i = x_min;
        }

        x_f -= inv_m21;

        if(x_f > x_max){
            x_f = x_max;
        }

        z_i -= inv_z20;
        range(z_i, z_min, z_max);

        z_f -= inv_z21;
        range(z_f, z_min, z_max);

        inv_mzp = (z_f - z_i)/(x_f - x_i);
        z_p = z_i;

        for(int i = R; i <= B; i++){
            iI[i] -= invColor20[i];
            iF[i] -= invColor21[i];
            iP[i] = iI[i];
        }

        for(int i = R; i <= B; i++){
            invColorP[i] = (iF[i] - iI[i])/(x_f - x_i);
        }
    }
}

typedef std::pair<Vertex,Vertex> VertexNormal;

bool compareVertexNormal ( const VertexNormal& l, const VertexNormal& r){
    return Vertex::compareYX(l.first, r.first);
}

//--------------------------------------------------------------
void Renderer::rTriangleFill(const Vertex& vertex0, const Vertex& normal0, const Vertex& vertex1, const Vertex& normal1,
    const Vertex& vertex2, const Vertex& normal2, const Vertex& triangleNormal, const Vertex& centroid) const{

    VertexNormal aux;
    vector<VertexNormal> vertices;
    vertices.reserve(3);
    aux.first = applyPerspective(vertex0);
    aux.second = normal0;
    vertices.push_back(aux);
    aux.first = applyPerspective(vertex1);
    aux.second = normal1;
    vertices.push_back(aux);
    aux.first = applyPerspective(vertex2);
    aux.second = normal2;
    vertices.push_back(aux);


    sort (vertices.begin(), vertices.end(), compareVertexNormal);
    //The triangle is already Top Flat or Bottom Flat
    if(vertices[0].first.getY() == vertices[1].first.getY()){
        switch(lightingMode){
        case NONE:
            triangleFillTopFlat(vertices[0].first, vertices[1].first, vertices[2].first);
            break;
        case PHONG_REFLECTION:
            triangleFillTopFlat(vertices[0].first, vertices[1].first, vertices[2].first, triangleNormal, centroid);
            break;
        case GOURAUD_SHADING:
            triangleFillTotFlatGouraud(vertices[0].first, vertices[0].second, vertices[1].first, vertices[1].second,
                vertices[2].first, vertices[2].second);
            break;
        case PHONG_SHADING:
            break;
        }
        return;
    }else{
        if(vertices[1].first.getY() == vertices[2].first.getY()){
            switch(lightingMode){
            case NONE:
                triangleFillBotFlat(vertices[0].first, vertices[1].first, vertices[2].first);
                break;
            case PHONG_REFLECTION:
                triangleFillBotFlat(vertices[0].first, vertices[1].first, vertices[2].first, triangleNormal, centroid);
                break;
            case GOURAUD_SHADING:
                triangleFillBotFlatGouraud(vertices[0].first, vertices[0].second, vertices[1].first,
                    vertices[1].second, vertices[2].first, vertices[2].second);
                break;
            case PHONG_SHADING:
                break;
            }
        }
    }

    Vertex v3;
    v3 = vertices[1].first;
    //Interpolate where vertex1.x cuts the line vertex0-vertex2
    v3.setX( vertices[2].first.getX()*( (vertices[0].first.getY() - vertices[1].first.getY()) / (vertices[0].first.getY() - vertices[2].first.getY()) ) +
        vertices[0].first.getX()*( 1 - (vertices[0].first.getY() - vertices[1].first.getY()) / (vertices[0].first.getY() - vertices[2].first.getY())) );
    //Interpolate where vertex1.z cuts the line vertex0-vertex2
    v3.setZ( vertices[2].first.getZ()*( (vertices[0].first.getY() - vertices[1].first.getY()) / (vertices[0].first.getY() - vertices[2].first.getY()) ) +
        vertices[0].first.getZ()*( 1 - (vertices[0].first.getY() - vertices[1].first.getY()) / (vertices[0].first.getY() - vertices[2].first.getY())) );

    Vertex n3;
    if(lightingMode == GOURAUD_SHADING){
        //FIXME Interpolate normal0 and normal2
        n3 = vertices[0].second + vertices[2].second;
        n3.normalize();
    }
    if(v3.getX() > vertices[1].first.getX()){
        //Triangle is d shape
        switch(lightingMode){
        case NONE:
            triangleFillBotFlat(vertices[0].first, vertices[1].first, v3);
            triangleFillTopFlat(vertices[1].first, v3, vertices[2].first);
            break;
        case PHONG_REFLECTION:
            triangleFillBotFlat(vertices[0].first, vertices[1].first, v3, triangleNormal, centroid);
            triangleFillTopFlat(vertices[1].first, v3, vertices[2].first, triangleNormal, centroid);
            break;
        case GOURAUD_SHADING:
            triangleFillBotFlatGouraud(vertices[0].first, vertices[0].second, vertices[1].first, vertices[0].second, v3, n3);
            triangleFillTotFlatGouraud(vertices[1].first, vertices[1].second, v3, n3, vertices[2].first, vertices[2].second);
            break;
        case PHONG_SHADING:
            break;
        }
    }else{
        //Triangle is b shape
        switch(lightingMode){
        case NONE:
            triangleFillBotFlat(vertices[0].first, v3, vertices[1].first);
            triangleFillTopFlat(v3, vertices[1].first, vertices[2].first);
            break;
        case PHONG_REFLECTION:
            triangleFillBotFlat(vertices[0].first, v3, vertices[1].first, triangleNormal, centroid);
            triangleFillTopFlat(v3, vertices[1].first, vertices[2].first, triangleNormal, centroid);
            break;
        case GOURAUD_SHADING:
            triangleFillBotFlatGouraud(vertices[0].first, vertices[0].second, v3, n3, vertices[1].first, vertices[1].second);
            triangleFillTotFlatGouraud(v3, n3, vertices[1].first, vertices[1].second, vertices[2].first, vertices[2].second);
            break;
        case PHONG_SHADING:
            break;
        }
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

//--------------------------------------------------------------
void Renderer::setColor( const ofColor& newColor ){
    currentColor = newColor;
    ofSetColor( currentColor );
}

//--------------------------------------------------------------
void Renderer::addLight( LightSource* const light ){
    LightSource** auxLightSources;
    auxLightSources = new LightSource*[nLightSources + 1];

    //Copy old vertices in allocated memory
    for( int i = 0; i < nLightSources; i++){
        auxLightSources[i] = lightSources[i];
    }

    auxLightSources[nLightSources] = light;
    nLightSources++;

    delete[] lightSources;
    lightSources = auxLightSources;

}

//--------------------------------------------------------------
void Renderer::deleteLight( LightSource* const light ){
    LightSource** auxLightSources;
    auxLightSources = new LightSource*[nLightSources - 1];

    //Copy old vertices in allocated memory
    int j = 0;
    for( int i = 0; i < nLightSources; i++){
        if( lightSources[i] != light){
            auxLightSources[j] = lightSources[i];
            j++;
        }
    }

    nLightSources--;

    delete[] lightSources;
    lightSources = auxLightSources;
}
