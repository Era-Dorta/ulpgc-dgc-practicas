#include "drawableObject.hpp"
#include <cstdlib>

//--------------------------------------------------------------
DrawableObject::DrawableObject( int totalVertices_, ofColor color_  ){
    totalVertices = totalVertices_;
    if( totalVertices ){
        vertices = new Vertex[totalVertices];
        transVertices = new Vertex[totalVertices];
        triangles = NULL;
        normals = NULL;
        triangleCentroids = NULL;
        totalTriangles = 0;
    }else{
        vertices =  NULL;
        transVertices = NULL;
        triangles = NULL;
        normals = NULL;
        triangleCentroids = NULL;
        totalTriangles = 0;
    }
    drawTriangles_ = false;
    color = color_;
}

//--------------------------------------------------------------
DrawableObject::DrawableObject( const DrawableObject& otherDrawableObject ){
    //Copy how much vertices the object has
    totalVertices = otherDrawableObject.totalVertices;
    subtype = otherDrawableObject.subtype;
    totalTriangles = otherDrawableObject.totalTriangles;
    drawTriangles_ = otherDrawableObject.drawTriangles_;
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
        //Copy triangles indices
        for( int i = 0; i < totalTriangles; i++ ){
            triangles[i] = new int[3];
            normals[i] = otherDrawableObject.normals[i];
            triangleCentroids[i] = otherDrawableObject.triangleCentroids[i];
            for( int j = 0; j < 3; j++ ){
                triangles[i][j] = otherDrawableObject.triangles[i][j];
            }
        }
    }else{
        triangles = NULL;
        normals = NULL;
        triangleCentroids = NULL;
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
    delete[] triangleCentroids;
}

//--------------------------------------------------------------
void DrawableObject::rotate( Axis axis, double amount, int permanent){
    double cosVal, sinVal;
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

    if(permanent){
        //Multiply transMatrix by auxMatrix and save the
        //result in transMAtrix, recalculate the transformed
        //vertices
        multiplyMatrix(transMatrix, auxMatrix);
        for( int i = 0; i < totalVertices; i++){
            transVertices[i] = vertices[i]*transMatrix;
        }
    }else{
        //Multiply transMatrix by auxMatrix and save the
        //result in auxMatrix, recalculate the transformed
        //vertices
        DrawableObject::multiplyMatrix(transMatrix, auxMatrix, 0);
        for( int i = 0; i < totalVertices; i++){
            transVertices[i] = vertices[i]*auxMatrix;
        }
    }
}

//--------------------------------------------------------------
void DrawableObject::translate( double tX, double tY, int permanent){
    resetAuxMatrix();
    auxMatrix[3][0] = tX;
    auxMatrix[3][1] = tY;
    if(permanent){
        //Multiply transMatrix by auxMatrix and save the
        //result in transMAtrix, recalculate the transformed
        //vertices
        multiplyMatrix(transMatrix, auxMatrix);
        for( int i = 0; i < totalVertices; i++){
            transVertices[i] = vertices[i]*transMatrix;
        }
    }else{
        //Multiply transMatrix by auxMatrix and save the
        //result in auxMatrix, recalculate the transformed
        //vertices
        multiplyMatrix(transMatrix, auxMatrix, 0);
        for( int i = 0; i < totalVertices; i++){
            transVertices[i] = vertices[i]*auxMatrix;
        }
    }
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
void DrawableObject::multiplyMatrix( double matrix0[4][4], double matrix1[4][4], int firstSave ){

    double aux[4][4];

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
void DrawableObject::calculateNormals(){
    Vertex d1, d2;

    if(normals == NULL){
        normals = new Vertex[totalTriangles];
    }

    for(int i = 0; i < totalTriangles; i++){
        d1 = vertices[triangles[i][1]] - vertices[triangles[i][0]];
        d2 = vertices[triangles[i][2]] - vertices[triangles[i][1]];
        normals[i] = d1*d2;
    }
}

//--------------------------------------------------------------
void DrawableObject::calculateCentroids(){
    Vertex aux;

    if(triangleCentroids == NULL){
        triangleCentroids = new Vertex[totalTriangles];
    }

    for(int i = 0; i < totalTriangles; i++){
        aux.set(0,0,0);
        for(int j = 0; j < 3; j++){
            aux = aux + vertices[triangles[i][j]];
        }
        triangleCentroids[i] = aux/3.0;
    }
}
