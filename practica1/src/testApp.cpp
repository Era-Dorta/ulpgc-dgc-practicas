#include "testApp.hpp"
#include "revolutionSurface.hpp"
#include "cube.hpp"
#include <cmath>

#define N_ACTION_BUTTONS 7
#define N_STATUS_BUTTONS 5
#define N_BUTTONS N_ACTION_BUTTONS + N_STATUS_BUTTONS

//Center of the screen
extern Vertex center;

//--------------------------------------------------------------
void testApp::setup(){
    int i = 0;
    renderer.setup(1024, 768);
    withPerspective = true;
    zbuffer = false;
    state = DRAW_CUBE;
    currentObject = NULL;
    //Create buttons
    int x = 400, y = -300;
    const int width = 90;
    const int height = 30;
    nextObjButPos.set(-500,300,0);
    Vertex auxVertex;
    string buttonNames[N_ACTION_BUTTONS] = { "Rotate X", "Rotate Y", "Rotate Z",
        "Translate", "New Cube", "New Rev", "Delete" };
    for(i = 0; i < N_ACTION_BUTTONS; i++){
        auxVertex.set(x, y + i*(height + 10), 0);
        buttonList.push_back( new Button(this, auxVertex, buttonNames[i], (AppStates)i, width, height) );
    }
;

    string sButtonNames[N_STATUS_BUTTONS] = { "Perspective","Triangles", "Normals", "Fill trian", "Z-Buffer" };
    for(i = N_ACTION_BUTTONS; i < N_BUTTONS; i++){
        auxVertex.set( x, y + i*(height + 10), 0 );
        buttonList.push_back( new StatusButton(this, auxVertex, sButtonNames[i - N_ACTION_BUTTONS], (AppStates)i, width, height) );
    }

    //By default activate perspective and draw cube
    auxVertex.set( x + 1, y + PERSPECTIVE*(height + 10), 0 );
    buttonList[PERSPECTIVE]->checkPress(auxVertex);

    auxVertex.set( x + 1, y + DRAW_REVOLUTION*(height + 10), 0 );
    buttonList[DRAW_REVOLUTION]->checkPress(auxVertex);

    opReady = true;
}

//--------------------------------------------------------------
void testApp::update(){
}

//--------------------------------------------------------------
void testApp::draw(){

    renderer.resetZBuffer();
    renderer.perspective(withPerspective);
    for( unsigned int i = 0; i < objectList.size(); i++){
        objectList[i]->draw(&renderer);
    }
    renderer.perspective(false);
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
                currentObject->rotate( X, pRawY - y, false );
                break;
            case ROTATING_Y:
                currentObject->rotate( Y, pRawX - x, false );
                break;
            case ROTATING_Z:
                currentObject->rotate( Z, pRawY - y, false );
                break;
            case TRANSLATING:
                currentObject->translate( x - pRawX, y - pRawY, false );
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
    switch(button){
    case L_MOUSE:
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
            //a revolution object
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
                }
            }

            //If not opReady the user only clicked the button, so that is
            //not a valid vertex
            if(opReady){
                //Add vertex to revolution object
                ((RevolutionSurface*)currentObject)->setVertex( pmouse );
                //The user just clicked so avoid drawing a new line
                ((RevolutionSurface*)currentObject)->setDrawHelper(pmouse);
            }

            break;
        default:
            break;
        }
        break;
    case R_MOUSE:
        //Right click and drawing a revolution object
        if( state == DRAW_REVOLUTION ){
            //User finished adding vertices, construct the object
            RevolutionSurface* revObject = (RevolutionSurface*)objectList.back();
            revObject->noMoreVertices();
            buttonList.push_back( new ObjectButton(this, nextObjButPos, "Rev", revObject, currentColor) );
            nextObjButPos.setX(nextObjButPos.getX() + 40);
        }
        break;
    case WHEEL_FW:
        break;
    case WHEEL_BK:
        break;
    default:
        break;
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
                currentObject->rotate( X, pRawY - y, true );
                break;
            case ROTATING_Y:
                currentObject->rotate( Y, pRawX - x, true );
                break;
            case ROTATING_Z:
                currentObject->rotate( Z, pRawY - y, true );
                break;
            case TRANSLATING:
                currentObject->translate( x - pRawX, y - pRawY, true );
                break;
            case DRAW_CUBE:
                ((Cube*)objectList.back())->setVertices( pmouse, current );
                auxButton = new ObjectButton(this, nextObjButPos, "Cube", objectList.back(), currentColor);
                buttonList.push_back( auxButton );
                nextObjButPos.setX(nextObjButPos.getX() + 40);
                break;
            default:
                break;
            }
        }else{
            switch(state){
            case DELETE:
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
                    ((ObjectButton*)buttonList[i])->moveToLeft();
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
                    nextObjButPos.setX(nextObjButPos.getX() - 40);
                }
                break;
            default:
                opReady = true;
                break;
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
void testApp::setPerspective( bool active ){
    withPerspective = active;
}

//--------------------------------------------------------------
bool testApp::getPerspective(){
    return withPerspective;
}

//--------------------------------------------------------------
void testApp::setZBuffer( bool active ){
    zbuffer = active;
    renderer.setZBuffer(zbuffer);
    //Activate/Deactivate fillTriangles in all objects
    for(unsigned int i = 0; i < objectList.size(); i++){
        objectList[i]->setFillTriangles(zbuffer);
    }
}

//--------------------------------------------------------------
bool testApp::getZBuffer(){
    return zbuffer;
}

//--------------------------------------------------------------
void testApp::setCurrentObject( DrawableObject* currentObject_ ){
    currentObject = currentObject_;
}

//--------------------------------------------------------------
DrawableObject* testApp::getCurrentObject(){
    return currentObject;
}
