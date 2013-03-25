#include "testApp.hpp"

#include "triangle.hpp"
#include "cube.hpp"
#include "revolutionSurface.hpp"
#include "lightSource.hpp"

#define N_ACTION_BUTTONS 10
#define N_STATUS_BUTTONS 7
#define N_BUTTONS N_ACTION_BUTTONS + N_STATUS_BUTTONS

//Center of the screen
const extern Vertex center;

//--------------------------------------------------------------
testApp::~testApp(){
    for(unsigned int i = 0; i < buttonList.size(); i++){
        delete buttonList[i];
    }
    for(unsigned int i = 0; i < objectList.size(); i++){
        delete objectList[i];
    }
}

//--------------------------------------------------------------
void testApp::setup(){
    int i = 0;
    renderer.setup(1024, 768);
    withPerspective = true;
    zbuffer = false;
    state = TRANSLATING;
    currentObject = NULL;
    //Create buttons
    int x = 400, y = -350;
    const int width = 90;
    const int height = 27;
    const int buttonsDistance = 7;
    nextObjButPos.set(-500,300,0);
    Vertex auxVertex;
    string buttonNames[N_ACTION_BUTTONS] = { "Rotate X", "Rotate Y", "Rotate Z",
        "Translate", "Scale", "New Triang", "New Cube", "New Rev", "New Light","Delete" };
    for(i = 0; i < N_ACTION_BUTTONS - DRAW_TRIANGLE; i++){
        auxVertex.set(x, y + i*(height + buttonsDistance), 0);
        buttonList.push_back( new Button(this, auxVertex, buttonNames[i], (AppStates)i, width, height) );
    }

    for(; i < N_ACTION_BUTTONS; i++){
        auxVertex.set(x, y + i*(height + buttonsDistance), 0);
        buttonList.push_back( new Button(this, auxVertex, buttonNames[i], (AppStates)i, width, height, ofColor::yellow) );
    }

    string sButtonNames[N_STATUS_BUTTONS] = { "Perspective","Triangles", "Normals", "Fill trian", "Z-Buffer", "Phong", "Gouraud" };
    for(i = N_ACTION_BUTTONS; i < N_BUTTONS; i++){
        auxVertex.set( x, y + i*(height + buttonsDistance), 0 );
        buttonList.push_back( new StatusButton(this, auxVertex, sButtonNames[i - N_ACTION_BUTTONS], (AppStates)i, width, height, ofColor::green) );
    }

    //By default activate perspective and draw rev
    auxVertex.set( x + 1, y + PERSPECTIVE*(height + buttonsDistance), 0 );
    buttonList[PERSPECTIVE]->checkPress(auxVertex);

    auxVertex.set( x + 1, y + DRAW_REVOLUTION*(height + buttonsDistance), 0 );
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
                currentObject->translate( x - pRawX, y - pRawY, 0, false );
                break;
            case SCALING:
                currentObject->scale( x - pRawX, y - pRawY, 0, false );
                break;
            case DRAW_TRIANGLE:
                ((Triangle*)objectList.back())->setVertices( pmouse, current );
                break;
            case DRAW_CUBE:
                ((Cube*)objectList.back())->setVertices( pmouse, current );
                break;
            case DRAW_REVOLUTION:
                break;
            case DRAW_LIGHT:
                ((LightSource*)objectList.back())->setVertex( current );
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

        //If not opReady the user only clicked the button
        if(opReady){
            switch(state){
            case DRAW_CUBE:
                currentColor = ofColor::fromHex(rand());
                objectList.push_back( new Cube(currentColor) );
                currentObject = objectList.back();
                currentObject->setFillTriangles(zbuffer);
                break;
            case DRAW_TRIANGLE:
                currentColor = ofColor::fromHex(rand());
                objectList.push_back( new Triangle(currentColor) );
                currentObject = objectList.back();
                currentObject->setFillTriangles(zbuffer);
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
                //Add vertex to revolution object
                ((RevolutionSurface*)currentObject)->setVertex( pmouse );
                //The user just clicked so avoid drawing a new line
                ((RevolutionSurface*)currentObject)->setDrawHelper(pmouse);
                currentObject->setFillTriangles(zbuffer);
                break;
            case DRAW_LIGHT:
                objectList.push_back( new LightSource() );
                currentObject = objectList.back();
                renderer.addLight((LightSource*)currentObject);
                break;
            default:
                break;
            }
        }
        break;
    case R_MOUSE:
        //Right click and drawing a revolution object
        if( objectList.size() > 0 && state == DRAW_REVOLUTION ){
            //User finished adding vertices, construct the object
            RevolutionSurface* revObject = (RevolutionSurface*)objectList.back();
            if(!revObject->hasAllVertices()){
                revObject->noMoreVertices();
                buttonList.push_back( new ObjectButton(this, nextObjButPos, "Rev", revObject, currentColor) );
                nextObjButPos.setX(nextObjButPos.getX() + 40);
            }
        }
        break;
    case WHEEL_FW:
        //Translate or Scale object in z
        if( objectList.size() > 0){
            switch(state){
            case TRANSLATING:
                currentObject->translate( 0, 0, -20, true );
                break;
            case SCALING:
                currentObject->scale( 0, 0, -20, true );
                break;
            default:
                break;
            }

        }
        break;
    case WHEEL_BK:
        //Translate or Scale object in z
        if( objectList.size() > 0){
            switch(state){
            case TRANSLATING:
                currentObject->translate( 0, 0, 20, true );
                break;
            case SCALING:
                currentObject->scale( 0, 0, 20, true );
                break;
            default:
                break;
            }

        }
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
                currentObject->translate( x - pRawX, y - pRawY, 0, true );
                break;
            case SCALING:
                currentObject->scale( x - pRawX, y - pRawY, 0, true );
                break;
            case DRAW_TRIANGLE:
                ((Triangle*)objectList.back())->setVertices( pmouse, current );
                auxButton = new ObjectButton(this, nextObjButPos, "Tria", objectList.back(), currentColor);
                buttonList.push_back( auxButton );
                nextObjButPos.setX(nextObjButPos.getX() + 40);
                break;
            case DRAW_CUBE:
                ((Cube*)objectList.back())->setVertices( pmouse, current );
                auxButton = new ObjectButton(this, nextObjButPos, "Cube", objectList.back(), currentColor);
                buttonList.push_back( auxButton );
                nextObjButPos.setX(nextObjButPos.getX() + 40);
                break;
            case DRAW_LIGHT:
                ((LightSource*)objectList.back())->setVertex( current );
                auxButton = new ObjectButton(this, nextObjButPos, "LS", objectList.back(), ofColor::white);
                buttonList.push_back( auxButton );
                nextObjButPos.setX(nextObjButPos.getX() + 40);
                break;
            default:
                break;
            }
        }else{
            switch(state){
            case DELETE:

                if( currentObject->getSubtype() == LIGHT ){
                    renderer.deleteLight((LightSource*)currentObject);
                }
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
                //Call object destructor
                delete currentObject;
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
void testApp::setState( const AppStates state_ ){
    state = state_;
    opReady = false;
}

//--------------------------------------------------------------
AppStates testApp::getState() const{
    return state;
}

//--------------------------------------------------------------
void testApp::setPerspective( const bool active ){
    withPerspective = active;
}

//--------------------------------------------------------------
bool testApp::getPerspective() const{
    return withPerspective;
}

//--------------------------------------------------------------
void testApp::setZBuffer( const bool active ){
    zbuffer = active;
    renderer.setZBuffer(zbuffer);
    //Activate/Deactivate fillTriangles in all objects
    for(unsigned int i = 0; i < objectList.size(); i++){
        objectList[i]->setFillTriangles(zbuffer);
    }
}

//--------------------------------------------------------------
bool testApp::getZBuffer() const{
    return zbuffer;
}

//--------------------------------------------------------------
void testApp::setPhongReflection( const bool active ){
    if(active){
        renderer.setLightingMode(PHONG_REFLECTION);
    }else{
        renderer.setLightingMode(NONE);
    }
    setZBuffer(active);
}

//--------------------------------------------------------------
bool testApp::getPhongReflection() const{
    return renderer.getLightingMode() == PHONG_REFLECTION ||
        renderer.getLightingMode() == GOURAUD_SHADING;
}


//--------------------------------------------------------------
void testApp::setGouraudShading( const bool active ){
    if(active){
        setZBuffer(true);
        renderer.setLightingMode(GOURAUD_SHADING);
    }else{
        setZBuffer(true);
        renderer.setLightingMode(PHONG_REFLECTION);
    }
}

//--------------------------------------------------------------
bool testApp::getGouraudShading() const{
    return renderer.getLightingMode() == GOURAUD_SHADING;
}

//--------------------------------------------------------------
void testApp::setCurrentObject( DrawableObject* currentObject_ ){
    currentObject = currentObject_;
}

//--------------------------------------------------------------
DrawableObject* testApp::getCurrentObject() const{
    return currentObject;
}
