#include "cube.hpp"
#include "ofMain.h"
//--------------------------------------------------------------
Cube::Cube( const ofColor color_ )
:DrawableObject(8, color_)
{
    subtype = CUBE;
    DrawableObject::resetTransMatrix();
    DrawableObject::resetAuxMatrix();
    for( int i = 0; i < totalVertices; i++){
        vertices[i].setX( 0 );
        vertices[i].setY( 0 );
        vertices[i].setZ( 0 );
    }
    totalTriangles = 12;
    triangles = new int*[totalTriangles];

    triangles[0] = new int[3] {0,1,2};
    triangles[1] = new int[3] {0,2,3};
    triangles[2] = new int[3] {1,5,6};
    triangles[3] = new int[3] {1,6,2};
    triangles[4] = new int[3] {5,4,7};
    triangles[5] = new int[3] {5,7,6};
    triangles[6] = new int[3] {4,5,1};
    triangles[7] = new int[3] {4,1,0};
    triangles[8] = new int[3] {3,2,6};
    triangles[9] = new int[3] {3,6,7};
    triangles[10] = new int[3] {4,0,3};
    triangles[11] = new int[3] {4,3,7};
}

//--------------------------------------------------------------
Cube::Cube( const Vertex &vertex0, const Vertex &vertex1 )
:DrawableObject(8)
{
    subtype = CUBE;
    DrawableObject::resetTransMatrix();
    DrawableObject::resetAuxMatrix();
    setVertices(vertex0, vertex1);
    totalTriangles = 12;
    triangles = new int*[totalTriangles];

    triangles[0] = new int[3] {0,1,2};
    triangles[1] = new int[3] {0,2,3};
    triangles[2] = new int[3] {1,5,6};
    triangles[3] = new int[3] {1,6,2};
    triangles[4] = new int[3] {5,4,7};
    triangles[5] = new int[3] {5,7,6};
    triangles[6] = new int[3] {4,5,1};
    triangles[7] = new int[3] {4,1,0};
    triangles[8] = new int[3] {3,2,6};
    triangles[9] = new int[3] {3,6,7};
    triangles[10] = new int[3] {4,0,3};
    triangles[11] = new int[3] {4,3,7};
}

//--------------------------------------------------------------
//Vertices order
//  0____1      Then goes the back face in the same order
//  |   |
//  |   |
// 3|___|2
void Cube::setVertices( const Vertex &vertex0, const Vertex &vertex1 ){
    float sideX, sideY, sideZ;
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

    calculateNormals();
    calculateCentroids();
    calculateDistances();
}

//--------------------------------------------------------------
void Cube::draw(Renderer* const renderer) const {
    renderer->setColor( color );
    if(drawNormals_ || drawTriangles_){
        //Draw using triangles
        DrawableObject::draw(renderer);
    }else{
        //Draw using squares
        for(int i = 0; i < 4; i++){
            //Back face of the cube
            renderer->rLine(transVertices[i + 4], transVertices[(i+1)%4 + 4]);
            //Lines between the two faces
            renderer->rLine(transVertices[i], transVertices[i + 4]);
            //Front face of the cube
            renderer->rLine(transVertices[i], transVertices[(i+1)%4]);
        }
    }
}
