#include "testApp.h"

//Center of the screen
Vertex center;
int Vertex::draw = 0;
int Vertex::perpective = 0;
int k = 200;
static float transMatrix[4][4];

void showMatrix( const float matrix[4][4])
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
    resetTransMatrix();
    resetAuxMatrix();
}

//--------------------------------------------------------------
Cube::Cube( Vertex vertex0, Vertex vertex1 ){
    setVertices(vertex0, vertex1);
    resetTransMatrix();
    resetAuxMatrix();
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
    transVertices[0].drawing();
    transVertices[0].withPerpective();
    for(int i = 0; i < 4; i++){
        //Back face of the cube
        ofSetColor ( 0 ,255 ,0 ); //Green
        ofLine(transVertices[i + 4].getX(), transVertices[i + 4].getY(), transVertices[(i+1)%4 + 4].getX(), transVertices[(i+1)%4 + 4].getY());
        //Lines between the two faces
        ofSetColor ( 0 ,0 ,255 ); //Blue
        ofLine(transVertices[i].getX(), transVertices[i].getY(), transVertices[i + 4].getX(), transVertices[i + 4].getY());
        //Front face of the cube
        ofSetColor ( 255 ,0 ,0 ); //Red
        ofLine(transVertices[i].getX(), transVertices[i].getY(), transVertices[(i+1)%4].getX(), transVertices[(i+1)%4].getY());
    }
    //getc(stdin);
    transVertices[0].withoutPerpective();
    transVertices[0].notDrawing();
}

//--------------------------------------------------------------
void Cube::resetTransMatrix(){
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
void Cube::resetAuxMatrix(){
    //Set transformation matrix to its initial state
    for( int i = 0; i < 4; i++){
        for( int j = 0; j < 4; j++){
            if(i == j){
                auxMatrix[i][j] = 1;
            }else{
                auxMatrix[i][j] = 0;
            }
        }
    }
}

//--------------------------------------------------------------
void Cube::multiplyMatrix( float matrix0[4][4], float matrix1[4][4], int firstSave ){

    float aux[4][4];
    for( int i = 0; i < 4; i++){
        for( int j = 0; j < 4; j++){
            aux[i][j] = 0;
        }
    }

    for( int i = 0; i < 4; i++){
        for( int j = 0; j < 4; j++){
            for( int k = 0; k < 4; k++){
                aux[i][j] += matrix0[i][k]*matrix1[k][j];
            }
        }
    }

    if(firstSave){
        for( int i = 0; i < 4; i++){
            for( int j = 0; j < 4; j++){
                matrix0[i][j] = aux[i][j];
            }
        }
    }else{
        for( int i = 0; i < 4; i++){
            for( int j = 0; j < 4; j++){
                matrix1[i][j] = aux[i][j];
            }
        }
    }
}

//--------------------------------------------------------------
void Cube::rotate( int axis, float amount, int permanent){
    float cosVal, sinVal;
    resetAuxMatrix();
    cout << "Rota antes" << amount << endl;
    amount = 0.02*amount;
    cout << "Rota despues" << amount << endl;
    cosVal = cos(amount);
    sinVal = sin(amount);
    auxMatrix[1][1] = cosVal;
    auxMatrix[2][2] = cosVal;
    auxMatrix[1][2] = sinVal;
    auxMatrix[2][1] = -sinVal;
    showMatrix(auxMatrix);
    if(permanent){
        multiplyMatrix(transMatrix, auxMatrix);
        showMatrix(transMatrix);
        for( int i = 0; i < 8; i++){
            transVertices[i] = vertices[i]*transMatrix;
        }
        resetAuxMatrix();
    }else{
        multiplyMatrix(transMatrix, auxMatrix, 0);
        showMatrix(transMatrix);
        for( int i = 0; i < 8; i++){
            transVertices[i] = vertices[i]*auxMatrix;
        }
    }
}

//--------------------------------------------------------------
Button::Button( testApp *app_, Vertex vertex, string buttonTex_, AppStates state_ ){
    pressed = false;
    size = 50;
    buttonTex = buttonTex_;
    app = app_;
    state = state_;
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
            app->setState( state );
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
    ofDrawBitmapString(buttonTex, vertices[3].getX() + size/3, vertices[3].getY() - size/2);
    vertices[0].notDrawing();
}

//--------------------------------------------------------------
void testApp::setup(){
    center = Vertex(512,384,0);
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
        case ROTATING_X:
            cube.rotate( X, pmouse.getY() - current.getY(), 0 );
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
        case ROTATING_X:
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
        case ROTATING_X:
        cout << "Rotando" << endl;
            cube.rotate( X, pmouse.getY() - current.getY(), 1 );
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
