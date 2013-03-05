#ifndef DRAWABLE_OBJECT_H
#define DRAWABLE_OBJECT_H

#include "vertex.hpp"
#include "renderer.hpp"
#include "ofMain.h"
#include <cmath>
using namespace std;

enum Axis { X, Y, Z };
enum Subtype { TRIANGLE, CUBE, REVOLUTION };

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
        void applyTransform( const bool permanent );

    protected:
        void calculateNormals();
        void calculateCentroids();

    public:
        DrawableObject( const int totalVertices_, const ofColor color_ = ofColor::white );
        DrawableObject( const DrawableObject& otherDrawableObject );
        virtual ~DrawableObject();
        virtual void draw(const Renderer* renderer) const;
        virtual int getTotalVertices() const{ return totalVertices; };
        virtual void rotate( const Axis axis, const float amount, const bool permanent);
        virtual void translate( const float tX, const float tY, const float tZ, const bool permanent);
        void resetTransMatrix();
        void resetAuxMatrix();
        Subtype getSubtype() const { return subtype; };
        void setDrawTriangles( const bool activate ) { drawTriangles_ = activate; };
        bool getDrawTriangles() const { return drawTriangles_; };
        void setNormals( const bool activate );
        bool getNormals() const { return drawNormals_; };
        void setFillTriangles( const bool activate );
        bool getFillTriangles() const { return drawFillTriangles_; };
};

#endif
// DRAWABLE_OBJECT_H
