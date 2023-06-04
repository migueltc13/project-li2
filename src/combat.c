#include <stdlib.h>
#include <ncurses.h>
#include "combat.h"
#include "state.h"
#include "map.h"
#include "cell.h"
#include "player.h"
#include "monster.h"
#include "item.h"
#include "inventory.h"
#include "menu.h"

/**
 * @brief (private) Structure of a thrown projectile
 * 
 * @param x The current x coordinate of the projectile
 * @param y The current y coordinate of the projectile
 * @param direction The ongoing direction of the projectile
 * @param turns_left The number of turns left for the projectile to disappear/have an effect
 * @param effect The effect of the projectile
 * @param damage The damage of the projectile
 * @param color The color of the projectile
*/
typedef struct projectile {
    int x;
    int y;
    int direction;
    int turns_left;
    int effect; // 0: damage, 2: smoke, 3: fire, 4: ice
    int damage;
    char symbol;
    int color;
} Projectile;

/**
 * @brief Initializes a new projectile
 * 
 * @param x The x coordinate of the projectile
 * @param y The y coordinate of the projectile
 * @param direction The direction of the projectile
 * @param range The range of the projectile
 * @param effect The effect of the projectile
 * @param damage The damage of the projectile
 * @param symbol The symbol of the projectile
 * @param color The color of the projectile
 * @return Projectile* The initialized projectile
 */
Projectile *initProjectile(int x, int y, int direction, int range, int effect, int damage, char symbol, int color) {
    Projectile *p = (Projectile *) malloc(sizeof(Projectile));
    p->x = x;
    p->y = y;
    p->direction = direction;
    p->turns_left = range;
    p->effect = effect;
    p->damage = damage;
    p->symbol = symbol;
    p->color = color;
    return p;
}

/**
 * @brief Moves a projectile. Reduces the number of turns left by 1
 * 
 * @param p The projectile to move
 * @return void
 */
void moveProjectile(Projectile *p) {
    switch (p->direction) {
        // TODO 8 directions numpad
        case 7: p->x--; p->y--; break;
        case 8:         p->y--; break;
        case 9: p->x++; p->y--; break;

        case 4: p->x--;         break;
        case 6: p->x++;         break;

        case 1: p->x--; p->y++; break;
        case 2:         p->y++; break;
        case 3: p->x++; p->y++; break;
    }
    p->turns_left--;
    return;
}

/**
 * @brief Inserts a projectile into the map
 * 
 * @param map The map to insert the projectile
 * @param p The projectile to insert
*/
void insertProjectile(Map *map, Projectile *p) {
    map->projectiles[map->nr_projectiles++] = p;
}

/**
 * @brief Removes a projectile from the map
 * 
 * @param map The map to remove the projectile
 * @param index The index of the projectile in the map
*/
void removeProjectile(Map *map, int index) {
    free(map->projectiles[index]);
    map->projectiles[index] = NULL; // unnecessary
    // shift left
    for (int i = index; i < map->nr_projectiles - 1; i++)
        map->projectiles[i] = map->projectiles[i + 1];
    map->nr_projectiles--;
}

/**
 * @brief Explodes a projectile
 * 
 * @details The explosion is a star shape with radius 5
 * Various effects are applied to the cells within the explosion
 * 
 * TODO: visual effects for monsters
 * 
 * @param st The game state
 * @param projectile The projectile to explode
 */
void explodeProjectile(State *st, Projectile *projectile) {
    for (int i = -5; i <= 5; i++) {
        for (int j = -5; j <= 5; j++) {
            if (i*i + j*j <= 25) {
                // Check coordinates dont go out of bounds
                if ((projectile->y + i < 0 || projectile->y + i >= st->map->height) ||
                    (projectile->x + j < 0 || projectile->x + j >= st->map->width)) break;
                // if (st->map->cells[projectile->y + i][projectile->x + j] == NULL) break; // unnecessary
                
                Cell *cell = st->map->cells[projectile->y + i][projectile->x + j];
                if (isCellWalkable(cell)) {
                    if (projectile->effect == SMOKE_BOMB_EFFECT) {
                        cell->is_visible = 0;
                        cell->was_visible = 0;
                        cell->color = projectile->color;
                        cell->effect = SMOKE_BOMB_EFFECT;
                        cell->effect_duration = SMOKE_BOMB_DURATION;
                    }
                    else if (projectile->effect == FIRE_BOMB_EFFECT) {
                        cell->is_visible = 1;
                        cell->was_visible = 1;
                        cell->color = projectile->color;
                        cell->effect = FIRE_BOMB_EFFECT;
                        cell->effect_duration = FIRE_BOMB_DURATION;
                    }
                    else if (projectile->effect == ICE_BOMB_EFFECT) {
                        cell->is_visible = 1;
                        cell->was_visible = 1;
                        cell->color = projectile->color;
                        cell->effect = ICE_BOMB_EFFECT;
                        cell->effect_duration = ICE_BOMB_DURATION;
                    }
                    if (isCellMonster(cell)) {
                        int monster_index = cell->monster_index;
                        Monster *monster = st->monsters[monster_index];

                        // TODO visual effect in monsters: drawMonster

                        // Update monster health with projectile damage
                        monster->health -= projectile->damage;

                        if (monster->health <= 0) {
                            // Send menu message: Monster name and gold value
                            char *msg = (char *) malloc(sizeof(char) * st->map->width);
                            snprintf(msg, st->map->width, "You killed a \"%s\" with explosion and earned %d gold!", monster->name, monster->gold);
                            sendMenuMessage(st, msg);
                            free(msg);

                            // Kill and remove the monster from the map
                            killMonster(st, monster->x, monster->y);
                        }
                    }
                }
            }
        }
    }
}

