#include "Vertex.hpp"
#include <iostream>
int Vertex::draw = 0;
int Vertex::perpective = 0;
int k = 400;

//--------------------------------------------------------------
double Vertex::getX() const {
    if( this == &center ){
        return x;
    }else{
        if( perpective ){
            return ( x / ( 1 - getZ() / (double)k ) + center.getX()*draw );
        }else{
            return (x + center.getX()*draw);
        }
    }
}

//--------------------------------------------------------------
double Vertex::getY() const {
    if( this == &center ){
        return y;
    }else{
        if( perpective ){
            return ( y / ( 1 - getZ() / (double)k ) + center.getY()*draw );
        }else{
            return (y + center.getY()*draw);
        }
    }
}

//--------------------------------------------------------------
double Vertex::getZ() const {
    if( this == &center ){
        return z;
    }else{
        return (z + center.getZ()*draw);
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
    setX(otherVertex.getX());
    setY(otherVertex.getY());
    setZ(otherVertex.getZ());
}

//--------------------------------------------------------------
Vertex Vertex::operator*( const double matrix[4][4] ){
    Vertex vRes;
    double vAux[4];
    vAux[0] = getX();
    vAux[1] = getY();
    vAux[2] = getZ();
    vAux[3] = getH();
    for(int i = 0; i < 4; i++){
        double res = 0;
        for( int j = 0; j < 4; j++){
            res += matrix[j][i]*vAux[j];
        }
        vRes.set(i,res);
    }
    return vRes;
}
