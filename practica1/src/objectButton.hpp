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
        ObjectButton( testApp *app_, Vertex vertex, string buttonTex_, DrawableObject* object_, int size_ = 25);
        void checkPress( Vertex mouse );
        void update();
        DrawableObject* getObject();
        void moveX(const int amount);
};

#endif
// OBJECT_BUTTON_H
