#include "triangle.hpp"
#include "ofMain.h"
//--------------------------------------------------------------
Triangle::Triangle( const ofColor color_ )
:DrawableObject(3, color_)
{
    subtype = TRIANGLE;
    drawTriangles_ = true;
    DrawableObject::resetTransMatrix();
    DrawableObject::resetAuxMatrix();
    for( int i = 0; i < totalVertices; i++){
        vertices[i].setX( 0 );
        vertices[i].setY( 0 );
        vertices[i].setZ( 0 );
    }
    totalTriangles = 1;
    triangles = new int*[totalTriangles];

    triangles[0] = new int[3] {0,1,2};
}

//--------------------------------------------------------------
Triangle::Triangle( const Vertex &vertex0, const Vertex &vertex1 )
:DrawableObject(3)
{
    subtype = TRIANGLE;
    DrawableObject::resetTransMatrix();
    DrawableObject::resetAuxMatrix();
    setVertices(vertex0, vertex1);
    totalTriangles = 3;
    triangles = new int*[totalTriangles];

    triangles[0] = new int[3] {0,1,2};
}

//--------------------------------------------------------------
//Vertices order
/*
   v0
   /\
  /__\
v1    v2
*/
void Triangle::setVertices( const Vertex &vertex0, const Vertex &vertex1 ){

    vertices[0] = vertex0;
    vertices[1] = vertex1;
    vertices[2] = vertex1;

    vertices[2].setY(vertex0.getY() + vertex0.getX() - vertex1.getX());
    vertices[2].setX((vertex0.getX() + vertex1.getX())*0.5 );

    for( int i = 0; i < totalVertices; i++){
        transVertices[i] = vertices[i]*transMatrix;
    }

    calculateNormals();
    calculateCentroids();
    calculateDistances();
}

void Triangle::setVertices( const Vertex &vertex0, const Vertex &vertex1, const Vertex &vertex2 ){
    vertices[0] = vertex0;
    vertices[1] = vertex1;
    vertices[2] = vertex2;

    for( int i = 0; i < totalVertices; i++){
        transVertices[i] = vertices[i]*transMatrix;
    }

    calculateNormals();
    calculateCentroids();
    calculateDistances();
}

//--------------------------------------------------------------
void Triangle::draw(Renderer* const renderer) const {
    renderer->setColor( color );
    //Draw using triangles
    DrawableObject::draw(renderer);
}

