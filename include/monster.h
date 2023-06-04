#ifndef _MONSTER_H_
#define _MONSTER_H_

typedef struct state State;
typedef struct map Map;

/** 
 * @brief Structure of a monster
 * 
 * @details Contains the monster's position, attributes, name, symbol and index.
 * 
 * @param x Monster x coordinate
 * @param y Monster y coordinate
 * @param health Monster hit points 
 * @param attack Monster attack points
 * @param defense Monster defense points
 * @param is_alive Is the Monster is alive?
 * @param symbol Monster symbol
 * @param name Monster name
 * @param color The color of the monster
 * @param pathfinding pathfinding type of the Monster
 * @param gold Monster gold value when killed
 * @param index Monster index in the monsters array also stored in the cell (used as a pointer)
 */
typedef struct monster {
    int x;
    int y;
    int health;
    int attack;
    int defense;
    // int speed;
    int is_alive;
    char symbol;
    char *name;
    int color;
    int pathfinding; // 0: random, 1: searching, 2: towards player, 3: away from player, 4: recruiting
    int gold;
    int index;
    // Item drop; // TODO
} Monster;

#define NUMBER_OF_MONSTERS 5

// Pathfinding types
#define PATHFINDING_RANDOM 0
#define PATHFINDING_SEARCHING 1
#define PATHFINDING_TOWARDS_PLAYER 2
#define PATHFINDING_AWAY_FROM_PLAYER 3
#define PATHFINDING_RECRUITING 4

// Rat
#define RAT_SYMBOL 'r'
#define RAT_COLOR COLOR_GREEN
#define RAT_GOLD_MIN 1 // * 10
#define RAT_GOLD_MAX 2 // * 10

// Goblin
#define GOBLIN_SYMBOL 'g'
#define GOBLIN_COLOR COLOR_MAGENTA
#define GOBLIN_GOLD_MIN 1
#define GOBLIN_GOLD_MAX 3

// Orc
#define ORC_SYMBOL 'o'
#define ORC_COLOR COLOR_RED
#define ORC_GOLD_MIN 2
#define ORC_GOLD_MAX 4

// Troll
#define TROLL_SYMBOL 't'
#define TROLL_COLOR COLOR_YELLOW
#define TROLL_GOLD_MIN 3
#define TROLL_GOLD_MAX 4

// Dragon
#define DRAGON_SYMBOL 'd'
#define DRAGON_COLOR COLOR_BLUE
#define DRAGON_GOLD_MIN 4
#define DRAGON_GOLD_MAX 5

Monster *initMonster(int x, int y, int health, int attack, int defense, char symbol, char *name, int color, int pathfinding, int gold, int index);
void moveMonster(State *st, Monster* monster, int x, int y);
// void drawMonster(State *st, Monster *monster, int mode);
void drawMonsters(State* st);
void freeMonster(void *p);
// TODO void freeMonsters(Monster **monsters, int nMonsters);

Monster **generateMonsters(Map* map);
void moveMonsters(State* st, Monster **monsters, int nMonsters);

#endif
