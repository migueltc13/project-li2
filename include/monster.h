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
 * @param health Monster current hit points
 * @param max_health Monster maximum hit points
 * @param attack Monster attack points
 * @param defense Monster defense points
 * @param is_alive Is the Monster is alive?
 * @param symbol Monster symbol
 * @param name Monster name
 * @param color The current color of the monster is affected by bombs
 * @param default_color The default color of the monster
 * @param pathfinding pathfinding type of the Monster
 * @param gold Monster gold value when killed
 * @param noise Monster noise
 * @param index Monster index in the monsters array also stored in the cell (used as a pointer)
 */
typedef struct monster {
    unsigned int x;
    unsigned int y;
    int health;
    int max_health;
    int attack;
    int defense;
    // int speed;
    int is_alive;
    char symbol;
    char *name;
    int color;
    int default_color;
    int pathfinding; // 0: random, 1: searching, 2: towards player, 3: away from player, 4: recruiting
    int gold;
    const char *noise;
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
#define RAT_HEALTH 20
#define RAT_ATTACK 3
#define RAT_DEFENSE 0
#define RAT_SYMBOL 'r'
#define RAT_COLOR COLOR_GREEN
#define RAT_GOLD_MIN 1 // * 10
#define RAT_GOLD_MAX 2 // * 10
#define RAT_NOISE "squeaks and sniffles"

// Goblin
#define GOBLIN_HEALTH 30
#define GOBLIN_ATTACK 5
#define GOBLIN_DEFENSE 10
#define GOBLIN_SYMBOL 'g'
#define GOBLIN_COLOR COLOR_MAGENTA
#define GOBLIN_GOLD_MIN 1
#define GOBLIN_GOLD_MAX 3
#define GOBLIN_NOISE "robbing noises"

// Orc
#define ORC_HEALTH 40
#define ORC_ATTACK 8
#define ORC_DEFENSE 20
#define ORC_SYMBOL 'o'
#define ORC_COLOR COLOR_RED
#define ORC_GOLD_MIN 2
#define ORC_GOLD_MAX 4
#define ORC_NOISE "grunts and growls"

// Troll
#define TROLL_HEALTH 50
#define TROLL_ATTACK 10
#define TROLL_DEFENSE 30
#define TROLL_SYMBOL 't'
#define TROLL_COLOR COLOR_YELLOW
#define TROLL_GOLD_MIN 3
#define TROLL_GOLD_MAX 4
#define TROLL_NOISE "laughs and grunts"

// Dragon
#define DRAGON_HEALTH 60
#define DRAGON_ATTACK 20
#define DRAGON_DEFENSE 50
#define DRAGON_SYMBOL 'd'
#define DRAGON_COLOR COLOR_BLUE
#define DRAGON_GOLD_MIN 4
#define DRAGON_GOLD_MAX 5
#define DRAGON_NOISE "roars and growls"

Monster *initMonster(int x, int y, int health, int attack, int defense, char symbol, char *name, int color, int pathfinding, int gold, const char *noise, int index);
void moveMonster(State *st, Monster* monster, int x, int y);
// void drawMonster(State *st, Monster *monster, int mode);
void drawMonsters(State* st);
void freeMonster(void *p);
// TODO void freeMonsters(Monster **monsters, int nMonsters);

Monster **generateMonsters(Map* map);
void moveMonsters(State* st, Monster **monsters, int nMonsters);

#endif
