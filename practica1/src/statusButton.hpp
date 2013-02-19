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
        StatusButton( testApp *app_, Vertex vertex, string buttonTex_,AppStates state_, ofColor color_ = ofColor::white, int size_ = 50 );
        void checkPress( Vertex mouse );
        void update();
};

#endif
// STATUS_BUTTON_H
