#include "button.hpp"
#include "ofMain.h"

Renderer* Button::renderer = NULL;
int Button::nObjects = 0;

class testApp : public ofBaseApp{
    public:
		void setState( AppStates state_ );
		AppStates getState();

};

//--------------------------------------------------------------
Button::Button( testApp *app_, Vertex vertex, string buttonTex_, AppStates state_, int size_ ){
    pressed = false;
    size = size_;
    buttonTex = buttonTex_;
    app = app_;
    state = state_;
    vertices[0].set(vertex.getX(), vertex.getY(), vertex.getZ());
    vertices[1].set(vertex.getX() + size*2, vertex.getY(), vertex.getZ());
    vertices[2].set(vertex.getX() + size*2, vertex.getY() + size, vertex.getZ());
    vertices[3].set(vertex.getX(), vertex.getY() + size, vertex.getZ());
    vertices[4].set(vertices[3].getX() + size/3, vertices[3].getY() - size/2, vertex.getZ());
    if(nObjects == 0){
        renderer = new Renderer();
    }
    nObjects++;
}

//--------------------------------------------------------------
Button::Button( const Button& otherButton ){
    pressed = otherButton.pressed;
    size = otherButton.size;
    buttonTex = otherButton.buttonTex;
    app = otherButton.app;
    state = otherButton.state;
    vertices[0] = otherButton.vertices[0];
    vertices[1] = otherButton.vertices[1];
    vertices[2] = otherButton.vertices[2];
    vertices[3] = otherButton.vertices[3];
    vertices[4] = otherButton.vertices[4];
    nObjects++;
}

//--------------------------------------------------------------
Button::~Button(){
    nObjects--;
    if(nObjects == 0){
        delete renderer;
    }
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
}

//--------------------------------------------------------------
bool Button::isPressed(){
    return pressed;
}

//--------------------------------------------------------------
void Button::update(){
    //Other button was pressed, disable this one
    if(app->getState() != state){
        pressed = false;
    }
}

//--------------------------------------------------------------
void Button::draw(){
    if( pressed ){
        ofSetColor ( 0 ,0 ,255 ); //Blue
    }else{
        ofSetColor ( 255 ,0 ,0 ); //Red
    }
    renderer->perspective(false);
    for(int i = 0; i < 4; i++){
        renderer->rLine(vertices[i], vertices[(i + 1)%4]);
    }
    renderer->rDrawBitmapString(buttonTex, vertices[4]);
}

