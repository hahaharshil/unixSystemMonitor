#include <stdio.h>
#include <unistd.h>
#include "proc.h"
#include <ctype.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>


/*
typedef struct  {
    long long user, nice, system, idle, iowait, irq, softirq;
} cpuStats;

*/


int is_number(char *str) {
    for (int i = 0; str[i]; i++) {
        if (!isdigit(str[i])) return 0;
    }
    return 1;
}




void getStats(cpuStats *s){
    FILE *fp = fopen("/proc/stat", "r");
    
    fscanf(fp, "cpu %lld %lld %lld %lld %lld %lld %lld", 
           &s->user, &s->nice, &s->system, &s->idle, &s->iowait, &s->irq, &s->softirq);
    fclose(fp);
}

long long get_total_cpu(){
    cpuStats s;
    getStats(&s);

    return s.user + s.nice + s.system + s.idle + s.iowait + s.irq + s.softirq;
}

long get_total_memory(){
    FILE *fp = fopen("/proc/meminfo", "r");
    if(!fp)
        return 0;
    
    long mem_kb;

    fscanf(fp, "MemTotal: %ld kB", &mem_kb);

    fclose(fp);

    return mem_kb * 1024;
}


double get_sys_mem_percent() {
    FILE *fp = fopen("/proc/meminfo", "r");
    if (!fp) return 0.0;

    char line[256];
    long total_mem = 0;
    long available_mem = 0;
    int found_total = 0, found_avail = 0;

    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "MemTotal:", 9) == 0) {
            sscanf(line, "MemTotal: %ld kB", &total_mem);
            found_total = 1;
        } else if (strncmp(line, "MemAvailable:", 13) == 0) {
            sscanf(line, "MemAvailable: %ld kB", &available_mem);
            found_avail = 1;
        }
        
        if (found_total && found_avail) break;
    }
    fclose(fp);

    if (total_mem > 0 && found_avail) {
        return ((double)(total_mem - available_mem) / total_mem) * 100.0;
    }
    return 0.0;
}

int get_processes(Process *processes) {
    DIR *dir = opendir("/proc");
    if (!dir) return 0;

    struct dirent *entry;
    int count = 0;

    while ((entry = readdir(dir)) != NULL) {

        if (!is_number(entry->d_name)) continue;

        int pid = atoi(entry->d_name);

        Process p;
        p.pid = pid;

        char path[256];
        long utime, stime;


        sprintf(path, "/proc/%d/stat", pid);

        FILE *fp = fopen(path, "r");
        if (!fp) continue;

        char name[256];

        fscanf(fp, "%d (%[^)])", &pid, name);

        for(int i = 0; i < 11; i++){
            fscanf(fp, "%*s");
        }

        fscanf(fp, "%ld %ld", &utime, &stime);

        fclose(fp);

        strcpy(p.name, name);
        p.cpu_time = utime + stime;

        sprintf(path, "/proc/%d/statm", pid);
        fp = fopen(path, "r");
        if (!fp) continue;

        long rss;
        fscanf(fp, "%*s %ld", &rss);
        fclose(fp);

        p.memory = rss * getpagesize();

        processes[count++] = p;
    }

    closedir(dir);
    return count;
}

    
