#include <stdio.h>
#include <unistd.h>
#include "proc.h"
#include <ctype.h>
#include <dirent.h>
#include <string.h>


typedef struct  {
    long long user, nice, system, idle, iowait, irq, softirq;
} cpuStats;


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

//I dont know why this is there, but this solves an error;

int is_number(char *str) {
    for (int i = 0; str[i]; i++) {
        if (!isdigit(str[i])) return 0;
    }
    return 1;
}
int get_processes(Process *processes) {
    DIR *dir = opendir("/proc");
    if (!dir) return 0;

    struct dirent *entry;
    int count = 0;

    long total_mem = get_total_memory();

    while ((entry = readdir(dir)) != NULL) {
        if (!is_number(entry->d_name)) continue;

        int pid = atoi(entry->d_name);
        Process p;
        p.pid = pid;

        char path[256];
        FILE *fp;

        // ---- stat (name + cpu) ----
        sprintf(path, "/proc/%d/stat", pid);
        fp = fopen(path, "r");
        if (!fp) continue;

        char name[256];
        long utime, stime;

        fscanf(fp, "%d (%[^)])", &pid, name);
        for (int i = 0; i < 11; i++) fscanf(fp, "%*s");
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
        p.mem_percent = (double)p.memory / total_mem * 100.0;

        processes[count++] = p;
    }

    closedir(dir);
    return count;
}



int main(){
    cpuStats s1, s2;

    while(1){
        getStats(&s1);
        sleep(1);
        getStats(&s2);

    long long total1 = s1.user + s1.nice + s1.system + s1.idle + s1.iowait + s1.softirq;
    long long total2 = s1.user + s2.nice + s2.system + s2.idle + s2.iowait + s2.softirq;

    long long idle1 = s1.idle;
    long long idle2 = s2.idle;
    
    double totalDiff = (double)total2 - total1;
    double idleDiff = (double)idle2 - idle1;
    double cpu_usage = (double)(100 * (totalDiff - idleDiff)/ totalDiff);

    printf("Current CPU Usage: %.2f%%\n", cpu_usage);
    }
    return 0;
}



    
