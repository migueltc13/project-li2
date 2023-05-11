#ifndef _STATE_H_
#define _STATE_H_

typedef struct state State;

State *initState(int width, int height);
void freeState(void *p);
void updateState(State *st, int input_key);
void drawState(State *st);

#endif