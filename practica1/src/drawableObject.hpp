#ifndef DRAWABLE_OBJECT_H
#define DRAWABLE_OBJECT_H

#include "vertex.hpp"
#include "renderer.hpp"
#include "ofMain.h"
#include <cmath>

#define ROTATION_FACTOR 0.01
using namespace std;

enum Axis { X, Y, Z };
enum Subtype { TRIANGLE, CUBE, REVOLUTION, LIGHT };

class DrawableObject {

    protected:
        Vertex* vertices;
        Vertex* transVertices;
        int** triangles;
        Vertex* normals;
        Vertex* verticesNormals;
        Vertex* triangleCentroids;
        Vertex* transNormals;
        Vertex* transVerticesNormals;
        Vertex* transTriangleCentroids;
        float transMatrix[4][4];
        float auxMatrix[4][4];
        int totalTriangles;
        int totalVertices;
        Subtype subtype;
        ofColor color;
        bool drawTriangles_;
        bool drawNormals_;
        bool drawFillTriangles_;

    private:
        void multiplyMatrix( float matrix0[4][4], float matrix1[4][4], int firstSave = 1 );
        void applyTransform( const bool permanent );
        void applyTranslateTransform( const bool permanent );

    protected:
        virtual void calculateNormals();
        void calculateCentroids();

    public:
        DrawableObject( const int totalVertices_, const ofColor color_ = ofColor::white );
        DrawableObject( const DrawableObject& otherDrawableObject );
        virtual ~DrawableObject();
        virtual void draw(Renderer* const renderer) const;
        virtual int getTotalVertices() const{ return totalVertices; };
        virtual void rotate( const Axis axis, const float amount, const bool permanent);
        virtual void translate( const float tX, const float tY, const float tZ, const bool permanent);
        virtual void scale( const float sX, const float sY, const float sZ, const bool permanent);
        void resetTransMatrix();
        void resetAuxMatrix();
        Subtype getSubtype() const { return subtype; };
        virtual void setDrawTriangles( const bool activate ) { drawTriangles_ = activate; };
        virtual bool getDrawTriangles() const { return drawTriangles_; };
        virtual void setNormals( const bool activate );
        virtual bool getNormals() const { return drawNormals_; };
        virtual void setFillTriangles( const bool activate );
        virtual bool getFillTriangles() const { return drawFillTriangles_; };
        virtual void setColor( const ofColor color_ ){ color = color_; };
        virtual ofColor getColor() const { return color; };
};

#endif
// DRAWABLE_OBJECT_H
