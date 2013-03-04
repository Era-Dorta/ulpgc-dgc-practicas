#include "drawableObject.hpp"
#include <cstdlib>
#include <algorithm>    // std::swap

//--------------------------------------------------------------
void DrawableObject::multiplyMatrix( float matrix0[4][4], float matrix1[4][4], int firstSave ){

    float aux[4][4];

    //Calcula matrix multiplication
    for( int i = 0; i < 4; i++){
        for( int j = 0; j < 4; j++){
            aux[i][j] = 0;
            for( int k = 0; k < 4; k++){
                aux[i][j] += matrix0[i][k]*matrix1[k][j];
            }
        }
    }

    //Save result in matrix0 or matrix1
    if(firstSave){
        for( int i = 0; i < 4; i++){
            for( int j = 0; j < 4; j++){
                matrix0[i][j] = aux[i][j];
            }
        }
    }else{
        for( int i = 0; i < 4; i++){
            for( int j = 0; j < 4; j++){
                matrix1[i][j] = aux[i][j];
            }
        }
    }
}

//--------------------------------------------------------------
void DrawableObject:: applyTransform( const bool permanent ){
    if(permanent){
        //Multiply transMatrix by auxMatrix and save the
        //result in transMAtrix, recalculate the transformed
        //vertices
        multiplyMatrix(transMatrix, auxMatrix);
        for( int i = 0; i < totalVertices; i++){
            transVertices[i] = vertices[i]*transMatrix;
        }
        for( int i = 0; i < totalTriangles; i++){
            transNormals[i] = normals[i]*transMatrix;
            transTriangleCentroids[i] = triangleCentroids[i]*transMatrix;
        }
    }else{
        //Multiply transMatrix by auxMatrix and save the
        //result in auxMatrix, recalculate the transformed
        //vertices
        multiplyMatrix(transMatrix, auxMatrix, 0);
        for( int i = 0; i < totalVertices; i++){
            transVertices[i] = vertices[i]*auxMatrix;
        }
        for( int i = 0; i < totalTriangles; i++){
            transNormals[i] = normals[i]*auxMatrix;
            transTriangleCentroids[i] = triangleCentroids[i]*auxMatrix;
        }
    }
}

//--------------------------------------------------------------
void DrawableObject::calculateNormals(){
    Vertex d1, d2;

    if(normals == NULL){
        normals = new Vertex[totalTriangles];
        transNormals = new Vertex[totalTriangles];
    }

    for(int i = 0; i < totalTriangles; i++){
        d1 = vertices[triangles[i][1]] - vertices[triangles[i][0]];
        d2 = vertices[triangles[i][2]] - vertices[triangles[i][1]];
        normals[i] = d1*d2;
        normals[i].normalize();
        transNormals[i] = normals[i];
    }
}

//--------------------------------------------------------------
void DrawableObject::calculateCentroids(){
    Vertex aux;

    if(triangleCentroids == NULL){
        triangleCentroids = new Vertex[totalTriangles];
        transTriangleCentroids = new Vertex[totalTriangles];
    }

    for(int i = 0; i < totalTriangles; i++){
        aux.set(0,0,0);
        for(int j = 0; j < 3; j++){
            aux = aux + vertices[triangles[i][j]];
        }
        triangleCentroids[i] = aux/3.0;
        transTriangleCentroids[i] = triangleCentroids[i];
    }

}

//--------------------------------------------------------------
DrawableObject::DrawableObject( const int totalVertices_, const ofColor color_  ){

    totalVertices = totalVertices_;
    if( totalVertices ){
        vertices = new Vertex[totalVertices];
        transVertices = new Vertex[totalVertices];
        triangles = NULL;
        normals = NULL;
        triangleCentroids = NULL;
        transNormals = NULL;
        transTriangleCentroids = NULL;
        totalTriangles = 0;
    }else{
        vertices =  NULL;
        transVertices = NULL;
        triangles = NULL;
        normals = NULL;
        triangleCentroids = NULL;
        transNormals = NULL;
        transTriangleCentroids = NULL;
        totalTriangles = 0;
    }
    drawTriangles_ = false;
    drawNormals_ = false;
    drawFillTriangles_ = false;
    color = color_;
}

