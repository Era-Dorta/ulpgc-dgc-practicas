#include "testApp.h"

//Center of the screen
Vertex center;
int Vertex::draw = 0;
int Vertex::perpective = 0;
int k = 200;
static float transMatrix[4][4];
//--------------------------------------------------------------
float Vertex::getX() const {
    if( this == &center ){
        return x;
    }else{
        if( perpective ){
            return ( x / ( 1 - getZ() / (float)k ) + center.getX()*draw );
        }else{
            return (x + center.getX()*draw);
        }
    }
}

//--------------------------------------------------------------
float Vertex::getY() const {
    if( this == &center ){
        return y;
    }else{
        if( perpective ){
            return ( y / ( 1 - getZ() / (float)k ) + center.getY()*draw );
        }else{
            return (y + center.getY()*draw);
        }
    }
}

//--------------------------------------------------------------
float Vertex::getZ() const {
    if( this == &center ){
        return z;
    }else{
        return (z + center.getZ()*draw);
    }
}

//--------------------------------------------------------------
void Vertex::set( int pos, float val ){
    switch(pos){
    case 0:
        setX(val);
        break;
    case 1:
        setY(val);
        break;
    case 2:
        setZ(val);
        break;
    default:
        setH(val);
        break;
    }
}

//--------------------------------------------------------------
bool Vertex::operator==( const Vertex &otherVertex ){
    return ( getX() ==  otherVertex.getX() && getY() == otherVertex.getY() );
}

//--------------------------------------------------------------
void Vertex::operator=( const Vertex  &otherVertex ){
    setX(otherVertex.getX());
    setY(otherVertex.getY());
    setZ(otherVertex.getZ());
}

//--------------------------------------------------------------
Vertex Vertex::operator*( const float matrix[4][4] ){
    Vertex vRes;
    float vAux[4];
    vAux[0] = getX();
    vAux[1] = getY();
    vAux[2] = getZ();
    vAux[3] = getH();
    for(int i = 0; i < 4; i++){
        float res = 0;
        for( int j = 0; j < 4; j++){
            res += matrix[j][i]*vAux[i];
        }
        vRes.set(i,res);
    }
    return vRes;
}

//--------------------------------------------------------------
Cube::Cube(){
    for( int i = 0; i < 0; i++){
        vertices[i].setX( 0 );
        vertices[i].setY( 0 );
        vertices[i].setZ( 0 );
    }
}

//--------------------------------------------------------------
Cube::Cube( Vertex vertex0, Vertex vertex1 ){
    setVertices(vertex0, vertex1);
}

//--------------------------------------------------------------
void Cube::setVertices( Vertex vertex0, Vertex vertex1 ){
    float sideX, sideY, sideZ;
    int z;
    sideX = vertex1.getX() - vertex0.getX();
    sideY = vertex1.getY() - vertex0.getY();
    //Depth of the cube is length of side x
    sideZ = abs(sideX);

    for(z = 0; z < 2; z++){
        vertices[0 + z*4].setX( vertex0.getX() );
        vertices[0 + z*4].setY( vertex0.getY() );
        vertices[0 + z*4].setZ( vertex0.getZ() - z*sideZ );

        vertices[1 + z*4].setX( vertex0.getX() + sideX );
        vertices[1 + z*4].setY( vertex0.getY() );
        vertices[1 + z*4].setZ( vertex0.getZ() - z*sideZ );

        vertices[2 + z*4].setX( vertex0.getX() + sideX );
        vertices[2 + z*4].setY( vertex0.getY() + sideY );
        vertices[2 + z*4].setZ( vertex0.getZ() - z*sideZ );

        vertices[3 + z*4].setX( vertex0.getX() );
        vertices[3 + z*4].setY( vertex0.getY() + sideY );
        vertices[3 + z*4].setZ( vertex0.getZ() - z*sideZ );
    }
    for( int i = 0; i < 8; i++){
        transVertices[i] = vertices[i]*transMatrix;
    }
}

//--------------------------------------------------------------
void Cube::draw(){
    vertices[0].drawing();
    vertices[0].withPerpective();
    for(int i = 0; i < 4; i++){
        //Back face of the cube
        ofSetColor ( 0 ,255 ,0 ); //Green
        ofLine(vertices[i + 4].getX(), vertices[i + 4].getY(), vertices[(i+1)%4 + 4].getX(), vertices[(i+1)%4 + 4].getY());
        //Lines between the two faces
        ofSetColor ( 0 ,0 ,255 ); //Blue
        ofLine(vertices[i].getX(), vertices[i].getY(), vertices[i + 4].getX(), vertices[i + 4].getY());
        //Front face of the cube
        ofSetColor ( 255 ,0 ,0 ); //Red
        ofLine(vertices[i].getX(), vertices[i].getY(), vertices[(i+1)%4].getX(), vertices[(i+1)%4].getY());
    }
    //getc(stdin);
    vertices[0].withoutPerpective();
    vertices[0].notDrawing();
}

