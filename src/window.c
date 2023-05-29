#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include "window.h"

/**
 * @brief Initializes the color pairs
 * 
 * @details Enables color and initializes the color pairs for the game.
 * 
 * @return void
 */
void initColor() {
	// Enable color
    start_color();

    // Initialize color pairs
    init_pair(COLOR_BLACK, COLOR_BLACK, COLOR_BLACK);
    init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);
	init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);
    init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_BLACK);
    init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
}

/**
 * @brief Initializes the ncurses window
 * 
 * @details Initializes the ncurses window with game settings,
 * and sets up the color pairs, also saves the window size for later use.
 * 
 * @param ncols pointer to the number of columns
 * @param nrows pointer to the number of rows
 * @return WINDOW* pointer to the ncurses window
 */
WINDOW *initWindow(int *ncols, int *nrows) {
    WINDOW *window = initscr();
    if (window == NULL) {
        fprintf(stderr, "Error initialising ncurses.\n");
        return NULL;
    }

    // Get window dimensions
    int nr, nc;
	getmaxyx(window, nr, nc);
    *ncols = nc;
    *nrows = nr;

    // Ncurses settings
    srandom(time(NULL));      // Seed random number generator
	cbreak();                 // Disable line buffering
	noecho();                 // Disable input echoing
	nonl();                   // Disable newline translation
	intrflush(stdscr, false); // Disable flushing on interrupt
	keypad(stdscr, true);     // Enable keypad input
    initColor();              // Enable color and set color pairs

    return window;
}
