#pragma once

#ifndef BUTTON_H
#define BUTTON_H

#include "vertex.hpp"
#include "ofMain.h"
#include "cube.hpp"
#include <string>

using namespace std;

class testApp;
enum AppStates { ROTATING_X,ROTATING_Y,ROTATING_Z, ROTATING, TRANSLATING, DRAWING };

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

#endif
// BUTTON_H
