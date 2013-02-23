#pragma once

#ifndef OBJECT_BUTTON_H
#define OBJECT_BUTTON_H

#include "drawableObject.hpp"
#include "button.hpp"
#include "ofMain.h"

class ObjectButton : public Button{

    private:
        DrawableObject* object;

    public:
        ObjectButton( testApp *app_, Vertex vertex, string buttonTex_, DrawableObject* object_, int w = 40, int h = 30, ofColor color = ofColor::blue );
        ObjectButton( testApp *app_, Vertex vertex, string buttonTex_, DrawableObject* object_, ofColor color = ofColor::blue );
        void checkPress( Vertex mouse );
        void update();
        DrawableObject* getObject();
        void moveToLeft();
};

#endif
// OBJECT_BUTTON_H
