#ifndef PROC_H
#define PROC_H

typedef struct {
    long long user, nice, system, idle, iowait, irq, softirq;
} cpuStats;


typedef struct {
    int pid;
    char name[256];

    long cpu_time;
    long prev_cpu_time;
    double cpu_percent;

    long memory;
    double mem_percent;
}Process;


void getStats(cpuStats *s);

int get_processes(Process *processes);
long long get_total_cpu();
long get_total_memory();

#endif



