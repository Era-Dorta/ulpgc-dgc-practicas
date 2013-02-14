#include "revolutionSurface.hpp"
#include "ofMain.h"
#include <cmath>

//ROT defines how many times a revolution object
//is rotated
#define ROT 4
//--------------------------------------------------------------
RevolutionSurface::RevolutionSurface(ofColor color_)
:DrawableObject(0, color_)
{
    hasAllVertices_ = false;
    subtype = REVOLUTION;
    DrawableObject::resetTransMatrix();
    DrawableObject::resetAuxMatrix();
}

//--------------------------------------------------------------
void RevolutionSurface::setVertex( Vertex &vertex ){
    Vertex* auxVertices,* auxTransVertices;

    auxVertices = new Vertex[totalVertices + 1];
    auxTransVertices = new Vertex[totalVertices + 1];

    for( int i = 0; i < totalVertices; i++){
        auxVertices[i] = vertices[i];
        auxTransVertices[i] = transVertices[i];
    }

    auxVertices[totalVertices] = vertex;
    auxTransVertices[totalVertices] = vertex;

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
    double rotation = (2*PI/ROT)/0.02;
    Vertex* auxVertices,* auxTransVertices;

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
}

//--------------------------------------------------------------
void RevolutionSurface::setDrawHelper( Vertex &mouse ){
    pmouse = mouse;
}

//--------------------------------------------------------------
void RevolutionSurface::draw(Renderer* renderer){
    int i, j;
    if( totalVertices >= 1 ){
        renderer->perspective(true);
        ofSetColor ( color );
        if(hasAllVertices_){
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
        }else{
            ofSetColor ( ofColor::white);
            for( i = 1; i < totalVertices; i++ ){
                renderer->rLine(transVertices[i - 1], transVertices[i]);
            }
            renderer->rLine(transVertices[i - 1], pmouse);
        }
    }
}
