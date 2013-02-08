#include "cube.hpp"
#include "ofMain.h"
//--------------------------------------------------------------
Cube::Cube() :DrawableObject(8) {
    DrawableObject::resetTransMatrix();
    DrawableObject::resetAuxMatrix();
    for( int i = 0; i < 0; i++){
        vertices[i].setX( 0 );
        vertices[i].setY( 0 );
        vertices[i].setZ( 0 );
    }
}

//--------------------------------------------------------------
Cube::Cube( Vertex vertex0, Vertex vertex1 ) :DrawableObject(8) {
    DrawableObject::resetTransMatrix();
    DrawableObject::resetAuxMatrix();
    setVertices(vertex0, vertex1);
}

//--------------------------------------------------------------
void Cube::setVertices( Vertex vertex0, Vertex vertex1 ){
    double sideX, sideY, sideZ;
    int z;
    sideX = vertex1.getX() - vertex0.getX();
    sideY = vertex1.getY() - vertex0.getY();
    //Depth of the cube is length of side x
    sideZ = abs(sideX);

    for(z = 0; z < 2; z++){
        vertices[0 + z*4].setX( vertex0.getX() );
        vertices[0 + z*4].setY( vertex0.getY() );
        vertices[0 + z*4].setZ( vertex0.getZ() - z*sideZ );

        vertices[1 + z*4].setX( vertex0.getX() + sideX );
        vertices[1 + z*4].setY( vertex0.getY() );
        vertices[1 + z*4].setZ( vertex0.getZ() - z*sideZ );

        vertices[2 + z*4].setX( vertex0.getX() + sideX );
        vertices[2 + z*4].setY( vertex0.getY() + sideY );
        vertices[2 + z*4].setZ( vertex0.getZ() - z*sideZ );

        vertices[3 + z*4].setX( vertex0.getX() );
        vertices[3 + z*4].setY( vertex0.getY() + sideY );
        vertices[3 + z*4].setZ( vertex0.getZ() - z*sideZ );
    }
    for( int i = 0; i < 8; i++){
        transVertices[i] = vertices[i]*transMatrix;
    }
}

//--------------------------------------------------------------
void Cube::draw(){
    transVertices[0].drawing();
    transVertices[0].withPerpective();
    for(int i = 0; i < 4; i++){
        //Back face of the cube
        ofSetColor ( 0 ,255 ,0 ); //Green
        ofLine(transVertices[i + 4].getX(), transVertices[i + 4].getY(), transVertices[(i+1)%4 + 4].getX(), transVertices[(i+1)%4 + 4].getY());
        //Lines between the two faces
        ofSetColor ( 0 ,0 ,255 ); //Blue
        ofLine(transVertices[i].getX(), transVertices[i].getY(), transVertices[i + 4].getX(), transVertices[i + 4].getY());
        //Front face of the cube
        ofSetColor ( 255 ,0 ,0 ); //Red
        ofLine(transVertices[i].getX(), transVertices[i].getY(), transVertices[(i+1)%4].getX(), transVertices[(i+1)%4].getY());
    }
    //getc(stdin);
    transVertices[0].withoutPerpective();
    transVertices[0].notDrawing();
}
