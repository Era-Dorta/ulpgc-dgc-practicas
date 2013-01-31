#include "testApp.h"

//--------------------------------------------------------------
Cube::Cube(){
    for( int i = 0; i < 0; i++){
        vertices[i].x = 0;
        vertices[i].y = 0;
        vertices[i].z = 0;
    }
}

//--------------------------------------------------------------
Cube::Cube( Vertex vertex0, Vertex vertex1 ){
    setVertices(vertex0, vertex1);
}

//--------------------------------------------------------------
void Cube::setVertices( Vertex vertex0, Vertex vertex1 ){
    int side, z;
    side = abs(vertex0.x - vertex1.x);

    for(z = 0; z < 1; z++){
        vertices[0 + z*4].x = vertex0.x;
        vertices[0 + z*4].y = vertex0.y;
        vertices[0 + z*4].z = vertex0.z + z*side;

        vertices[1 + z*4].x = vertex0.x + side;
        vertices[1 + z*4].y = vertex0.y;
        vertices[1 + z*4].z = vertex0.z + z*side;

        vertices[2 + z*4].x = vertex0.x + side;
        vertices[2 + z*4].y = vertex0.y + side;
        vertices[2 + z*4].z = vertex0.z + z*side;

        vertices[3 + z*4].x = vertex0.x;
        vertices[3 + z*4].y = vertex0.y + side;
        vertices[3 + z*4].z = vertex0.z + z*side;


    }
}

//--------------------------------------------------------------
void Cube::draw(){
    for(int i = 0; i < 4; i++){
        ofLine(vertices[i].x, vertices[i].y, vertices[(i+1)%4].x, vertices[(i+1)%4].y);
    }
}

//--------------------------------------------------------------
void testApp::setup(){
}

//--------------------------------------------------------------
void testApp::update(){
}

//--------------------------------------------------------------
void testApp::draw(){
    cube.draw();
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

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    if( button == L_MOUSE){
        pmouse.x = x;
        pmouse.y = y;
        pmouse.z = 0;
    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    if( button == L_MOUSE){
        Vertex current(x, y, 0);
        cube.setVertices( pmouse, current );
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
