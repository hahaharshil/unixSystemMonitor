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

int get_processes(Process *processes) {
    DIR *dir = opendir("/proc");
    if (!dir) return 0;

    struct dirent *entry;
    int count = 0;

    while ((entry = readdir(dir)) != NULL) {

        // skip non-numeric names
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

        // 👇 THIS LINE IS THE KEY
        fscanf(fp, "%d (%[^)])", &pid, name);

        for(int i = 0; i < 11; i++){
            fscanf(fp, "%*s");
        }

        fscanf(fp, "%ld %ld", &utime, &stime);

        fclose(fp);

        strcpy(p.name, name);
        p.cpu_time = utime + stime;

        // ---- memory ----
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

    
