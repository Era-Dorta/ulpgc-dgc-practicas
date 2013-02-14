#pragma once

#ifndef TEST_APP_H
#define TEST_APP_H

#include "ofMain.h"
#include "vertex.hpp"
#include "drawableObject.hpp"
#include "button.hpp"
#include <vector>

#define L_MOUSE 0
#define R_MOUSE 2

using namespace std;

class testApp : public ofBaseApp{

    private:
        vector<DrawableObject*> objectList;
        Vertex pmouse;
        int pRawX, pRawY;
        Vertex nextObjButPos;
        bool opReady;
        vector<Button> buttonList;
        AppStates state;
        DrawableObject* currentObject;

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
// TEST_APP_H

