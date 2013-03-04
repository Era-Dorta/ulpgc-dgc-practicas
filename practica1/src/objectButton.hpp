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
        ObjectButton( testApp *app_, const Vertex vertex, const string buttonTex_,
            DrawableObject* object_, const int w = 40, const int h = 30,
            const ofColor color = ofColor::blue );
        ObjectButton( testApp *app_, const Vertex vertex, const string buttonTex_,
            DrawableObject* object_, const ofColor color = ofColor::blue );
        void checkPress( const Vertex mouse );
        void update();
        DrawableObject* getObject() const;
        void moveToLeft();
};

#endif
// OBJECT_BUTTON_H
