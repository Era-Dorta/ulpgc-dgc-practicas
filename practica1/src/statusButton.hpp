#pragma once

#ifndef STATUS_BUTTON_H
#define STATUS_BUTTON_H

#include "drawableObject.hpp"
#include "button.hpp"
#include "ofMain.h"

class StatusButton : public Button{

    private:
        DrawableObject* object;

    public:
        StatusButton( testApp *app_, Vertex vertex, string buttonTex_,AppStates state_, int w = 90, int h = 30, ofColor color_ = ofColor::white );
        void checkPress( Vertex mouse );
        void update();
};

#endif
// STATUS_BUTTON_H
