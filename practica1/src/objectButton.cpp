#include "objectButton.hpp"

class testApp : public ofBaseApp{
    public:
		void setCurrentObject( DrawableObject* currentObject_ );
		void setState( AppStates state_ );
		DrawableObject* getCurrentObject();
};


//--------------------------------------------------------------
ObjectButton::ObjectButton( testApp *app_, Vertex vertex, string buttonTex_, DrawableObject* object_, ofColor color, int size_ )
:Button(app_, vertex, buttonTex_, OBJECT_BUTTON, color, size_ )
{
    object = object_;
    pressed = true;
}

//--------------------------------------------------------------
void ObjectButton::checkPress( Vertex mouse )
{
    if( mouse.getX() >= vertices[0].getX() &&  mouse.getX() <= vertices[1].getX() &&
        mouse.getY() >= vertices[0].getY() && mouse.getY() <= vertices[2].getY() ){
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
void ObjectButton::moveX(const int amount){
    for(int i = 0; i < 5; i++){
        vertices[i].setX(vertices[i].getX() + amount);
    }
}
