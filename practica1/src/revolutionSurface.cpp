#include "revolutionSurface.hpp"
#include "ofMain.h"
#include <cmath>

//ROT defines how many times a revolution object
//is rotated
#define ROT 4

//--------------------------------------------------------------
void RevolutionSurface::calculateNormals()
{
    Vertex aux;
    DrawableObject::calculateNormals();
    int index;
    int currentLeft = totalTriangles - lineVerticesAmount - 2, currentRight = 1;
    int prevRight = currentRight;

    //Calculate normals for each vertex
    for(int i = 0; i < ROT; i++){
        //Top vertices
        aux.set(0,0,0);
        aux =  aux + normals[(lineVerticesAmount - 1)*2*i];
        aux =  aux + normals[((lineVerticesAmount - 1)*2*(i + 1))%totalTriangles];
        aux =  aux + normals[(((lineVerticesAmount - 1)*2*(i + 1)) + 1)%totalTriangles];
        aux.normalize();
        index = lineVerticesAmount*(i + 1)%totalVertices;
        verticesNormals[index] = aux;
        transVerticesNormals[index] = aux;

        //Bottom vertices
        aux.set(0,0,0);
        aux =  aux + normals[(lineVerticesAmount - 1)*2*i + lineVerticesAmount];
        aux =  aux + normals[(lineVerticesAmount - 1)*2*i + lineVerticesAmount + 1];
        aux =  aux + normals[(((lineVerticesAmount - 1)*2*(i + 1)) + 1 + lineVerticesAmount )%totalTriangles];
        aux.normalize();
        index = (lineVerticesAmount*(i + 1) + lineVerticesAmount - 1)%totalVertices;
        verticesNormals[index] = aux;
        transVerticesNormals[index] = aux;

        //Vertices between top and bottom
        for(int j = 1; j < lineVerticesAmount - 1; j++){
            aux.set(0,0,0);
            for(int k = 0; k < 3; k++){
                aux = aux + normals[currentLeft + k];
                aux = aux + normals[currentRight + k];
            }
            aux.normalize();
            index = i*lineVerticesAmount + j;
            verticesNormals[index] = aux;
            transVerticesNormals[index] = aux;
            currentLeft = currentLeft + 2;
            currentRight = currentRight + 2;
        }
        currentLeft = prevRight - 1;
        currentRight = (prevRight + lineVerticesAmount + 2)%totalTriangles;
        prevRight = currentRight;
    }
}

//--------------------------------------------------------------
RevolutionSurface::RevolutionSurface(const ofColor color_)
:DrawableObject(0, color_)
{
    hasAllVertices_ = false;
    subtype = REVOLUTION;
    DrawableObject::resetTransMatrix();
    DrawableObject::resetAuxMatrix();
}

//--------------------------------------------------------------
void RevolutionSurface::setVertex( const Vertex &vertex ){
    Vertex* auxVertices,* auxTransVertices;
    //Get memory for current vertices plus one
    auxVertices = new Vertex[totalVertices + 1];
    auxTransVertices = new Vertex[totalVertices + 1];

    //Copy old vertices in allocated memory
    for( int i = 0; i < totalVertices; i++){
        auxVertices[i] = vertices[i];
        auxTransVertices[i] = transVertices[i];
    }

    //Add the new vertex
    auxVertices[totalVertices] = vertex;
    auxTransVertices[totalVertices] = vertex;

    //Delete old vertices
    delete[] vertices;
    delete[] transVertices;

    vertices = auxVertices;
    transVertices = auxTransVertices;
    totalVertices++;
}

//--------------------------------------------------------------
void RevolutionSurface::noMoreVertices(){
    hasAllVertices_ = true;
    lineVerticesAmount = totalVertices;
    float rotation = (2*PI/ROT)/ROTATION_FACTOR;
    Vertex* auxVertices,* auxTransVertices;

    //Rotate the vertices ROT times and save them
    //as new vertices
    auxVertices = new Vertex[totalVertices*ROT];
    auxTransVertices = new Vertex[totalVertices*ROT];
    for(int i = 0; i < ROT; i++){
        rotate( Y, rotation*i, 0);
        for( int j = 0; j < totalVertices; j++){
            auxVertices[j+i*totalVertices] = transVertices[j];
            auxTransVertices[j+i*totalVertices] = transVertices[j];
        }
    }
    delete[] vertices;
    delete[] transVertices;

    totalVertices = totalVertices*ROT;
    vertices = auxVertices;
    transVertices = auxTransVertices;

    //Conect the vertices in triangles
    totalTriangles = (lineVerticesAmount - 1)*2*ROT;
    triangles = new int*[totalTriangles];
    int index = 0;
    //Clockwise order
    for(int i = 0; i < ROT; i++){
        for( int j = 0; j < lineVerticesAmount - 1; j++){

            int vIndex = j+i*lineVerticesAmount;
            triangles[index] = new int[3];

            triangles[index][0] = vIndex;
            triangles[index][1] = (vIndex + 1 + lineVerticesAmount)%totalVertices;
            triangles[index][2] = (vIndex + 1)%totalVertices;

            index++;
            triangles[index] = new int[3];

            triangles[index][0] = vIndex;
            triangles[index][1] = (vIndex + lineVerticesAmount)%totalVertices;
            triangles[index][2] = (vIndex + 1 + lineVerticesAmount)%totalVertices;

            index++;
        }
    }

    calculateNormals();
    calculateCentroids();
}

//--------------------------------------------------------------
void RevolutionSurface::setDrawHelper( const Vertex &mouse ){
    pmouse = mouse;
}

//--------------------------------------------------------------
void RevolutionSurface::draw(Renderer* const renderer) const {
    int i, j;
    //If the object is empty draw nothing
    if( totalVertices >= 1 ){
        renderer->setColor ( color );
        if(hasAllVertices_){
            //User ended drawing the object
            if(drawNormals_ || drawTriangles_){
                //Draw using triangles
                DrawableObject::draw(renderer);
            }else{
                //Draw using squares
                for( i = 0; i < ROT; i++ ){
                    for( j = 1; j < lineVerticesAmount; j++ ){
                    //Vertical lines
                    renderer->rLine(transVertices[j - 1+i*lineVerticesAmount],transVertices[j+i*lineVerticesAmount]);
                    //Horizontal lines
                    renderer->rLine(transVertices[(j - 1+i*lineVerticesAmount)%totalVertices], transVertices[(j - 1+(i+1)*lineVerticesAmount)%totalVertices]);
                    }
                    //Bottom horizontal lines
                    renderer->rLine(transVertices[(j - 1+i*lineVerticesAmount)%totalVertices], transVertices[(j - 1+(i+1)*lineVerticesAmount)%totalVertices]);
                   }
            }
        }else{
            //User is still drawing the object
            renderer->setColor ( ofColor::white);
            for( i = 1; i < totalVertices; i++ ){
                renderer->rLine(transVertices[i - 1], transVertices[i]);
            }
            renderer->rLine(transVertices[i - 1], pmouse);
        }
    }
}
