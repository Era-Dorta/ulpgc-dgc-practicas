#include "statusButton.hpp"

class testApp : public ofBaseApp{
    public:
		void setCurrentObject( DrawableObject* currentObject_ );
		void setState( AppStates state_ );
		DrawableObject* getCurrentObject();
		void setPerspective( bool active );
};

//--------------------------------------------------------------
StatusButton::StatusButton( testApp *app_, Vertex vertex, string buttonTex_,AppStates state_, ofColor color_, int size_)
:Button(app_, vertex, buttonTex_, state_, color_, size_)
{
}

//--------------------------------------------------------------
void StatusButton::checkPress( Vertex mouse )
{
    if( mouse.getX() >= vertices[0].getX() &&  mouse.getX() <= vertices[1].getX() &&
        mouse.getY() >= vertices[0].getY() && mouse.getY() <= vertices[2].getY() ){
        pressed = !pressed;
        app->setState( state );
        switch(state){
        case DRAW_TRIANGLES:
            if(app->getCurrentObject()){
                app->getCurrentObject()->changeDrawTriangles();
            }
            break;
        case PERSPECTIVE:
            app->setPerspective(pressed);
            break;
        default:
            break;
        }
    }
}

//--------------------------------------------------------------
void StatusButton::update(){

    switch(state){
    case DRAW_TRIANGLES:
        if(app->getCurrentObject() && app->getCurrentObject()->getDrawTriangles()){
            pressed = true;
        }else{
            pressed = false;
        }
        break;
    default:
        break;
    }

}
