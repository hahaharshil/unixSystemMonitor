#include <stdio.h>
#include <unistd.h>
#include "proc.h"

#define MAX_PROCESSES 1024
int pick_top10(Process *all, int count, Process *top) {
    int top_count = 0;

    for (int i = 0; i < count; i++) {

        if (top_count < 10) {
            top[top_count++] = all[i];
            continue;
        }

        // find smallest CPU % in top[]
        int min_idx = 0;
        for (int j = 1; j < 10; j++) {
            if (top[j].cpu_percent < top[min_idx].cpu_percent)
                min_idx = j;
        }

        // replace if current is bigger
        if (all[i].cpu_percent > top[min_idx].cpu_percent) {
            top[min_idx] = all[i];
        }
    }

    return top_count;
}



int main() {
    Process processes[MAX_PROCESSES];

    int count;

    long long prev_total_cpu = get_total_cpu();

    while (1) {
        long long total_cpu = get_total_cpu();

        count = get_processes(processes);

        long long total_diff = total_cpu - prev_total_cpu;

        Process top[10];

int top_count = pick_top10(processes, count, top);

printf("\n------ TOP 10 (CPU) ------\n");

for (int i = 0; i < top_count; i++) {
    printf("PID: %d | %-15s | CPU: %.2f%% | MEM: %ld\n",
           top[i].pid,
           top[i].name,
           top[i].cpu_percent,
           top[i].memory);
}


        prev_total_cpu = total_cpu;

        sleep(1);
    }

    return 0;
}
