/**
 * Representation of a task in the system.
 */

#ifndef TASK_H
#define TASK_H

// representation of a task

typedef struct task {
    char *name;
    int tid;
    int priority;
    int burst;
    int burst_left;
    int wait_time;
    int turnaround_time;
    int response_time;
    int flag;
} Task;

#endif
