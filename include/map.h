#ifndef _MAP_H_
#define _MAP_H_

typedef struct cell Cell;
typedef struct state State;

typedef struct map {
    int width; // nCols
    int height; // nRows
    Cell ***cells; // 2D array of Cells
} Map;

Map *initMap(int width, int height);
void freeMap(void *p);

void drawMap(Map *map, int mode);

//void generateMap(Map *map, int *x, int *y);
void generateMap(State *st);
void getPlayerInitialPosition(Map *map, int *x, int *y);
void calculateDistances(Map* map, int x, int y);
void calculateVision(Map* map, int x, int y);

#endif
