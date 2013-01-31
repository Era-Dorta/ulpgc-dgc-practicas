#pragma once

#include "ofMain.h"

#define L_MOUSE 0
#define R_MOUSE 3
class Vertex {

    private:

    int x;
    int y;
    int z;
    int draw = 0;

    public:

    Vertex() { x = 0; y = 0; z = 0;};
    Vertex( int x_, int y_, int z_ ) { x = x_; y = y_; z = z_;};
    void draw(); { draw = 1; };
    void not_draw(); { draw = 0; };
    int getX();
    int getY();
    int getZ();
    int setX( int x_ ) { x = x_; };
    int setY( int y_ ) { y = y_; };
    int setZ( int z_ ) { z = z_; };
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
        //Center of the screen
        Vertex center(512,384,0);
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


