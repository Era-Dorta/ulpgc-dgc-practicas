#pragma once

#ifndef BUTTON_H
#define BUTTON_H

#include "ofMain.h"
#include "vertex.hpp"
#include "renderer.hpp"
#include <string>
#include <iostream>
using namespace std;
class testApp;
enum AppStates { ROTATING_X,ROTATING_Y,ROTATING_Z, ROTATING, TRANSLATING,
    DRAW_CUBE, DRAW_REVOLUTION, DRAW_TRIANGLES, DELETE, OBJECT_BUTTON };
//Class button is declared here to avoid some nasty
//circular problems
class Button{

    protected:
        Vertex vertices[5];
        Vertex center;
        bool pressed;
        int size;
        string buttonTex;
        testApp *app;
        AppStates state;
        ofColor color;
        ofColor colorInverted;

    public:
        Button( testApp *app_, Vertex vertex, string buttonTex_,AppStates state_, ofColor color_ = ofColor::white, int size_ = 50 );
        Button( const Button& otherButton );
        virtual void checkPress( Vertex mouse );
        bool isPressed();
        virtual void update();
        void draw(Renderer* renderer);
};

#endif
// BUTTON_H
