#include "revolutionSurface.hpp"
#include "ofMain.h"

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
void RevolutionSurface::setVertex( Vertex vertex ){
    cout << "total antes "  << totalVertices << endl;
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

    cout << "total despues "  << totalVertices << endl;
}

//--------------------------------------------------------------
void RevolutionSurface::noMoreVertices(){
    DrawableObject::resetTransMatrix();
    DrawableObject::resetAuxMatrix();
    hasAllVertices_ = true;
}

//--------------------------------------------------------------
void RevolutionSurface::draw(){
    if( totalVertices > 1 ){
        transVertices[0].drawing();
        transVertices[0].withPerpective();
        if(hasAllVertices_){

        }else{
            for(int i = 1; i < totalVertices; i++){
                //Lines
                //cout << "Dibjuando vertices"<< i <<  " \n";
                ofSetColor ( 0 ,0 ,255 ); //Blue
                ofLine(transVertices[i - 1].getX(), transVertices[i -1].getY(),
                    transVertices[i].getX(), transVertices[i].getY());
            }
        }
        transVertices[0].withoutPerpective();
        transVertices[0].notDrawing();
    }
}
