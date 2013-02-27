#include "vertex.hpp"
#include <iostream>

//--------------------------------------------------------------
Vertex::Vertex( const float x, const float y, const float z ) {
    coordinates[0] = x;
    coordinates[1] = y;
    coordinates[2] = z;
    coordinates[3] = 1;
}

//--------------------------------------------------------------
Vertex::Vertex( const Vertex& otherVertex ){
    for(int i = 0; i < 4; i++){
        coordinates[i] = otherVertex.coordinates[i];
    }
}

//--------------------------------------------------------------
bool Vertex::compareY (const Vertex& a, const Vertex& b)
{
  return a.getY() > b.getY();
}

//--------------------------------------------------------------
float Vertex::getX() const {
    return coordinates[0];
}

//--------------------------------------------------------------
float Vertex::getY() const {
    return coordinates[1];
}

//--------------------------------------------------------------
float Vertex::getZ() const {
    return coordinates[2];
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
float Vertex::module(){
    float res = 0;
    for(int i = 0; i < 3; i++){
        res += coordinates[i]*coordinates[i];
    }
    return sqrt(res);
}

//--------------------------------------------------------------
void Vertex::normalize(){
    float mod = module();
    for(int i = 0; i < 3; i++){
        coordinates[i] = coordinates[i]/mod;
    }
}

//--------------------------------------------------------------
bool Vertex::operator==( const Vertex &otherVertex ){
    return ( getX() ==  otherVertex.getX() && getY() == otherVertex.getY() && getZ() == otherVertex.getZ());
}

//--------------------------------------------------------------
void Vertex::operator=( const Vertex  &otherVertex ){
    for(int i = 0; i < 4; i++){
        coordinates[i] = otherVertex.coordinates[i];
    }
}

//--------------------------------------------------------------
Vertex Vertex::operator*( const float matrix[4][4] ){
    Vertex vRes;
    for(int i = 0; i < 4; i++){
        float res = 0;
        for( int j = 0; j < 4; j++){
            res += coordinates[j]*matrix[j][i];
        }
        vRes.set(i,res);
    }
    return vRes;
}

//--------------------------------------------------------------
Vertex Vertex::operator*( const Vertex &otherVertex ){
    Vertex vRes;
    vRes.setX( coordinates[1]*otherVertex.coordinates[2] - coordinates[2]*otherVertex.coordinates[1] );
    vRes.setY( coordinates[2]*otherVertex.coordinates[0] - coordinates[0]*otherVertex.coordinates[2] );
    vRes.setZ( coordinates[0]*otherVertex.coordinates[1] - coordinates[1]*otherVertex.coordinates[0] );
    return vRes;
}

//--------------------------------------------------------------
Vertex Vertex::operator-( const Vertex &otherVertex ){
    Vertex vRes;
    for(int i = 0; i < 3; i++){
        vRes.set(i, coordinates[i] - otherVertex.coordinates[i] );
    }
    return vRes;
}

//--------------------------------------------------------------
Vertex Vertex::operator+( const Vertex &otherVertex ){
    Vertex vRes;
    for(int i = 0; i < 3; i++){
        vRes.set(i, coordinates[i] + otherVertex.coordinates[i] );
    }
    return vRes;
}

//--------------------------------------------------------------
Vertex Vertex::operator/( const float &val ){
    Vertex vRes;
    for(int i = 0; i < 3; i++){
        vRes.set(i, coordinates[i] / val );
    }
    return vRes;
}

//--------------------------------------------------------------
Vertex Vertex::operator*( const float &val ){
    Vertex vRes;
    for(int i = 0; i < 3; i++){
        vRes.set(i, coordinates[i] * val );
    }
    return vRes;
}
