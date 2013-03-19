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
    coordinates[0] = otherVertex.coordinates[0];
    coordinates[1] = otherVertex.coordinates[1];
    coordinates[2] = otherVertex.coordinates[2];
    coordinates[3] = otherVertex.coordinates[3];
}

//--------------------------------------------------------------
bool Vertex::compareY (const Vertex& a, const Vertex& b)
{
  return a.getY() > b.getY();
}

//--------------------------------------------------------------
bool Vertex::compareYX (const Vertex& a, const Vertex& b)
{
  if(a.getY() == b.getY()){
    return a.getX() < b.getX();
  } else{
    return a.getY() < b.getY();
  }
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
void Vertex::set( const int pos, const float val ){
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
void Vertex::checkH(){
    float aux = 1/coordinates[3];
    coordinates[0] *= aux;
    coordinates[1] *= aux;
    coordinates[2] *= aux;
    coordinates[3] = 1;
}

//--------------------------------------------------------------
float Vertex::module() const{
    float res = 0;
    for(int i = 0; i < 3; i++){
        res += coordinates[i]*coordinates[i];
    }
    return sqrt(res);
}

//--------------------------------------------------------------
void Vertex::normalize(){
    float mod = 1.0/module();
    for(int i = 0; i < 3; i++){
        coordinates[i] = coordinates[i]*mod;
    }
}

//--------------------------------------------------------------
bool Vertex::operator==( const Vertex &otherVertex ) const{
    return ( getX() ==  otherVertex.getX() && getY() == otherVertex.getY() && getZ() == otherVertex.getZ());
}

//--------------------------------------------------------------
void Vertex::operator=( const Vertex  &otherVertex ){
    for(int i = 0; i < 4; i++){
        coordinates[i] = otherVertex.coordinates[i];
    }
}

//--------------------------------------------------------------
float Vertex::dot( const Vertex &otherVertex ) const{
    return (coordinates[0]*otherVertex.coordinates[0] +
        coordinates[1]*otherVertex.coordinates[1] +
        coordinates[2]*otherVertex.coordinates[2]);
}

//--------------------------------------------------------------
float Vertex::distance( const Vertex &otherVertex ) const{
    return sqrt( (coordinates[0] - otherVertex.coordinates[0])*(coordinates[0] - otherVertex.coordinates[0]) +
                (coordinates[1] - otherVertex.coordinates[1])*(coordinates[1] - otherVertex.coordinates[1]) +
                (coordinates[2] - otherVertex.coordinates[2])*(coordinates[2] - otherVertex.coordinates[2])
                 );
}

//--------------------------------------------------------------
Vertex Vertex::operator*( const float matrix[4][4] ) const{
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
Vertex Vertex::operator*( const Vertex &otherVertex ) const{
    Vertex vRes;
    vRes.setX( coordinates[1]*otherVertex.coordinates[2] - coordinates[2]*otherVertex.coordinates[1] );
    vRes.setY( coordinates[2]*otherVertex.coordinates[0] - coordinates[0]*otherVertex.coordinates[2] );
    vRes.setZ( coordinates[0]*otherVertex.coordinates[1] - coordinates[1]*otherVertex.coordinates[0] );
    return vRes;
}

//--------------------------------------------------------------
Vertex Vertex::operator-( const Vertex &otherVertex ) const{
    Vertex vRes;
    for(int i = 0; i < 3; i++){
        vRes.set(i, coordinates[i] - otherVertex.coordinates[i] );
    }
    return vRes;
}

//--------------------------------------------------------------
Vertex Vertex::operator+( const Vertex &otherVertex ) const{
    Vertex vRes;
    for(int i = 0; i < 3; i++){
        vRes.set(i, coordinates[i] + otherVertex.coordinates[i] );
    }
    return vRes;
}

//--------------------------------------------------------------
Vertex Vertex::operator/( const float &val ) const{
    Vertex vRes;
    for(int i = 0; i < 3; i++){
        vRes.set(i, coordinates[i] / val );
    }
    return vRes;
}

//--------------------------------------------------------------
Vertex Vertex::operator*( const float &val ) const{
    Vertex vRes;
    for(int i = 0; i < 3; i++){
        vRes.set(i, coordinates[i] * val );
    }
    return vRes;
}

//--------------------------------------------------------------
ostream& operator<< (ostream &out, const Vertex &vertex){
    out << "v: " << vertex.getX() << ", " << vertex.getY() << ", " << vertex.getZ();
    return out;

}

