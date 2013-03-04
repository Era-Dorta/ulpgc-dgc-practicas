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
        StatusButton( testApp *app_, const Vertex vertex, const string buttonTex_,
            const AppStates state_, const int w = 90, const int h = 30,
            const ofColor color_ = ofColor::white );
        void checkPress( const Vertex mouse );
        void update();
};

#endif
// STATUS_BUTTON_H