//--------------------------------------------------------------
void Cube::resetMatrix(){
    //Set transformation matrix to its initial state
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

//--------------------------------------------------------------
void Cube::multiplyMatrix( float matrix[4][4] ){

    float aux[4][4];
    for( int i = 0; i < 4; i++){
        for( int j = 0; j < 4; j++){
            aux[i][j] = 0;
        }
    }

    for( int i = 0; i < 4; i++){
        for( int j = 0; j < 4; j++){
            for( int k = 0; k < 4; k++){
                aux[i][j] += transMatrix[i][k]*matrix[k][j];
            }
        }
    }

    for( int i = 0; i < 4; i++){
        for( int j = 0; j < 4; j++){
            transMatrix[i][k] = aux[i][j];
        }
    }
}

//--------------------------------------------------------------
void Cube::rotate( int axis, int amount){
    float aux[4][4], cosVal, sinVal;
    for( int i = 0; i < 4; i++){
        for( int j = 0; j < 4; j++){
            if(i == j){
                aux[i][j] = 1;
            }else{
                aux[i][j] = 0;
            }
        }
    }
    amount = 0.01*amount;
    cosVal = cos(amount);
    sinVal = sin(amount);
    aux[1][1] = cosVal;
    aux[2][2] = cosVal;
    aux[1][2] = sinVal;
    aux[2][1] = -sinVal;
    multiplyMatrix(aux);
}

//--------------------------------------------------------------
Button::Button( testApp *app_, Vertex vertex, string buttonTex_ ){
    pressed = false;
    size = 50;
    buttonTex = buttonTex_;
    app = app_;
    vertices[0] = *(new Vertex( vertex.getX(), vertex.getY(), vertex.getZ()));
    vertices[1] = *(new Vertex( vertex.getX() + size*2, vertex.getY(), vertex.getZ()));
    vertices[2] = *(new Vertex( vertex.getX() + size*2, vertex.getY() + size, vertex.getZ()));
    vertices[3] = *(new Vertex( vertex.getX(), vertex.getY() + size, vertex.getZ()));
}

//--------------------------------------------------------------
void Button::checkPress( Vertex mouse ){
    if( mouse.getX() >= vertices[0].getX() &&  mouse.getX() <= vertices[1].getX() &&
        mouse.getY() >= vertices[0].getY() && mouse.getY() <= vertices[2].getY() ){
        if( pressed ){
            pressed = false;
        }else{
            pressed = true;
            app->setState( ROTATING );
        }
    }
}

//--------------------------------------------------------------
bool Button::isPressed(){
    return pressed;
}

//--------------------------------------------------------------
void Button::draw(){
    if( pressed ){
        ofSetColor ( 0 ,0 ,255 ); //Blue
    }else{
        ofSetColor ( 255 ,0 ,0 ); //Red
    }
    vertices[0].drawing();
    for(int i = 0; i < 4; i++){
        ofLine(vertices[i].getX(), vertices[i].getY(), vertices[(i + 1)%4].getX(), vertices[(i + 1)%4].getY());
    }
    ofDrawBitmapString(buttonTex, vertices[3].getX() + size/2, vertices[3].getY() - size/2);
    vertices[0].notDrawing();
}

//--------------------------------------------------------------
void testApp::setup(){
    center = Vertex(512,384,0);
    state = DRAWING;
    //Initialize matrix
    resetMatrix();
    //Create buttons
    Vertex auxVertex(200,-300,0);
    Button auxButton(this, auxVertex, "Rotate");
    buttonList.push_back( auxButton );
}

//--------------------------------------------------------------
void testApp::update(){
}

//--------------------------------------------------------------
void testApp::draw(){
    cube.draw();
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
    //Force 0,0 at the center of the screen
    x = x - center.getX();
    y = y - center.getY();
    if( button == L_MOUSE){
        Vertex current(x, y, 0);
        switch(state){
        case ROTATING:
            cube.rotate( X, pmouse.getX() - current.getX() );
            break;
        case TRANSLATING:
            break;
        case DRAWING:
            cube.setVertices( pmouse, current );
            break;
        }
    }
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    //Force 0,0 at the center of the screen
    x = x - center.getX();
    y = y - center.getY();
    if( button == L_MOUSE){
        pmouse.setX( x );
        pmouse.setY( y );
        pmouse.setZ( 0 );

        switch(state){
        case ROTATING:
            break;
        case TRANSLATING:
            break;
        case DRAWING:
            break;
        }
        //Check buttons
        for( unsigned int i = 0; i < buttonList.size(); i++){
            buttonList[i].checkPress(pmouse);
        }
    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    //Force 0,0 at the center of the screen
    x = x - center.getX();
    y = y - center.getY();
    if( button == L_MOUSE){
        Vertex current(x, y, 0);
        switch(state){
        case ROTATING:
            cube.rotate( X, pmouse.getX() - current.getX() );
            break;
        case TRANSLATING:
            break;
        case DRAWING:
            cube.setVertices( pmouse, current );
            break;
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
void testApp::setState( int state_ ){
    state = state_;
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
