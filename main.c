#include <stdio.h>
#include <unistd.h>
#include "proc.h"
#include <stdlib.h>
#include "ui.h"
#include "sort.h"

#define MAX_PROCESSES 1024

int main(){
   

    Process prev_processes[MAX_PROCESSES];
    int prev_count = 0;

    Process processes[MAX_PROCESSES];
    
    long long prev_total_cpu = get_total_cpu();

    cpuStats prev_stats;
    getStats(&prev_stats);


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

        cpuStats curr_stats;
        getStats(&curr_stats);

        long long prev_total =
            prev_stats.user + prev_stats.nice + prev_stats.system +
            prev_stats.idle + prev_stats.iowait +
            prev_stats.irq + prev_stats.softirq;

        long long curr_total =
            curr_stats.user + curr_stats.nice + curr_stats.system +
            curr_stats.idle + curr_stats.iowait +
            curr_stats.irq + curr_stats.softirq;

        long long total_diff_sys = curr_total - prev_total;
        long long idle_diff = curr_stats.idle - prev_stats.idle;

        double cpu_usage = 0;

        if (total_diff_sys > 0) {
            cpu_usage = (double)(total_diff_sys - idle_diff) / total_diff_sys * 100.0;
        }

        prev_stats = curr_stats;

        
        qsort(processes, count, sizeof(Process), compare_cpu);


        double mem_usage = get_sys_mem_percent();

        draw_ui(processes, count, cpu_usage, mem_usage);

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

