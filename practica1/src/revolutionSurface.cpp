#include "revolutionSurface.hpp"
#include "ofMain.h"

//--------------------------------------------------------------
void RevolutionSurface::operator=( const RevolutionSurface& otherRevolutionSurface ){
    //Copy how much vertices the object has
    totalVertices = otherRevolutionSurface.totalVertices;
    //Get memory for the vertices
    /*
    vertices = otherRevolutionSurface.vertices;
    transVertices = otherRevolutionSurface.transVertices;
    */
    delete[] vertices;
    delete[] transVertices;
    vertices = new Vertex[totalVertices];
    transVertices = new Vertex[totalVertices];
    //Copy vertices content
    for( int i = 0; i < (totalVertices - 1); i++ ){
        vertices[i] = otherRevolutionSurface.vertices[i];
        transVertices[i] = otherRevolutionSurface.transVertices[i];
    }
    //Copy matrices content
    for( int i = 0; i < 4; i++ ){
        for( int j = 0; i < 4; i++ ){
            transMatrix[i][j] = otherRevolutionSurface.transMatrix[i][j];
            auxMatrix[i][j] = otherRevolutionSurface.auxMatrix[i][j];
        }
    }
}

//--------------------------------------------------------------
RevolutionSurface::RevolutionSurface( const RevolutionSurface &otherRevolutionSurface )
:DrawableObject(otherRevolutionSurface.totalVertices + 1)
{
    hasAllVertices_ = false;
    subtype = REVOLUTION;
    //DrawableObject::DrawableObject(otherRevolutionSurface.totalVertices + 1);
    DrawableObject::resetTransMatrix();
    DrawableObject::resetAuxMatrix();
    //Copy vertices content
    for( int i = 0; i < (totalVertices - 1); i++ ){
        cout << "Copiando " << i << "vertice\n";
        vertices[i] = otherRevolutionSurface.vertices[i];
        transVertices[i] = otherRevolutionSurface.transVertices[i];
    }
}

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
    (*this) = *(new RevolutionSurface( (*this) ));
    vertices[totalVertices - 1] = vertex;
    transVertices[totalVertices - 1] = vertex;
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
