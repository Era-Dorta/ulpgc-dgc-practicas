#include "testApp.hpp"

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
    state = DRAWING;
    //Initialize matrix
    resetMatrix();
    //Create buttons
    int x = 400, y = -300;
    Vertex* auxVertex;
    string buttonNames[6] = { "Rotate X", "Rotate Y", "Rotate Z", "Rotate", "Translate", "Draw" };
    Button* auxButton;
    for(int i = 0; i < 6; i++){
        auxVertex = new Vertex( x, y + i*60, 0 );
        auxButton = new Button(this, *auxVertex, buttonNames[i], (AppStates)i);
        buttonList.push_back( *auxButton );
    }
    //This lines make draw button, think it was pressed
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

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

    if( button == L_MOUSE){
        if(opReady){
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
            case DRAWING:
                //Force 0,0 at the center of the screen
                x = x - center.getX();
                y = y - center.getY();
                Vertex current(x, y, 0);
                objectList.back()->setVertices( pmouse, current );
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

        if( state == DRAWING ){
            objectList.push_back( new Cube() );
        }
    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

    if( button == L_MOUSE){
        if(opReady){
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
            case DRAWING:
                //Force 0,0 at the center of the screen
                x = x - center.getX();
                y = y - center.getY();
                Vertex current(x, y, 0);
                objectList.back()->setVertices( pmouse, current );
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
