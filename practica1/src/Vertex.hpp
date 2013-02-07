#pragma once

#ifndef VERTEX_H
#define VERTEX_H

class Vertex {
    private:

    double x;
    double y;
    double z;
    double h;
    static int draw;
    static int perpective;

    public:

    Vertex() { x = 0; y = 0; z = 0; h = 1; draw = 0; };
    Vertex( double x_, double y_, double z_ ) { x = x_; y = y_; z = z_; h = 1; draw = 0; };
    void drawing() { draw = 1; };
    void notDrawing() { draw = 0; };
    void withPerpective() { perpective = 1; };
    void withoutPerpective() { perpective = 0; };
    double getX() const;
    double getY() const;
    double getZ() const;
    double getH() const { return h; };
    void setX( double x_ ) { x = x_; };
    void setY( double y_ ) { y = y_; };
    void setZ( double z_ ) { z = z_; };
    void setH( double h_ ) { h = h_; };
    void set( int pos, double val );
    bool operator==( const Vertex &otherVertex );
    Vertex operator*( const double matrix[4][4] );
    void operator=( const Vertex &otherVertex );
};

typedef struct Vertex Vertex;

//Center of the screen
static Vertex center = Vertex(512,384,0);
#endif
// VERTEX_H
