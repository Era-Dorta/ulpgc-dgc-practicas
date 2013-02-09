#include "testApp.hpp"
#include "revolutionSurface.hpp"
#include "cube.hpp"
#include <cmath>

#define N_BUTTONS 7
static double transMatrix[4][4];
//Center of the screen
extern Vertex center;

void showMatrix( const double matrix[4][4])
{
    cout << "[";
	for( unsigned int i=0; i<4; i++ ){
		for( unsigned int j=0; j<4; j++ ){
			cout << matrix[i][j] << " ";
		}
		cout << "; \n";
	}
	cout << "]\n";
}

//--------------------------------------------------------------
void testApp::setup(){
    state = DRAW_CUBE;
    //Initialize matrix
    resetMatrix();
    //Create buttons
    int x = 400, y = -300;
    Vertex* auxVertex;
    string buttonNames[N_BUTTONS] = { "Rotate X", "Rotate Y", "Rotate Z",
        "Rotate", "Translate", "Cube", "Revolution" };
    Button* auxButton;
    for(int i = 0; i < N_BUTTONS; i++){
        auxVertex = new Vertex( x, y + i*60, 0 );
        auxButton = new Button(this, *auxVertex, buttonNames[i], (AppStates)i);
        buttonList.push_back( *auxButton );
    }
    //This lines make draw button think it was pressed
    //This is done because it is the default state of the app
    auxVertex = new Vertex( x + 20, y + 5*60, 0 );
    buttonList[5].checkPress(*auxVertex);
    opReady = true;
}

//--------------------------------------------------------------
void testApp::update(){
}

//--------------------------------------------------------------
void testApp::draw(){

    for( unsigned int i = 0; i < objectList.size(); i++){
        objectList[i]->draw();
    }
    for( unsigned int i = 0; i < buttonList.size(); i++){
        buttonList[i].draw();
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
                objectList.back()->rotate( X, pRawY - y, 0 );
                break;
            case ROTATING_Y:
                objectList.back()->rotate( Y, pRawX - x, 0 );
                break;
            case ROTATING_Z:
                objectList.back()->rotate( Z, pRawY - y, 0 );
                break;
            case ROTATING:
                break;
            case TRANSLATING:
                objectList.back()->translate( x - pRawX, y - pRawY, 0 );
                break;
            case DRAW_CUBE:
                ((Cube*)objectList.back())->setVertices( pmouse, current );
                break;
            case DRAW_REVOLUTION:
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
            buttonList[i].checkPress(pmouse);
        }

        switch(state){
        case DRAW_CUBE:
            objectList.push_back( new Cube() );
            break;
        case DRAW_REVOLUTION:
            //First click on revolution, and previous object is not
            //a revolution objetc
            if( objectList.size() == 0 || objectList.back()->getSubtype() != REVOLUTION ){
                objectList.push_back( new RevolutionSurface() );
            }else{
                RevolutionSurface* revObject = (RevolutionSurface*)objectList.back();
                //Previous object is done, so make a new one
                if( revObject->hasAllVertices() ){
                    objectList.push_back( new RevolutionSurface() );
                //Still adding vertices to revolution object
                }else{
                    revObject->setVertex( pmouse );
                }
            }
            break;
        default:
            //FIXME How to nop??
            x--;
        }
    }else{
        //Right click and drawing a revolution object
        if( button == R_MOUSE && state == DRAW_REVOLUTION ){
            //User finished adding vertices, construct the object
            RevolutionSurface* revObject = (RevolutionSurface*)objectList.back();
            revObject->noMoreVertices();
        }
    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

    if( button == L_MOUSE){
        if(opReady){
            //Force 0,0 at the center of the screen
            Vertex current(x - center.getX(), y - center.getY(), 0);
            switch(state){
            case ROTATING_X:
                objectList.back()->rotate( X, pRawY - y, 1 );
                break;
            case ROTATING_Y:
                objectList.back()->rotate( Y, pRawX - x, 1 );
                break;
            case ROTATING_Z:
                objectList.back()->rotate( Z, pRawY - y, 1 );
                break;
            case ROTATING:
                break;
            case TRANSLATING:
                objectList.back()->translate( x - pRawX, y - pRawY, 1 );
                break;
            case DRAW_CUBE:
                ((Cube*)objectList.back())->setVertices( pmouse, current );
                break;
            case DRAW_REVOLUTION:
                break;
            }
        }else{
            opReady = true;
        }
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
void testApp::resetMatrix(){
    for( int i = 0; i < 4; i++){
        for( int j = 0; j < 4; j++){
            if(i == j){
                transMatrix[i][j] = 1;
            }else{
                transMatrix[i][j] = 0;
            }
        }
    }
}
