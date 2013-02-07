#include "testApp.hpp"
#include "ofMain.h"

//--------------------------------------------------------------
Button::Button( testApp *app_, Vertex vertex, string buttonTex_, AppStates state_ ){
    pressed = false;
    size = 50;
    buttonTex = buttonTex_;
    app = app_;
    state = state_;
    vertices[0] = *(new Vertex( vertex.getX(), vertex.getY(), vertex.getZ()));
    vertices[1] = *(new Vertex( vertex.getX() + size*2, vertex.getY(), vertex.getZ()));
    vertices[2] = *(new Vertex( vertex.getX() + size*2, vertex.getY() + size, vertex.getZ()));
    vertices[3] = *(new Vertex( vertex.getX(), vertex.getY() + size, vertex.getZ()));
}

//--------------------------------------------------------------
void Button::checkPress( Vertex mouse ){
    if( mouse.getX() >= vertices[0].getX() &&  mouse.getX() <= vertices[1].getX() &&
        mouse.getY() >= vertices[0].getY() && mouse.getY() <= vertices[2].getY() ){
        if( pressed ){
            pressed = false;
        }else{
            pressed = true;
            app->setState( state );
        }
    }
    //Other button was pressed, disable this one
    if(app->getState() != state){
        pressed = false;
    }
}

//--------------------------------------------------------------
bool Button::isPressed(){
    return pressed;
}

//--------------------------------------------------------------
void Button::draw(){
    if( pressed ){
        ofSetColor ( 0 ,0 ,255 ); //Blue
    }else{
        ofSetColor ( 255 ,0 ,0 ); //Red
    }
    vertices[0].drawing();
    for(int i = 0; i < 4; i++){
        ofLine(vertices[i].getX(), vertices[i].getY(), vertices[(i + 1)%4].getX(), vertices[(i + 1)%4].getY());
    }
    ofDrawBitmapString(buttonTex, vertices[3].getX() + size/3, vertices[3].getY() - size/2);
    vertices[0].notDrawing();
}
