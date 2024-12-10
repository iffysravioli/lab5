#include <stdio.h>
#include <stdlib.h>
#include "process.h"

int compare_priority(const void *a, const void *b) {
    ProcessType *processA = (ProcessType *)a;
    ProcessType *processB = (ProcessType *)b;
    return processA->pri - processB->pri;  
}

int compare_burst_time(const void *a, const void *b) {
    ProcessType *processA = (ProcessType *)a;
    ProcessType *processB = (ProcessType *)b;
    return processA->bt - processB->bt;  
}

void priority_scheduling(ProcessType plist[], int n) {
    qsort(plist, n, sizeof(ProcessType), compare_priority);  

    int wt = 0;  
    for (int i = 0; i < n; i++) {
        plist[i].wt = wt;  
        plist[i].tat = plist[i].wt + plist[i].bt;  
        wt += plist[i].bt;  
    }
}

void shortest_job_first(ProcessType plist[], int n) {
    qsort(plist, n, sizeof(ProcessType), compare_burst_time);  

    int wt = 0;  
    for (int i = 0; i < n; i++) {
        plist[i].wt = wt;  
        plist[i].tat = plist[i].wt + plist[i].bt;  
        wt += plist[i].bt;  
    }
}

void round_robin(ProcessType plist[], int n, int quantum) {
    int *rem_bt = (int *)malloc(n * sizeof(int));  
    for (int i = 0; i < n; i++)
        rem_bt[i] = plist[i].bt;  

    int t = 0;  
    int processes_completed = 0;  

    while (processes_completed < n) {
        for (int i = 0; i < n; i++) {
            if (rem_bt[i] > 0) {  
                if (rem_bt[i] > quantum) {
                    t += quantum;
                    rem_bt[i] -= quantum;
                } else {
                    t += rem_bt[i];
                    plist[i].wt = t - plist[i].bt;
                    rem_bt[i] = 0;
                    processes_completed++;
                }
            }
        }
    }

    for (int i = 0; i < n; i++) {
        plist[i].tat = plist[i].wt + plist[i].bt;
    }

    free(rem_bt);
}

void print_processes(ProcessType plist[], int n) {
    printf("PID\tBurst Time\tArrival Time\tPriority\tWaiting Time\tTurnaround Time\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", plist[i].pid, plist[i].bt, plist[i].art, plist[i].pri, plist[i].wt, plist[i].tat);
    }
}

int main() {
    int n, quantum;

    printf("Enter the number of processes: ");
    scanf("%d", &n);
    ProcessType *plist = (ProcessType *)malloc(n * sizeof(ProcessType));

    printf("Enter time quantum for Round Robin scheduling: ");
    scanf("%d", &quantum);

    for (int i = 0; i < n; i++) {
        plist[i].pid = i + 1;
        printf("Enter burst time and priority for process %d: ", plist[i].pid);
        scanf("%d %d", &plist[i].bt, &plist[i].pri);
        plist[i].art = 0;  
    }

    printf("\nPriority Scheduling\n");
    priority_scheduling(plist, n);
    print_processes(plist, n);

    printf("\nShortest Job First (SJF) Scheduling\n");
    shortest_job_first(plist, n);
    print_processes(plist, n);

    printf("\nRound Robin Scheduling\n");
    round_robin(plist, n, quantum);
    print_processes(plist, n);

    free(plist);
    return 0;
}