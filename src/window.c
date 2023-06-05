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
    
    init_pair(COLOR_BLACK + 8, COLOR_BLACK, COLOR_WHITE);
    init_pair(COLOR_RED + 8, COLOR_RED, COLOR_WHITE);
    init_pair(COLOR_GREEN + 8, COLOR_GREEN, COLOR_WHITE);
    init_pair(COLOR_YELLOW + 8, COLOR_YELLOW, COLOR_WHITE);
    init_pair(COLOR_BLUE + 8, COLOR_BLUE, COLOR_WHITE);
    init_pair(COLOR_MAGENTA + 8, COLOR_MAGENTA, COLOR_WHITE);
    init_pair(COLOR_CYAN + 8, COLOR_CYAN, COLOR_WHITE);
    init_pair(COLOR_WHITE + 8, COLOR_WHITE, COLOR_WHITE);

    /* init_pair(COLOR_BLACK + 16, COLOR_BLACK, COLOR_YELLOW);
    init_pair(COLOR_RED + 16, COLOR_RED, COLOR_YELLOW);
    init_pair(COLOR_GREEN + 16, COLOR_GREEN, COLOR_YELLOW);
    init_pair(COLOR_YELLOW + 16, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(COLOR_BLUE + 16, COLOR_BLUE, COLOR_YELLOW);
    init_pair(COLOR_MAGENTA + 16, COLOR_MAGENTA, COLOR_YELLOW);
    init_pair(COLOR_CYAN + 16, COLOR_CYAN, COLOR_YELLOW);
    init_pair(COLOR_WHITE + 16, COLOR_WHITE, COLOR_YELLOW); */
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
        fprintf(stderr, "Fatal: Error initializing ncurses window with initscr().\n");
        exit(EXIT_FAILURE);
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
	intrflush(window, false); // Disable flushing on interrupt
	keypad(window, true);     // Enable keypad input
    curs_set(0);              // Disable cursor
    initColor();              // Enable color and set color pairs

    return window;
}
