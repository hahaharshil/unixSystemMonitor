#include "sort.h"
#include "proc.h"

int compare_cpu(const void *a, const void *b) {
    Process *p1 = (Process *)a;
    Process *p2 = (Process *)b;

    if (p2->cpu_percent > p1->cpu_percent) return 1;
    if (p2->cpu_percent < p1->cpu_percent) return -1;
    return 0;
}
