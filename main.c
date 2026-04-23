#include <stdio.h>
#include <unistd.h>

typedef struct  {
    long long user, nice, system, idle, iowait, irq, softirq;
} cpuStats;


void getStats(cpuStats *s){
    FILE *fp = fopen("/proc/stat", "r");
    
    fscanf(fp, "cpu %llu %llu %llu %llu %llu %llu %llu", 
           &s->user, &s->nice, &s->system, &s->idle, &s->iowait, &s->irq, &s->softirq);
    fclose(fp);
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



    
