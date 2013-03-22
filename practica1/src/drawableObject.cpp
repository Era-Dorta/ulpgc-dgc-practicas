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
void DrawableObject::applyTransform( const bool permanent ){
    Vertex translation;
    if(permanent){
        //Multiply transMatrix by auxMatrix and save the
        //result in transMAtrix, recalculate the transformed
        //vertices
        multiplyMatrix(transMatrix, auxMatrix, permanent);
        translation.set(transMatrix[3][0], transMatrix[3][1], transMatrix[3][2]);
        for( int i = 0; i < totalVertices; i++){
            transVertices[i] = vertices[i]*transMatrix;
            transVerticesNormals[i] =  verticesNormals[i]*transMatrix;
            transVerticesNormals[i] = transVerticesNormals[i] - translation;
        }
        for( int i = 0; i < totalTriangles; i++){
            transNormals[i] = normals[i]*transMatrix;
            //Undo a posible translation of the normals, they are vectors
            //so they are only affected by rotations
            transNormals[i] = transNormals[i] - translation;
            transTriangleCentroids[i] = triangleCentroids[i]*transMatrix;
        }
    }else{
        //Multiply transMatrix by auxMatrix and save the
        //result in auxMatrix, recalculate the transformed
        //vertices
        multiplyMatrix(transMatrix, auxMatrix, permanent);
        translation.set(auxMatrix[3][0], auxMatrix[3][1], auxMatrix[3][2]);
        for( int i = 0; i < totalVertices; i++){
            transVertices[i] = vertices[i]*auxMatrix;
            transVerticesNormals[i] =  verticesNormals[i]*auxMatrix;
            transVerticesNormals[i] = transVerticesNormals[i] - translation;
        }
        for( int i = 0; i < totalTriangles; i++){
            transNormals[i] = normals[i]*auxMatrix;
            transNormals[i] = transNormals[i] - translation;
            transTriangleCentroids[i] = triangleCentroids[i]*auxMatrix;
        }
    }
}

//--------------------------------------------------------------
void DrawableObject::applyTranslateTransform( const bool permanent ){
    if(permanent){
        //Multiply transMatrix by auxMatrix and save the
        //result in transMAtrix, recalculate the transformed
        //vertices
        multiplyMatrix(transMatrix, auxMatrix, permanent);
        for( int i = 0; i < totalVertices; i++){
            transVertices[i] = vertices[i]*transMatrix;
        }
        for( int i = 0; i < totalTriangles; i++){
            transTriangleCentroids[i] = triangleCentroids[i]*transMatrix;
        }
    }else{
        //Multiply transMatrix by auxMatrix and save the
        //result in auxMatrix, recalculate the transformed
        //vertices
        multiplyMatrix(transMatrix, auxMatrix, permanent);
        for( int i = 0; i < totalVertices; i++){
            transVertices[i] = vertices[i]*auxMatrix;
        }
        for( int i = 0; i < totalTriangles; i++){
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

    if(verticesNormals == NULL){
        verticesNormals = new Vertex[totalVertices];
        transVerticesNormals = new Vertex[totalVertices];
    }

    for(int i = 0; i < totalTriangles; i++){
        d1 = vertices[triangles[i][1]] - vertices[triangles[i][0]];
        d2 = vertices[triangles[i][2]] - vertices[triangles[i][1]];
        normals[i] = d1*d2;
        normals[i].normalize();
        transNormals[i] = normals[i];
    }
}

#define ONE_THIRD 1/3.0
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
        triangleCentroids[i] = aux*ONE_THIRD;
        transTriangleCentroids[i] = triangleCentroids[i];
    }
}

//--------------------------------------------------------------
DrawableObject::DrawableObject( const int totalVertices_, const ofColor color_  ){

    totalVertices = totalVertices_;
    if( totalVertices ){
        vertices = new Vertex[totalVertices];
        transVertices = new Vertex[totalVertices];
    }else{
        vertices =  NULL;
        transVertices = NULL;
    }
     triangles = NULL;
    normals = NULL;
    verticesNormals = NULL;
    triangleCentroids = NULL;
    transNormals = NULL;
    transVerticesNormals = NULL;
    transTriangleCentroids = NULL;
    totalTriangles = 0;
    drawTriangles_ = false;
    drawNormals_ = false;
    drawFillTriangles_ = false;
    color = color_;
}

