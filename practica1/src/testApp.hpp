#pragma once

#ifndef TEST_APP_H
#define TEST_APP_H

#include "ofMain.h"
#include "vertex.hpp"
#include "drawableObject.hpp"
#include "button.hpp"
#include "objectButton.hpp"
#include "statusButton.hpp"

#include <cmath>
#include <vector>
#include <cstdlib>

#define L_MOUSE 0
#define R_MOUSE 2
#define WHEEL_FW 3
#define WHEEL_BK 4

using namespace std;

class testApp : public ofBaseApp{

    private:
        vector<DrawableObject*> objectList;
        Vertex pmouse;
        int pRawX, pRawY;
        Vertex nextObjButPos;
        bool opReady;
        bool withPerspective;
        bool zbuffer;
        vector<Button*> buttonList;
        AppStates state;
        DrawableObject* currentObject;
        Renderer renderer;
        ofColor currentColor;

	public:
        ~testApp();

        //Default methods
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

		//Other methods
		void setState( const AppStates state_ );
		void setCurrentObject( DrawableObject* currentObject_ );
		void setPerspective( const bool active );
		bool getPerspective() const;
		void setZBuffer( const bool active );
		bool getZBuffer() const;
		void setPhongReflection( const bool active );
		bool getPhongReflection() const;
		void setGouraudShading( const bool active );
		bool getGouraudShading() const;
		DrawableObject* getCurrentObject() const;
		AppStates getState() const;
};

#endif
// TEST_APP_H

