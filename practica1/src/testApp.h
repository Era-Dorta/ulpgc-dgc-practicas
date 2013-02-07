#pragma once

#include "ofMain.h"
#include <vector>
#include <cmath>

#define L_MOUSE 0
#define R_MOUSE 3

enum AppStates { ROTATING, TRANSLATING, DRAWING };
enum Axis { X, Y, Z };

using namespace std;

class testApp;

class Vertex {
    private:

    float x;
    float y;
    float z;
    float h;
    static int draw;
    static int perpective;

    public:

    Vertex() { x = 0; y = 0; z = 0; h = 1; draw = 0; };
    Vertex( float x_, float y_, float z_ ) { x = x_; y = y_; z = z_; h = 1; draw = 0; };
    void drawing() { draw = 1; };
    void notDrawing() { draw = 0; };
    void withPerpective() { perpective = 1; };
    void withoutPerpective() { perpective = 0; };
    float getX() const;
    float getY() const;
    float getZ() const;
    float getH() const { return h; };
    void setX( float x_ ) { x = x_; };
    void setY( float y_ ) { y = y_; };
    void setZ( float z_ ) { z = z_; };
    void setH( float h_ ) { h = h_; };
    void set( int pos, float val );
    bool operator==( const Vertex &otherVertex );
    Vertex operator*( const float matrix[4][4] );
    void operator=( const Vertex &otherVertex );
};

typedef struct Vertex Vertex;

class Cube {

    private:

    Vertex vertices[8];
    Vertex transVertices[8];
    float transMatrix[4][4];
    void multiplyMatrix( float matrix[4][4] );

    public:
    Cube();
    Cube( Vertex vertex0, Vertex vertex1 );
    void setVertices( Vertex vertex0, Vertex vertex1 );
    void draw();
    void resetMatrix();
    void rotate( int axis, int amount);
};

class Button{

    private:

    Vertex vertices[4];
    Vertex center;
    bool pressed;
    int size;
    string buttonTex;
    testApp *app;

    public:

    Button( testApp *app_, Vertex vertex, string buttonTex_ );
    void checkPress( Vertex mouse );
    bool isPressed();
    void draw();

};

class testApp : public ofBaseApp{

    private:
        Cube cube;
        Vertex pmouse;
        vector<Button> buttonList;
        int state;
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
		void setState( int state_ );
	private:
        void resetMatrix();

};