//--------------------------------------------------------------
DrawableObject::DrawableObject( const DrawableObject& otherDrawableObject ){

    //Copy simple attributes
    totalTriangles = otherDrawableObject.totalTriangles;
    totalVertices = otherDrawableObject.totalVertices;
    subtype = otherDrawableObject.subtype;
    color = otherDrawableObject.color;
    drawTriangles_ = otherDrawableObject.drawTriangles_;
    drawNormals_ = otherDrawableObject.drawNormals_;
    drawFillTriangles_ = otherDrawableObject.drawFillTriangles_;

    //Get memory for the vertices
    if(totalVertices){
        vertices = new Vertex[totalVertices];
        transVertices = new Vertex[totalVertices];
    }else{
        vertices = NULL;
        transVertices = NULL;
    }

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
        verticesNormals = new Vertex[otherDrawableObject.totalVertices];
        transVerticesNormals = new Vertex[otherDrawableObject.totalVertices];
        //Copy the mean normal of each vertex
        for( int i = 0; i < totalVertices; i++ ){
            verticesNormals[i] = otherDrawableObject.verticesNormals[i];
            transVerticesNormals[i] = otherDrawableObject.transVerticesNormals[i];
        }
    }else{
        triangles = NULL;
        normals = NULL;
        verticesNormals = NULL;
        triangleCentroids = NULL;
        transNormals = NULL;
        transVerticesNormals = NULL;
        transTriangleCentroids = NULL;
    }
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
    delete[] verticesNormals;
    delete[] triangleCentroids;
    delete[] transNormals;
    delete[] transVerticesNormals;
    delete[] transTriangleCentroids;
}

//--------------------------------------------------------------
void DrawableObject::draw(Renderer* const renderer) const{

    if(drawFillTriangles_){
        for( int i = 0; i < totalTriangles; i++ ){
            renderer->rTriangleFill(transVertices[triangles[i][0]], transVertices[triangles[i][1]],
                transVertices[triangles[i][2]], transNormals[i], transTriangleCentroids[i]);
        }
    }else{
        if(drawNormals_){
            for( int i = 0; i < totalTriangles; i++ ){
                //Draw triangle
                renderer->rTriangle(transVertices[triangles[i][0]], transVertices[triangles[i][1]], transVertices[triangles[i][2]]);
                //Draw triangle's normal
                renderer->rLine(transTriangleCentroids[i], transTriangleCentroids[i] + transNormals[i]*15);
            }
            for( int i = 0; i < totalVertices; i++ ){
                renderer->rLine(transVertices[i], transVertices[i] + transVerticesNormals[i]*15 );
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
    cosVal = cos(ROTATION_FACTOR*amount);
    sinVal = sin(ROTATION_FACTOR*amount);
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

    for( int i = 0; i < totalTriangles; i++ ){
        transNormals[i].normalize();
    }

    if(transVerticesNormals != NULL){
        for( int i = 0; i < totalVertices; i++ ){
            transVerticesNormals[i].normalize();
        }
    }
}

//--------------------------------------------------------------
void DrawableObject::translate( const float tX, const float tY, const float tZ, const bool permanent){
    resetAuxMatrix();
    auxMatrix[3][0] = tX;
    auxMatrix[3][1] = tY;
    auxMatrix[3][2] = tZ;

    applyTranslateTransform(permanent);
}

//--------------------------------------------------------------
void DrawableObject::scale( const float sX, const float sY, const float sZ, const bool permanent){
    resetAuxMatrix();
    auxMatrix[0][0] = sX*ROTATION_FACTOR + 1;
    auxMatrix[1][1] = sY*ROTATION_FACTOR + 1;
    auxMatrix[2][2] = sZ*ROTATION_FACTOR + 1;

    applyTranslateTransform(permanent);
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
