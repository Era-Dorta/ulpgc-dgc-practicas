#pragma once

#include "ofMain.h"

#define L_MOUSE 0
#define R_MOUSE 3
struct Vertex {
    int x;
    int y;
    int z;

    Vertex() { x = 0; y = 0; z = 0;};
    Vertex( int x_, int y_, int z_ ) { x = x_; y = y_; z = z_;};
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


