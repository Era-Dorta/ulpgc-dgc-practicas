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
        double transMatrix[4][4];
        double auxMatrix[4][4];
        void multiplyMatrix( double matrix0[4][4], double matrix1[4][4], int firstSave = 1 );
        ofColor color;
        bool drawTriangles_;
        bool drawNormals_;


    public:
        DrawableObject( int totalVertices_, ofColor color_ = ofColor::white );
        DrawableObject( const DrawableObject& otherDrawableObject );
        virtual ~DrawableObject();
        virtual void draw(Renderer* renderer){};
        virtual int getTotalVertices() { return totalVertices; };
        virtual void rotate( Axis axis, double amount, int permanent);
        virtual void translate( double tX, double tY, int permanent);
        void resetTransMatrix();
        void resetAuxMatrix();
        Subtype getSubtype() { return subtype; };
        void setDrawTriangles( bool activate ){ drawTriangles_ = activate; };
        bool getDrawTriangles(){ return drawTriangles_; };
        bool getNormals(){ return drawNormals_; };
        void setNormals( bool activate );

    protected:
        void calculateNormals();
        void calculateCentroids();
};

#endif
// DRAWABLE_OBJECT_H
