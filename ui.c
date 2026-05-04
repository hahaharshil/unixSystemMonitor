#include <ncurses.h>
#include "ui.h"

void init_ui() {
    initscr();
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
    
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_GREEN, COLOR_BLACK); 
        init_pair(2, COLOR_CYAN, COLOR_BLACK);
        init_pair(3, COLOR_RED, COLOR_BLACK);
    }
}

void draw_ui(Process *processes, int count, double cpu_usage, double mem_usage) {
    clear();

    attron(COLOR_PAIR(1));
    mvprintw(0, 0, "CPU Usage: %.2f%%    MEM Usage: %.2f%%", cpu_usage, mem_usage);
    attroff(COLOR_PAIR(1));

    mvprintw(1, 0, "-------------------------------------------");

    attron(COLOR_PAIR(2));
    mvprintw(2, 0, "PID     NAME            CPU%%     MEM(B)");
    attroff(COLOR_PAIR(2));

    int row = 3;

    for (int i = 0; i < 10 && i < count; i++) {
        if (processes[i].cpu_percent > 10.0) {
            attron(COLOR_PAIR(3));
        }
        
        mvprintw(row++, 0,
            "%-7d %-15s %-8.2f %-10ld",
            processes[i].pid,
            processes[i].name,
            processes[i].cpu_percent,
            processes[i].memory);
            
        if (processes[i].cpu_percent > 10.0) {
            attroff(COLOR_PAIR(3));
        }
    }

    refresh();
}

void cleanup_ui() {
    endwin();
}
