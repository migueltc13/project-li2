#ifndef _MAP_H_
#define _MAP_H_

typedef struct map Map;

Map *initMap(int width, int height);
void freeMap(void *p);
void generateMap(Map *map);
void drawMap(Map *map);

#endif