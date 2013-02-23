#include "objectButton.hpp"

class testApp : public ofBaseApp{
    public:
		void setCurrentObject( DrawableObject* currentObject_ );
		void setState( AppStates state_ );
		DrawableObject* getCurrentObject();
};


//--------------------------------------------------------------
ObjectButton::ObjectButton( testApp *app_, Vertex vertex, string buttonTex_, DrawableObject* object_, int w, int h, ofColor color )
:Button(app_, vertex, buttonTex_, OBJECT_BUTTON, w, h, color)
{
    object = object_;
    pressed = true;
}

//--------------------------------------------------------------
ObjectButton::ObjectButton( testApp *app_, Vertex vertex, string buttonTex_, DrawableObject* object_, ofColor color)
:Button(app_, vertex, buttonTex_, OBJECT_BUTTON, 40, 30, color)
{
    object = object_;
    pressed = true;
}

//--------------------------------------------------------------
void ObjectButton::checkPress( Vertex mouse )
{
    if( mouse.getX() >= position.getX() &&  mouse.getX() <= position.getX() + width &&
        mouse.getY() >= position.getY() && mouse.getY() <= position.getY() + height ){
        pressed = true;
        app->setCurrentObject( object );
        app->setState( state );
    }
}

//--------------------------------------------------------------
void ObjectButton::update(){
    if(app->getCurrentObject() != object){
        pressed = false;
    }else{
        pressed = true;
    }
}

//--------------------------------------------------------------
DrawableObject* ObjectButton::getObject(){
    return object;
}

//--------------------------------------------------------------
void ObjectButton::moveToLeft(){
    position.setX(position.getX() - width);
    texPosition.setX(texPosition.getX() - width);
}
