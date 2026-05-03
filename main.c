#include <stdio.h>
#include <unistd.h>
#include "proc.h"
#include <stdlib.h>
#include "ui.h"

#define MAX_PROCESSES 1024


int compare_cpu(const void *a, const void *b) {
    Process *p1 = (Process *)a;
    Process *p2 = (Process *)b;

    if (p2->cpu_percent > p1->cpu_percent) return 1;
    if (p2->cpu_percent < p1->cpu_percent) return -1;
    return 0;
}


int main(){
   

    Process prev_processes[MAX_PROCESSES];
    int prev_count = 0;

    Process processes[MAX_PROCESSES];
    
    long long prev_total_cpu = get_total_cpu();

    init_ui();

while(1) {

    long long total_cpu = get_total_cpu();

    int count = get_processes(processes);

    for (int i = 0; i < count; i++) {
        processes[i].prev_cpu_time = 0;

        for (int j = 0; j < prev_count; j++) {
            if (processes[i].pid == prev_processes[j].pid) {
                processes[i].prev_cpu_time = prev_processes[j].cpu_time;
                break;
            }
        }
    }

    long long total_diff = total_cpu - prev_total_cpu;
        
    

    for (int i = 0; i < count; i++) {

        double cpu_percent = 0;

        if (processes[i].prev_cpu_time == 0) {
        processes[i].cpu_percent = 0;
        continue;
    }

    long proc_diff = processes[i].cpu_time - processes[i].prev_cpu_time;



        if (total_diff > 0) {
            cpu_percent = (double)proc_diff / total_diff * 100;
        }

        processes[i].cpu_percent = cpu_percent;
    }
    qsort(processes, count, sizeof(Process), compare_cpu);

    draw_ui(processes, count);

    for (int i = 0; i < count; i++) {
        prev_processes[i] = processes[i];
    }
    prev_count = count;

    prev_total_cpu = total_cpu;

    sleep(1);
}

    cleanup_ui();

    return 0;
}





