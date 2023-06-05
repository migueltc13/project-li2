#include <ncurses.h>
#include "window.h"
#include "state.h"

/**
 * @brief Main function
 * 
 * @details Initializes the ncurses window and the game state, then runs the game loop.
 * 
 * @return exit status
 */
int main() {
    // ncurses window
    int ncols, nrows;
    initWindow(&ncols, &nrows);

    // Game state
    State *st = initState(ncols, nrows);
    drawState(st); // Draw initial state

    int input_key;
    while (1) {
        input_key = getch();
        updateState(st, input_key);
    }

    // Free state and close window: not reached
    freeState(st);
    endwin();
    return 0; // EXIT_SUCCESS
}
