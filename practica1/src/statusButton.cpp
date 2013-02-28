#include "statusButton.hpp"

class testApp : public ofBaseApp{
    public:
		void setCurrentObject( DrawableObject* currentObject_ );
		void setState( AppStates state_ );
		DrawableObject* getCurrentObject();
		void setPerspective( bool active );
		void setZBuffer( bool active );
};

//--------------------------------------------------------------
StatusButton::StatusButton( testApp *app_, Vertex vertex, string buttonTex_,AppStates state_, int w, int h,ofColor color_ )
:Button(app_, vertex, buttonTex_, state_, w, h, color_)
{
}

//--------------------------------------------------------------
void StatusButton::checkPress( Vertex mouse )
{
    if( mouse.getX() >= position.getX() &&  mouse.getX() <= position.getX() + width &&
        mouse.getY() >= position.getY() && mouse.getY() <= position.getY() + height ){
        pressed = !pressed;
        app->setState( state );
        switch(state){
        case DRAW_TRIANGLES:
            if(app->getCurrentObject()){
                app->getCurrentObject()->setDrawTriangles(pressed);
            }
            break;
        case DRAW_NORMALS:
            if(app->getCurrentObject()){
                app->getCurrentObject()->setDrawTriangles(pressed);
                app->getCurrentObject()->setNormals(pressed);
            }
            break;
        case FILL_TRIANGLES:
            if(app->getCurrentObject()){
                app->getCurrentObject()->setDrawTriangles(pressed);
                app->getCurrentObject()->setFillTriangles(pressed);
            }
            break;
        case Z_BUFFER:
            app->setZBuffer(pressed);
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
    case FILL_TRIANGLES:
        if(app->getCurrentObject() && app->getCurrentObject()->getFillTriangles()){
            pressed = true;
        }else{
            pressed = false;
        }
        break;
    case DRAW_NORMALS:
        if(app->getCurrentObject() && app->getCurrentObject()->getNormals()){
            pressed = true;
        }else{
            pressed = false;
        }
        break;
    default:
        break;
    }

}
