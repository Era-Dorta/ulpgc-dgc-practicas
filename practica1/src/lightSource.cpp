#include "lightSource.hpp"

//--------------------------------------------------------------
LightSource::LightSource( const ofColor color_ )
:DrawableObject(5, color_)
{
    subtype = LIGHT;
    drawTriangles_ = true;
    DrawableObject::resetTransMatrix();
    DrawableObject::resetAuxMatrix();
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
    for( int i = 0; i < totalVertices; i++){
        transVertices[i] = vertices[i]*transMatrix;
    }
}

//--------------------------------------------------------------
void LightSource::draw(Renderer* const renderer) const{

        renderer->setColor(color);
        renderer->rLine(transVertices[1], transVertices[2]);
        renderer->rLine(transVertices[3], transVertices[4]);
}

//--------------------------------------------------------------
const Vertex LightSource::getLightPosition() const{
    return transVertices[0];
}
