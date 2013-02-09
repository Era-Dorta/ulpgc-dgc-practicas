#include "revolutionSurface.hpp"
#include "ofMain.h"
#include <cmath>

//ROT defines how many times a revolution object
//is rotated
#define ROT 4
//--------------------------------------------------------------
RevolutionSurface::RevolutionSurface()
:DrawableObject(0)
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
void RevolutionSurface::draw(){
    int i, j;
    if( totalVertices >= 1 ){
        transVertices[0].drawing();
        transVertices[0].withPerpective();
        if(hasAllVertices_){

            ofSetColor ( 0 ,0 ,255 ); //Blue
            for( i = 0; i < ROT; i++ ){
                for( j = 1; j < lineVerticesAmount; j++ ){
                //Vertical lines
                ofLine(transVertices[j - 1+i*lineVerticesAmount].getX(), transVertices[j -1 + i*lineVerticesAmount].getY(),
                    transVertices[j+i*lineVerticesAmount].getX(), transVertices[j+i*lineVerticesAmount].getY());
                //Horizontal lines
                ofLine(transVertices[(j - 1+i*lineVerticesAmount)%totalVertices].getX(), transVertices[(j -1+i*lineVerticesAmount)%totalVertices].getY(),
                    transVertices[((j - 1) + (i+1)*lineVerticesAmount)%totalVertices].getX(), transVertices[(j - 1+(i+1)*lineVerticesAmount)%totalVertices].getY());
                }
                //Bottom horizontal lines
                ofLine(transVertices[(j - 1+i*lineVerticesAmount)%totalVertices].getX(), transVertices[(j -1+i*lineVerticesAmount)%totalVertices].getY(),
                    transVertices[((j - 1) + (i+1)*lineVerticesAmount)%totalVertices].getX(), transVertices[(j - 1+(i+1)*lineVerticesAmount)%totalVertices].getY());
            }
        }else{
            for( i = 1; i < totalVertices; i++ ){
                ofSetColor ( 0 ,0 ,255 ); //Blue
                ofLine(transVertices[i - 1].getX(), transVertices[i -1].getY(),
                    transVertices[i].getX(), transVertices[i].getY());
            }
            ofLine(transVertices[i - 1].getX(), transVertices[i - 1].getY(),
                    pmouse.getX(), pmouse.getY());
        }
        transVertices[0].withoutPerpective();
        transVertices[0].notDrawing();
    }
}
