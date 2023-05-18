#include <stdio.h>
#include <stdlib.h>

typedef struct monster {
    int x;
    int y;
    int hp;
    int attack;
    int defense;
    int speed;
    int is_alive;
    char symbol;
    char *name;
    // Item drop;
} Monster;

/**
 * @brief Initialize a new monster
 * 
 * @param x
 * @param y 
 * @param hp 
 * @param attack 
 * @param defense 
 * @param speed 
 * @param symbol 
 * @param name 
 * @return Monster*
 */
Monster *initMonster(int x, int y, int hp, int attack, int defense, int speed, char symbol, char *name) {
    Monster *monster = (Monster *) malloc(sizeof(struct monster));
    monster->x = x;
    monster->y = y;
    monster->hp = hp;
    monster->attack = attack;
    monster->defense = defense;
    monster->speed = speed;
    monster->is_alive = 1;
    monster->symbol = symbol;
    monster->name = name;
    return monster;
}

/**
 * @brief Initialize a new monster: rat
 * 
 * @param x 
 * @param y 
 * @return Monster*
 */
/* Monster *initRat(int x, int y) {
    return initMonster(x, y, 10, 1, 1, 1, 'r', "rat");
} */
