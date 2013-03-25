#include "statusButton.hpp"

class testApp : public ofBaseApp{
    public:
		void setCurrentObject( DrawableObject* currentObject_ );
		void setState( const AppStates state_ );
		DrawableObject* getCurrentObject() const;
		void setPerspective( const bool active );
		void setZBuffer( const bool active );
		bool getZBuffer() const;
		void setPhongReflection( const bool active );
        bool getPhongReflection() const;
        void setGouraudShading( const bool active );
};

//--------------------------------------------------------------
StatusButton::StatusButton( testApp *app_, const Vertex vertex, const string buttonTex_,
    const AppStates state_, const int w, const int h, const ofColor color_ )
:Button(app_, vertex, buttonTex_, state_, w, h, color_)
{
}

//--------------------------------------------------------------
void StatusButton::checkPress( const Vertex mouse )
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
        case PHONG_R:
            app->setPhongReflection(pressed);
            break;
        case GOURAUD_S:
            app->setGouraudShading(pressed);
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
    case Z_BUFFER:
        if(app->getZBuffer()){
            pressed = true;
        }else{
            pressed = false;
        }
        break;
    case PHONG_R:
        if(app->getPhongReflection()){
            pressed = true;
        }else{
            pressed = false;
        }
        break;
    default:
        break;
    }

}
