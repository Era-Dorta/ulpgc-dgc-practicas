#include "testApp.hpp"
#include "revolutionSurface.hpp"
#include "cube.hpp"
#include <cmath>

#define N_BUTTONS 8

//Center of the screen
extern Vertex center;

//--------------------------------------------------------------
void testApp::setup(){
    state = DRAW_CUBE;
    currentObject = NULL;
    //Create buttons
    int x = 400, y = -300;
    nextObjButPos.set(-500,300,0);
    Vertex* auxVertex;
    string buttonNames[N_BUTTONS] = { "Rotate X", "Rotate Y", "Rotate Z",
        "Rotate", "Translate", "New Cube", "Revolution", "Delete" };
    for(int i = 0; i < N_BUTTONS; i++){
        auxVertex = new Vertex( x, y + i*60, 0 );
        buttonList.push_back( new Button(this, *auxVertex, buttonNames[i], (AppStates)i) );
    }
    //This lines make draw button think it was pressed
    //This is done because it is the default state of the app
    auxVertex = new Vertex( x + 20, y + 5*60, 0 );
    buttonList[5]->checkPress(*auxVertex);
    opReady = true;
}

//--------------------------------------------------------------
void testApp::update(){
}

//--------------------------------------------------------------
void testApp::draw(){

    for( unsigned int i = 0; i < objectList.size(); i++){
        objectList[i]->draw(&renderer);
    }
    for( unsigned int i = 0; i < buttonList.size(); i++){
        buttonList[i]->draw(&renderer);
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    if(opReady){
        //Force 0,0 at the center of the screen
        Vertex current(x - center.getX(), y - center.getY(), 0);
        //While drawing a revolution object, draw a line from las vertex
        //to current mouse position
        if( state == DRAW_REVOLUTION ){
            if( objectList.size() > 0 && objectList.back()->getSubtype() == REVOLUTION ){
                RevolutionSurface* revObject = (RevolutionSurface*)objectList.back();
                    revObject->setDrawHelper(current);
            }
        }
    }
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

    if( button == L_MOUSE){
        if(opReady){
            //Force 0,0 at the center of the screen
            Vertex current(x - center.getX(), y - center.getY(), 0);
            switch(state){
            case ROTATING_X:
                currentObject->rotate( X, pRawY - y, 0 );
                break;
            case ROTATING_Y:
                currentObject->rotate( Y, pRawX - x, 0 );
                break;
            case ROTATING_Z:
                currentObject->rotate( Z, pRawY - y, 0 );
                break;
            case ROTATING:
                break;
            case TRANSLATING:
                currentObject->translate( x - pRawX, y - pRawY, 0 );
                break;
            case DRAW_CUBE:
                ((Cube*)objectList.back())->setVertices( pmouse, current );
                break;
            case DRAW_REVOLUTION:
                break;
            default:
                break;
            }
        }
    }
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    pRawX = x;
    pRawY = y;
    //Force 0,0 at the center of the screen
    x = x - center.getX();
    y = y - center.getY();
    if( button == L_MOUSE){
        pmouse.setX( x );
        pmouse.setY( y );
        pmouse.setZ( 0 );
        //Check buttons
        for( unsigned int i = 0; i < buttonList.size(); i++){
            buttonList[i]->checkPress(pmouse);
        }

        for( unsigned int i = 0; i < buttonList.size(); i++){
            buttonList[i]->update();
        }

        switch(state){
        case DRAW_CUBE:
            currentColor = ofColor::fromHex(rand());
            objectList.push_back( new Cube(currentColor) );
            currentObject = objectList.back();
            break;
        case DRAW_REVOLUTION:
            //First click on revolution, and previous object is not
            //a revolution objetc
            if( objectList.size() == 0 || objectList.back()->getSubtype() != REVOLUTION ){
                currentColor = ofColor::fromHex(rand());
                objectList.push_back( new RevolutionSurface(currentColor) );
                currentObject = objectList.back();
            }else{
                RevolutionSurface* revObject = (RevolutionSurface*)objectList.back();
                //Previous object is done, so make a new one
                if( revObject->hasAllVertices() ){
                    currentColor = ofColor::fromHex(rand());
                    objectList.push_back( new RevolutionSurface(currentColor) );
                    currentObject = objectList.back();
                //Still adding vertices to revolution object
                }else{
                    revObject->setVertex( pmouse );
                }
            }
            break;
        default:
            break;
        }
    }else{
        //Right click and drawing a revolution object
        if( button == R_MOUSE && state == DRAW_REVOLUTION ){
            //User finished adding vertices, construct the object
            RevolutionSurface* revObject = (RevolutionSurface*)objectList.back();
            revObject->noMoreVertices();
            buttonList.push_back( new ObjectButton(this, nextObjButPos, "Rev", revObject, currentColor) );
            nextObjButPos.setX(nextObjButPos.getX() + 50);
        }
    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    ObjectButton* auxButton;
    unsigned int i;
    if( button == L_MOUSE){
        if(opReady){
            //Force 0,0 at the center of the screen
            Vertex current(x - center.getX(), y - center.getY(), 0);
            switch(state){
            case ROTATING_X:
                currentObject->rotate( X, pRawY - y, 1 );
                break;
            case ROTATING_Y:
                currentObject->rotate( Y, pRawX - x, 1 );
                break;
            case ROTATING_Z:
                currentObject->rotate( Z, pRawY - y, 1 );
                break;
            case ROTATING:
                break;
            case TRANSLATING:
                currentObject->translate( x - pRawX, y - pRawY, 1 );
                break;
            case DRAW_CUBE:
                ((Cube*)objectList.back())->setVertices( pmouse, current );
                auxButton = new ObjectButton(this, nextObjButPos, "Cube", objectList.back(), currentColor);
                buttonList.push_back( auxButton );
                nextObjButPos.setX(nextObjButPos.getX() + 50);
                break;
            default:
                break;
            }
        }else{
            if(state == DELETE){
                //Find object's button  and delete
                for( i = N_BUTTONS - 1; i < buttonList.size(); i++){
                    if( ((ObjectButton*)buttonList[i])->getObject() == currentObject ){
                        buttonList.erase (buttonList.begin()+i);
                        break;
                    }
                }
                //Move all object's button who are on the right to the one eliminate
                //one place to the left
                for( ; i < buttonList.size(); i++){
                    ((ObjectButton*)buttonList[i])->moveX(-50);
                }
                //Eliminate object from object's list
                for(i = 0; i < objectList.size(); i++){
                    if(objectList[i] == currentObject ){
                        objectList.erase (objectList.begin()+i);
                    }
                }
                if(objectList.size() == 0){
                    currentObject = NULL;
                    nextObjButPos.setX(-500);
                }else{
                    currentObject = objectList.back();
                    nextObjButPos.setX(nextObjButPos.getX() - 50);
                }
            }else{
                opReady = true;
            }
        }
    }
    for( unsigned int i = 0; i < buttonList.size(); i++){
        buttonList[i]->update();
    }
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}

//--------------------------------------------------------------
void testApp::setState( AppStates state_ ){
    state = state_;
    opReady = false;
}

//--------------------------------------------------------------
AppStates testApp::getState(){
    return state;
}

//--------------------------------------------------------------
void testApp::setCurrentObject( DrawableObject* currentObject_ ){
    currentObject = currentObject_;
}

//--------------------------------------------------------------
DrawableObject* testApp::getCurrentObject(){
    return currentObject;
}
