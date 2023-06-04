#ifndef _COMBAT_H_
#define _COMBAT_H_

typedef struct state State;
typedef struct map Map;
typedef struct projectile Projectile;

#define MAX_PROJECTILES 20

void throwRock(State *st);
void throwProjectile(State *st);
void drawProjectiles(Map *map);
void updateProjectiles(State *st);
void freeProjectile(void *p);
void killMonster(State *st, int x, int y);

#endif
