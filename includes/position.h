#ifndef _POSITION_H_
#define _POSITION_H_

// Struct that represents a (x,y) position
typedef struct position {
    int x;
    int y;
} *Position;

Position newPosition(int x, int y);
void freePosition(Position p);

#endif
