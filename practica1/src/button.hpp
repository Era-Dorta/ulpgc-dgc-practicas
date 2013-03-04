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
enum AppStates { ROTATING_X,ROTATING_Y,ROTATING_Z, TRANSLATING,
    DRAW_CUBE, DRAW_REVOLUTION, DELETE, PERSPECTIVE, DRAW_TRIANGLES,
    DRAW_NORMALS, FILL_TRIANGLES, Z_BUFFER, OBJECT_BUTTON };
//Class button is declared here to avoid some nasty
//circular problems
class Button{

    protected:
        Vertex position;
        Vertex texPosition;
        bool pressed;
        int width;
        int height;
        string buttonTex;
        testApp *app;
        AppStates state;
        ofColor color;
        ofColor colorInverted;

    public:
        Button( testApp *app_, const Vertex vertex, const string buttonTex_,
            const AppStates state_, const int w = 90, const int h = 30,
            const ofColor color_ = ofColor::white );
        Button( const Button& otherButton );
        virtual void checkPress( const Vertex mouse );
        virtual bool isPressed() const;
        virtual void update();
        virtual void draw(const Renderer* renderer) const;
        virtual int getWidth() const { return width; };
};

#endif
// BUTTON_H
