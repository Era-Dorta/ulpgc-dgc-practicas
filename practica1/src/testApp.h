#pragma once

#include "ofMain.h"
#include <vector>
#include <cmath>

#define L_MOUSE 0
#define R_MOUSE 3

enum AppStates { ROTATING_X,ROTATING_Y,ROTATING_Z, ROTATING, TRANSLATING, DRAWING };
enum Axis { X, Y, Z };

using namespace std;

class testApp;

class Vertex {
    private:

    double x;
    double y;
    double z;
    double h;
    static int draw;
    static int perpective;

    public:

    Vertex() { x = 0; y = 0; z = 0; h = 1; draw = 0; };
    Vertex( double x_, double y_, double z_ ) { x = x_; y = y_; z = z_; h = 1; draw = 0; };
    void drawing() { draw = 1; };
    void notDrawing() { draw = 0; };
    void withPerpective() { perpective = 1; };
    void withoutPerpective() { perpective = 0; };
    double getX() const;
    double getY() const;
    double getZ() const;
    double getH() const { return h; };
    void setX( double x_ ) { x = x_; };
    void setY( double y_ ) { y = y_; };
    void setZ( double z_ ) { z = z_; };
    void setH( double h_ ) { h = h_; };
    void set( int pos, double val );
    bool operator==( const Vertex &otherVertex );
    Vertex operator*( const double matrix[4][4] );
    void operator=( const Vertex &otherVertex );
};

typedef struct Vertex Vertex;

class Cube {

    private:
        Vertex vertices[8];
        Vertex transVertices[8];
        double transMatrix[4][4];
        double auxMatrix[4][4];
        void multiplyMatrix( double matrix0[4][4], double matrix1[4][4], int firstSave = 1 );

    public:
        Cube();
        Cube( Vertex vertex0, Vertex vertex1 );
        void setVertices( Vertex vertex0, Vertex vertex1 );
        void draw();
        void resetTransMatrix();
        void resetAuxMatrix();
        void rotate( Axis axis, double amount, int permanent);
        void translate( double tX, double tY, int permanent);
};

class Button{

    private:
        Vertex vertices[4];
        Vertex center;
        bool pressed;
        int size;
        string buttonTex;
        testApp *app;
        AppStates state;

    public:
        Button( testApp *app_, Vertex vertex, string buttonTex_,AppStates state_ );
        void checkPress( Vertex mouse );
        bool isPressed();
        void draw();

};

class testApp : public ofBaseApp{

    private:
        Cube cube;
        Vertex pmouse;
        int pRawX, pRawY;
        bool opReady;
        vector<Button> buttonList;
        AppStates state;

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void setState( AppStates state_ );
		AppStates getState();

	private:
        void resetMatrix();

};


