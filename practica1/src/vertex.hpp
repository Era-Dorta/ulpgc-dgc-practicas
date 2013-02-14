#pragma once

#ifndef VERTEX_H
#define VERTEX_H

class Vertex {

    private:
        double coordinates[4];

    public:
        Vertex() { coordinates[0] = 0; coordinates[1] = 0; coordinates[2] = 0; coordinates[3] = 1; };
        Vertex( const double x, const double y, const double z );
        Vertex( const Vertex& otherVertex );
        double getX() const;
        double getY() const;
        double getZ() const;
        double getH() const { return coordinates[3]; };
        double get(int pos) const { return coordinates[pos]; };
        void setX( double x ) { coordinates[0] = x; };
        void setY( double y ) { coordinates[1] = y; };
        void setZ( double z ) { coordinates[2] = z; };
        void setH( double h ) { coordinates[3] = h; };
        void set( int pos, double val );
        void set (double x, double y, double z){ coordinates[0] = x; coordinates[1] = y; coordinates[2] = z;};
        bool operator==( const Vertex &otherVertex );
        Vertex operator*( const double matrix[4][4] );
        void operator=( const Vertex &otherVertex );
};

typedef struct Vertex Vertex;

//Center of the screen
static Vertex center = Vertex(512,384,0);
#endif
// VERTEX_H
