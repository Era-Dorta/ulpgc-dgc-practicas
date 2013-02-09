#pragma once

#ifndef TEST_APP_H
#define TEST_APP_H

#include "ofMain.h"
#include "vertex.hpp"
#include "drawableObject.hpp"
#include <vector>

#define L_MOUSE 0
#define R_MOUSE 3

using namespace std;

class Button;
enum AppStates { ROTATING_X,ROTATING_Y,ROTATING_Z, ROTATING, TRANSLATING, DRAWING };

class testApp : public ofBaseApp{

    private:
        vector<DrawableObject*> objectList;
        //Cube cube;
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

//Class button is declared here to avoid some nasty
//circular problems
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

#endif
// TEST_APP_H

