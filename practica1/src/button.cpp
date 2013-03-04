#include "button.hpp"
#include "ofMain.h"
#include "drawableObject.hpp"

class testApp : public ofBaseApp{
    public:
		void setState( AppStates state_ );
		AppStates getState();

};

//--------------------------------------------------------------
Button::Button( testApp *app_, const Vertex vertex, const string buttonTex_,
               const AppStates state_, const int w, const int h, const ofColor color_ ){
    pressed = false;
    width = w;
    height = h;
    buttonTex = buttonTex_;
    app = app_;
    state = state_;
    color = color_;
    colorInverted = color;
    colorInverted.invert();
    position = vertex;
    texPosition = position;
    texPosition.setX(position.getX() + 5 );
    texPosition.setY(position.getY() + height/2 );
}

//--------------------------------------------------------------
Button::Button( const Button& otherButton ){
    pressed = otherButton.pressed;
    width = otherButton.width;
    height = otherButton.height;
    buttonTex = otherButton.buttonTex;
    app = otherButton.app;
    state = otherButton.state;
    color = otherButton.color;
    colorInverted = otherButton.colorInverted;
    position = otherButton.position;
}

//--------------------------------------------------------------
void Button::checkPress( const Vertex mouse ){
    if( mouse.getX() >= position.getX() &&  mouse.getX() <= position.getX() + width &&
            mouse.getY() >= position.getY() && mouse.getY() <= position.getY() + height ){
        if( pressed ){
            pressed = false;
        }else{
            pressed = true;
            app->setState( state );
        }
    }
}

//--------------------------------------------------------------
bool Button::isPressed() const {
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
void Button::draw(const Renderer* renderer) const {
    ofSetColor ( color );
    if( pressed ){
        ofFill();
    }else{
        ofNoFill();
    }
    renderer->rRect(position, width, height);
    ofSetColor ( colorInverted );
    renderer->rDrawBitmapString(buttonTex, texPosition);
}

