#include <ncurses.h>
#include "window.h"
#include "state.h"

/**
 * @brief Main function
 * 
 * @details Initializes the ncurses window and the game state, then runs the game loop.
 * 
 * @return int exit status
 */
int main() {
    // Ncurses window
    int ncols, nrows;
    WINDOW *window = initWindow(&ncols, &nrows);
    if (window == NULL) {
        fprintf(stderr, "Error initialising ncurses window.\n");
        return 1; // EXIT_FAILURE
    }

    // Game state
    State *st = initState(ncols, nrows);
    drawState(st); // Draw initial state

    char input_key;
    while (1) {
        input_key = getch();
        updateState(st, input_key);
    }

    // Free state and close window: not reached
    freeState(st);
    endwin();
    return 0; // EXIT_SUCCESS
}
