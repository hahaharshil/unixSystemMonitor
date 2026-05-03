#include <ncurses.h>
#include "ui.h"

void init_ui() {
    initscr();
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
}

void draw_ui(Process *processes, int count) {
    clear();

    mvprintw(0, 0, "Top Processes (CPU)");
    mvprintw(1, 0, "PID     NAME            CPU%%     MEM(B)");

    int row = 2;

    for (int i = 0; i < 10 && i < count; i++) {
        mvprintw(row++, 0,
            "%-7d %-15s %-8.2f %-10ld",
            processes[i].pid,
            processes[i].name,
            processes[i].cpu_percent,
            processes[i].memory);
    }

    refresh();
}

void cleanup_ui() {
    endwin();
}
