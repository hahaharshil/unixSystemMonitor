#ifndef UI_H
#define UI_H

#include "proc.h"

void init_ui();
void draw_ui(Process *processes, int count, double cpu_usage);
void cleanup_ui();

#endif
