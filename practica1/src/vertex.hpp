#pragma once

#ifndef VERTEX_H
#define VERTEX_H

#include <cmath>

class Vertex {

    private:
        float coordinates[4];

    public:
        Vertex() { coordinates[0] = 0; coordinates[1] = 0; coordinates[2] = 0; coordinates[3] = 1; };
        Vertex( const float x, const float y, const float z );
        Vertex( const Vertex& otherVertex );
        static bool compareY (const Vertex& a, const Vertex& b);
        float getX() const;
        float getY() const;
        float getZ() const;
        float getH() const { return coordinates[3]; };
        float get(int pos) const { return coordinates[pos]; };
        void setX( float x ) { coordinates[0] = x; };
        void setY( float y ) { coordinates[1] = y; };
        void setZ( float z ) { coordinates[2] = z; };
        void setH( float h ) { coordinates[3] = h; };
        void set( int pos, float val );
        void set (float x, float y, float z){ coordinates[0] = x; coordinates[1] = y; coordinates[2] = z;};
        float module();
        void normalize();
        bool operator==( const Vertex &otherVertex );
        Vertex operator*( const float matrix[4][4] );
        Vertex operator*( const Vertex &otherVertex );
        void operator=( const Vertex &otherVertex );
        Vertex operator-( const Vertex &otherVertex );
        Vertex operator+( const Vertex &otherVertex );
        Vertex operator/( const float &val );
        Vertex operator*( const float &val );

};

typedef struct Vertex Vertex;

//Center of the screen
static Vertex center = Vertex(512,384,0);
#endif
// VERTEX_H
