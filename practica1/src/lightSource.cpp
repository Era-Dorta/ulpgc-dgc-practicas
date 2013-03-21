#include "lightSource.hpp"

//--------------------------------------------------------------
LightSource::LightSource( const ofColor color_ )
:DrawableObject(5, color_)
{
    subtype = LIGHT;
    drawTriangles_ = true;
    DrawableObject::resetTransMatrix();
    DrawableObject::resetAuxMatrix();
    for( int i = 0; i < totalVertices; i++){
        vertices[i].setX( 0 );
        vertices[i].setY( 0 );
        vertices[i].setZ( 0 );
    }
    totalTriangles = 0;
    triangles = NULL;
}

//--------------------------------------------------------------
LightSource::LightSource( const Vertex &vertex )
:DrawableObject(5)
{
    subtype = LIGHT;
    DrawableObject::resetTransMatrix();
    DrawableObject::resetAuxMatrix();
    setVertex(vertex);
    totalTriangles = 0;
    triangles = NULL;
}

//--------------------------------------------------------------
void LightSource::setVertex( const Vertex &vertex ){

    for( int i = 0; i < totalVertices; i++){
        vertices[i] = vertex;
    }

    vertices[1].setX( vertices[0].getX() - 10 );
    vertices[2].setX( vertices[0].getX() + 10 );
    vertices[3].setY( vertices[0].getY() - 10 );
    vertices[4].setY( vertices[0].getY() + 10 );
}

//--------------------------------------------------------------
void LightSource::draw(Renderer* const renderer) const{

        renderer->setColor(color);
        renderer->rLine(vertices[1], vertices[2]);
        renderer->rLine(vertices[3], vertices[4]);
}
