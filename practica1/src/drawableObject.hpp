#ifndef DRAWABLE_OBJECT_H
#define DRAWABLE_OBJECT_H

#include "vertex.hpp"
#include "renderer.hpp"
#include "ofMain.h"
#include <cmath>
using namespace std;

enum Axis { X, Y, Z };
enum Subtype { CUBE, REVOLUTION };

class DrawableObject {

    protected:
        Vertex* vertices;
        Vertex* transVertices;
        int** triangles;
        Vertex* normals;
        Vertex* triangleCentroids;
        Vertex* transNormals;
        Vertex* transTriangleCentroids;
        int totalTriangles;
        int totalVertices;
        Subtype subtype;
        float transMatrix[4][4];
        float auxMatrix[4][4];
        ofColor color;
        bool drawTriangles_;
        bool drawNormals_;
        bool drawFillTriangles_;

    private:
        void multiplyMatrix( float matrix0[4][4], float matrix1[4][4], int firstSave = 1 );
        void applyTransform( int permanent );

    protected:
        void calculateNormals();
        void calculateCentroids();

    public:
        DrawableObject( int totalVertices_, ofColor color_ = ofColor::white );
        DrawableObject( const DrawableObject& otherDrawableObject );
        virtual ~DrawableObject();
        virtual void draw(Renderer* renderer);
        virtual int getTotalVertices() { return totalVertices; };
        virtual void rotate( Axis axis, float amount, int permanent);
        virtual void translate( float tX, float tY, int permanent);
        void resetTransMatrix();
        void resetAuxMatrix();
        Subtype getSubtype() { return subtype; };
        void setDrawTriangles( bool activate ){ drawTriangles_ = activate; };
        bool getDrawTriangles(){ return drawTriangles_; };
        bool getNormals(){ return drawNormals_; };
        void setNormals( bool activate );
        void setFillTriangles( bool activate ){ drawFillTriangles_ = activate; };
        bool getFillTriangles(){ return drawFillTriangles_; };
};

#endif
// DRAWABLE_OBJECT_H