/**
 * @brief Updates a projectile within the game state
 * 
 * @param st The game state
 * @param projectile The projectile to update
 * @param index The index of the projectile in the map
 */
void updateProjectile(State *st, Projectile *projectile, int index) {
    if (projectile == NULL) return;
    if (projectile->turns_left == 0) {
        if (projectile->effect != 0) explodeProjectile(st, projectile);
        removeProjectile(st->map, index);
    }
    else { 
        if (isCellWalkable(st->map->cells[projectile->y][projectile->x])) {
            moveProjectile(projectile);
        }
        // check if projectile hits a monster
        if (isCellMonster(st->map->cells[projectile->y][projectile->x])) {
            int monster_index = st->map->cells[projectile->y][projectile->x]->monster_index;
            Monster *monster = st->monsters[monster_index];
            monster->health -= projectile->damage;
            if (monster->health <= 0) { // Monster died from the projectile            
                // Send menu message: Monster name and gold value
                char *msg = (char *) malloc(sizeof(char) * st->map->width);
                snprintf(msg, st->map->width, "You killed a \"%s\" with projectile and earned %d gold!", monster->name, monster->gold);
                sendMenuMessage(st, msg);
                free(msg);

                // Kill and remove the monster from the map
                killMonster(st, monster->x, monster->y);
            }
            else sendMenuMessage(st, "You hit a monster!");
            if (projectile->effect != 0) explodeProjectile(st, projectile);
            removeProjectile(st->map, index);
        }
        else if (!isCellWalkable(st->map->cells[projectile->y][projectile->x])) {
            if (projectile->effect != 0) explodeProjectile(st, projectile);
            removeProjectile(st->map, index);   
        }
    }
}

void updateProjectiles(State *st) {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (st->map->projectiles[i] == NULL) continue;
        else if (i > st->map->nr_projectiles) break;
        updateProjectile(st, st->map->projectiles[i], i);
    }
    // TODO iter map and monsters to clean effects  
}

void freeProjectile(void *p) {
    Projectile *projectile = (Projectile *) p;
    projectile->x = 0;
    projectile->y = 0;
    projectile->direction = 0;
    projectile->turns_left = 0;
    projectile->effect = 0;
    projectile->damage = 0;
    free(projectile);
}

/**
 * @brief Function to throw a rock projectile
 * 
 * @details The rock is thrown in the direction the player is facing
 * The rock is thrown until it hits a wall or a monster
 * Update projectile position with moveProjectiles function
 * 
 * @param st The game state
 * @return void
 */
void throwRock(State *st) {
    int damage = ROCK_DAMAGE_MIN + rand() % (ROCK_DAMAGE_MAX - ROCK_DAMAGE_MIN + 1);
    Projectile *projectile = initProjectile(st->player->x, st->player->y, st->player->direction, ROCK_RANGE, 0, damage, ROCK_SYMBOL, ROCK_COLOR);
    insertProjectile(st->map, projectile);
}

/**
 * @brief Function to throw a smoke bomb projectile
 * 
 * @details The smoke bomb is thrown in the direction the player is facing
 * The smoke bomb is thrown until it hits a wall or a monster
 * 
 * 
 * @param st The game state
 * @return void
 */
void throwSmokebomb(State *st) {
    int damage = SMOKE_BOMB_DAMAGE_MIN + rand() % (SMOKE_BOMB_DAMAGE_MAX - SMOKE_BOMB_DAMAGE_MIN + 1);
    // TODO #define SMOKE_BOMB_EFFECT 1
    Projectile *projectile = initProjectile(st->player->x, st->player->y, st->player->direction, SMOKE_BOMB_RANGE, 1, damage, SMOKE_BOMB_SYMBOL, SMOKE_BOMB_COLOR);
    insertProjectile(st->map, projectile);
}

