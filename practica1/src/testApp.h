#pragma once

#include "ofMain.h"

#define L_MOUSE 0
#define R_MOUSE 3

class Vertex {



    private:

    int x;
    int y;
    int z;
    static int draw;
    static int perpective;

    public:

    Vertex() { x = 0; y = 0; z = 0; draw = 0; };
    Vertex( int x_, int y_, int z_ ) { x = x_; y = y_; z = z_; draw = 0; };
    void drawing() { draw = 1; };
    void notDrawing() { draw = 0; };
    void withPerpective() { perpective = 1; };
    void withoutPerpective() { perpective = 0; };
    int getX();
    int getY();
    int getZ();
    void setX( int x_ ) { x = x_; };
    void setY( int y_ ) { y = y_; };
    void setZ( int z_ ) { z = z_; };
};

typedef struct Vertex Vertex;

class Cube {

    private:

    Vertex vertices[8];

    public:
    Cube();
    Cube( Vertex vertex0, Vertex vertex1 );
    void setVertices( Vertex vertex0, Vertex vertex1 );
    void draw();
};

class testApp : public ofBaseApp{

    private:
        Cube cube;
        Vertex pmouse;
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

};