//--------------------------------------------------------------
DrawableObject::DrawableObject( const DrawableObject& otherDrawableObject ){
    //Copy simple attributes
    totalVertices = otherDrawableObject.totalVertices;
    subtype = otherDrawableObject.subtype;
    totalTriangles = otherDrawableObject.totalTriangles;
    drawTriangles_ = otherDrawableObject.drawTriangles_;
    drawNormals_ = otherDrawableObject.drawNormals_;
    drawFillTriangles_ = otherDrawableObject.drawFillTriangles_;
    //Get memory for the vertices
    vertices = new Vertex[totalVertices];
    transVertices = new Vertex[totalVertices];
    //Copy vertices content
    for( int i = 0; i < totalVertices; i++ ){
        vertices[i] = otherDrawableObject.vertices[i];
        transVertices[i] = otherDrawableObject.transVertices[i];
    }
    //Copy matrices content
    for( int i = 0; i < 4; i++ ){
        for( int j = 0; i < 4; i++ ){
            transMatrix[i][j] = otherDrawableObject.transMatrix[i][j];
            auxMatrix[i][j] = otherDrawableObject.auxMatrix[i][j];
        }
    }
    if(otherDrawableObject.triangles){
        triangles = new int*[otherDrawableObject.totalTriangles];
        normals = new Vertex[otherDrawableObject.totalTriangles];
        triangleCentroids = new Vertex[otherDrawableObject.totalTriangles];
        transNormals = new Vertex[otherDrawableObject.totalTriangles];
        transTriangleCentroids = new Vertex[otherDrawableObject.totalTriangles];
        //Copy triangles indices
        for( int i = 0; i < totalTriangles; i++ ){
            triangles[i] = new int[3];
            normals[i] = otherDrawableObject.normals[i];
            transNormals[i] = otherDrawableObject.transNormals[i];
            triangleCentroids[i] = otherDrawableObject.triangleCentroids[i];
            transTriangleCentroids[i] = otherDrawableObject.transTriangleCentroids[i];
            for( int j = 0; j < 3; j++ ){
                triangles[i][j] = otherDrawableObject.triangles[i][j];
            }
        }
    }else{
        triangles = NULL;
        normals = NULL;
        triangleCentroids = NULL;
        transNormals = NULL;
        transTriangleCentroids = NULL;
    }
    color = otherDrawableObject.color;
}

//--------------------------------------------------------------
DrawableObject::~DrawableObject(){
    delete[] vertices;
    delete[] transVertices;
    for( int i = 0; i < totalTriangles; i++ ){
        delete[] triangles[i];
    }
    delete[] triangles;
    delete[] normals;
    delete[] transNormals;
    delete[] triangleCentroids;
    delete[] transTriangleCentroids;
}

//--------------------------------------------------------------
void DrawableObject::draw(const Renderer* renderer) const{

    if(drawFillTriangles_){
        for( int i = 0; i < totalTriangles; i++ ){
            renderer->rTriangleFill(transVertices[triangles[i][0]], transVertices[triangles[i][1]], transVertices[triangles[i][2]]);
        }
    }else{
        if(drawNormals_){
            for( int i = 0; i < totalTriangles; i++ ){
                //Draw triangle
                renderer->rTriangle(transVertices[triangles[i][0]], transVertices[triangles[i][1]], transVertices[triangles[i][2]]);
                //Draw triangle's normal
                renderer->rLine(transTriangleCentroids[i], transTriangleCentroids[i] + transNormals[i]*10);
            }
        }else{
            if(drawTriangles_){
                for( int i = 0; i < totalTriangles; i++ ){
                    //Draw triangle
                    renderer->rTriangle(transVertices[triangles[i][0]], transVertices[triangles[i][1]], transVertices[triangles[i][2]]);
                }
            }
        }
    }

}

//--------------------------------------------------------------
void DrawableObject::rotate( const Axis axis, const float amount, const bool permanent){
    float cosVal, sinVal;
    resetAuxMatrix();
    cosVal = cos(0.02*amount);
    sinVal = sin(0.02*amount);
    switch(axis){
    case X:
        auxMatrix[1][1] = cosVal;
        auxMatrix[1][2] = sinVal;
        auxMatrix[2][1] = -sinVal;
        auxMatrix[2][2] = cosVal;
        break;
    case Y:
        auxMatrix[0][0] = cosVal;
        auxMatrix[0][2] = -sinVal;
        auxMatrix[2][0] = sinVal;
        auxMatrix[2][2] = cosVal;
        break;
    case Z:
        auxMatrix[0][0] = cosVal;
        auxMatrix[0][1] = sinVal;
        auxMatrix[1][0] = -sinVal;
        auxMatrix[1][1] = cosVal;
        break;
    }

    applyTransform(permanent);
}

//--------------------------------------------------------------
void DrawableObject::translate( const float tX, const float tY, const float tZ, const bool permanent){
    resetAuxMatrix();
    auxMatrix[3][0] = tX;
    auxMatrix[3][1] = tY;
    auxMatrix[3][2] = tZ;

    applyTransform(permanent);
}

//--------------------------------------------------------------
void DrawableObject::resetTransMatrix(){
    //Set transformation matrix to its initial state
    for( int i = 0; i < 4; i++){
        for( int j = 0; j < 4; j++){
            if(i == j){
                transMatrix[i][j] = 1;
            }else{
                transMatrix[i][j] = 0;
            }
        }
    }
}

//--------------------------------------------------------------
void DrawableObject::resetAuxMatrix(){
    //Set transformation matrix to its initial state
    for( int i = 0; i < 4; i++){
        for( int j = 0; j < 4; j++){
            if(i == j){
                auxMatrix[i][j] = 1;
            }else{
                auxMatrix[i][j] = 0;
            }
        }
    }
}

//--------------------------------------------------------------
void DrawableObject::setNormals( const bool activate ){
    if(activate){
        drawTriangles_ = true;
    }
    drawNormals_ = activate;
}

//--------------------------------------------------------------
void DrawableObject::setFillTriangles( const bool activate ){
    if(activate){
        drawTriangles_ = true;
    }
    drawFillTriangles_ = activate;
}