/**
 * @brief Function to throw a fire bomb projectile
 * 
 * @details The fire bomb is thrown in the direction the player is facing
 * The fire bomb is thrown until it hits a wall or a monster
 * 
 * 
 * @param st The game state
 * @return void
 */
void throwFirebomb(State *st) {
    int damage = FIRE_BOMB_DAMAGE_MIN + rand() % (FIRE_BOMB_DAMAGE_MAX - FIRE_BOMB_DAMAGE_MIN + 1);
    // TODO #define FIRE_BOMB_EFFECT 2
    Projectile *projectile = initProjectile(st->player->x, st->player->y, st->player->direction, FIRE_BOMB_RANGE, 2, damage, FIRE_BOMB_SYMBOL, FIRE_BOMB_COLOR);
    insertProjectile(st->map, projectile);
}

/**
 * @brief Function to throw an ice bomb projectile
 * 
 * @details The ice bomb is thrown in the direction the player is facing
 * The ice bomb is thrown until it hits a wall or a monster
 * 
 * 
 * @param st The game state
 * @return void
 */
void throwIcebomb(State *st) {
    int damage = ICE_BOMB_DAMAGE_MIN + rand() % (ICE_BOMB_DAMAGE_MAX - ICE_BOMB_DAMAGE_MIN + 1);
    // TODO #define ICE_BOMB_EFFECT 3
    Projectile *projectile = initProjectile(st->player->x, st->player->y, st->player->direction, ICE_BOMB_RANGE, 3, damage, ICE_BOMB_SYMBOL, ICE_BOMB_COLOR);
    insertProjectile(st->map, projectile);
}

void throwProjectile(State *st) {
    Item *item = st->player->inventory->equipped_item;
    if (item == NULL) return;
    else if (item->symbol == ROCK_SYMBOL) throwRock(st);
    else if (item->symbol == SMOKE_BOMB_SYMBOL && item->color == SMOKE_BOMB_COLOR) throwSmokebomb(st);
    else if (item->symbol == FIRE_BOMB_SYMBOL  && item->color == FIRE_BOMB_COLOR) throwFirebomb(st);
    else if (item->symbol == ICE_BOMB_SYMBOL   && item->color == ICE_BOMB_COLOR) throwIcebomb(st);
}

/**
 * @brief Draw a projectile
 * 
 * @param p Projectile to draw
*/
void drawProjectile(Projectile *p) {
    attron(COLOR_PAIR(p->color));
    mvaddch(p->y, p->x, p->symbol);
    attroff(COLOR_PAIR(p->color));
}

/**
 * @brief Draw all visible projectiles
 * 
 * @param map Map to draw visible projectiles
*/
void drawProjectiles(Map *map) {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (i > map->nr_projectiles) break;
        if (map->projectiles[i] != NULL) drawProjectile(map->projectiles[i]);
        // note if (map->cells[map->projectiles[i]->y][map->projectiles[i]->x]->is/was_visible)
        // drawProjectile independently of the cell visibility (is/was_visible field)
    }
}

/**
 * @brief Function to kill a monster with his coordinates
 * 
 * @details The monster is removed from the map and the monsters array
 * 
 * @param st The game state
 * @param x The x coordinate of the monster
 * @param y The y coordinate of the monster
 * TODO: change to monster instead of coordinates
*/
void killMonster(State *st, int x, int y) {
    int monster_index = st->map->cells[y][x]->monster_index;
    Monster *monster = st->monsters[monster_index];
    
    // Set monster index in cell to -1 (unique value for no monster)
    st->map->cells[y][x]->monster_index = -1;
    
    // Add gold to player inventory
    st->player->inventory->gold += monster->gold;
    
    // Free monster
    freeMonster(monster);
    monster = NULL;

    // left shift monsters
    for (int i = monster_index; i < st->nMonsters - 1; i++) {
        st->monsters[i] = st->monsters[i + 1];
        
        // update monster->index
        st->monsters[i]->index--;

        // update monster_index in cell
        st->map->cells[st->monsters[i]->y][st->monsters[i]->x]->monster_index--;
    }
    st->nMonsters--;
}

// TODO
void monsterAttacksPlayer(State *st, Monster *monster) {
    
    st->player->health -= monster->attack; // TODO defense
    if (st->player->health <= 0) {
        // TODO game over function
        return;
    }

    monster->health -= st->player->attack; // TODO defense
    if (monster->health <= 0) {
        // send message: monster attack you and died + gold
        killMonster(st, monster->x, monster->y);
        return;
    }

    char *msg = (char *) malloc(sizeof(char) * st->map->width);
    snprintf(msg, st->map->width, "A \"%s\" attacked you!", monster->name);
    sendMenuMessage(st, msg);
    free(msg);
}

// TODO playerAttacksMonster(State *st, Monster *monster) {}
