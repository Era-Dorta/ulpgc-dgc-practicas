#include "vertex.hpp"
#include <iostream>
int Vertex::draw = 0;
int Vertex::perpective = 0;
int k = 400;

//--------------------------------------------------------------
Vertex::Vertex( const double x, const double y, const double z ) {
    coordinates[0] = x;
    coordinates[1] = y;
    coordinates[2] = z;
    coordinates[3] = 1;
    draw = 0;
}

//--------------------------------------------------------------
Vertex::Vertex( const Vertex& otherVertex ){
    for(int i = 0; i < 4; i++){
        coordinates[i] = otherVertex.coordinates[i];
    }
}

//--------------------------------------------------------------
double Vertex::getX() const {
    if( this == &center ){
        return coordinates[0];
    }else{
        if( perpective ){
            return ( coordinates[0] / ( 1 - getZ() / (double)k ) + center.getX()*draw );
        }else{
            return (coordinates[0] + center.getX()*draw);
        }
    }
}

//--------------------------------------------------------------
double Vertex::getY() const {
    if( this == &center ){
        return coordinates[1];
    }else{
        if( perpective ){
            return ( coordinates[1] / ( 1 - getZ() / (double)k ) + center.getY()*draw );
        }else{
            return (coordinates[1] + center.getY()*draw);
        }
    }
}

//--------------------------------------------------------------
double Vertex::getZ() const {
    if( this == &center ){
        return coordinates[2];
    }else{
        return (coordinates[2] + center.getZ()*draw);
    }
}

//--------------------------------------------------------------
void Vertex::set( int pos, double val ){
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
    for(int i = 0; i < 4; i++){
        coordinates[i] = otherVertex.coordinates[i];
    }
}

//--------------------------------------------------------------
Vertex Vertex::operator*( const double matrix[4][4] ){
    Vertex vRes;
    for(int i = 0; i < 4; i++){
        double res = 0;
        for( int j = 0; j < 4; j++){
            res += coordinates[j]*matrix[j][i];
        }
        vRes.set(i,res);
    }
    return vRes;
}
