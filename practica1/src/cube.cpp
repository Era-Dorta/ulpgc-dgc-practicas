#include "cube.hpp"
#include "ofMain.h"
//--------------------------------------------------------------
Cube::Cube()
:DrawableObject(8)
{
    subtype = CUBE;
    DrawableObject::resetTransMatrix();
    DrawableObject::resetAuxMatrix();
    for( int i = 0; i < totalVertices; i++){
        vertices[i].setX( 0 );
        vertices[i].setY( 0 );
        vertices[i].setZ( 0 );
    }
}

//--------------------------------------------------------------
Cube::Cube( Vertex &vertex0, Vertex &vertex1 )
:DrawableObject(8)
{
    subtype = CUBE;
    DrawableObject::resetTransMatrix();
    DrawableObject::resetAuxMatrix();
    setVertices(vertex0, vertex1);
}

//--------------------------------------------------------------
void Cube::setVertices( Vertex &vertex0, Vertex &vertex1 ){
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
    for( int i = 0; i < totalVertices; i++){
        transVertices[i] = vertices[i]*transMatrix;
    }
}

//--------------------------------------------------------------
void Cube::draw(Renderer* renderer){
    renderer->perspective(true);
    ofSetColor ( ofColor::white );
    for(int i = 0; i < 4; i++){
        //Back face of the cube
        renderer->rLine(transVertices[i + 4], transVertices[(i+1)%4 + 4]);
        //Lines between the two faces
        renderer->rLine(transVertices[i], transVertices[i + 4]);
        //Front face of the cube
        renderer->rLine(transVertices[i], transVertices[(i+1)%4]);
    }
}
