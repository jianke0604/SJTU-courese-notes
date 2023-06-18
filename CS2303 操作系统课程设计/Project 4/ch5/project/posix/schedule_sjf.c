#include <stdio.h>
#include <stdlib.h>

#include "task.h"
#include "list.h"
#include "schedulers.h"
#include "cpu.h"

struct node *head = NULL;

tid_value = 0;

// time now
int time = 0;
// task number
int task_cnt = 0;
// wait time
int time_wait = 0;
int total_wait = 0;
// turnaround time
int time_turnaround = 0;
int total_turnaround = 0;
// total response time = wait time
// int time_response = 0;

void add(char *name, int priority, int burst) 
{
    Task *newTask = malloc(sizeof(Task));
    newTask->tid = __sync_fetch_and_add(&tid_value,1);
    newTask->name = name;
    newTask->priority = priority;
    newTask->burst = burst;
    insert(&head,newTask);
    task_cnt++;
}

int schedule_onetask()
{
    if (head == NULL) {
        return 0;
    }
    struct node *cur = head;
    struct node *temp = head;
    while(temp->next != NULL) {
        temp = temp->next;
        if (temp->task->burst <= cur->task->burst) {
            cur = temp;
        }
    }
    run(cur->task, cur->task->burst);
    delete(&head, cur->task);

    cur->task->wait_time = time_wait;
    cur->task->response_time = time_wait;

    time += cur->task->burst;
    time_wait += cur->task->burst;
    time_turnaround += cur->task->burst;

    cur->task->turnaround_time = time_turnaround;

    total_wait += cur->task->wait_time;
    total_turnaround += cur->task->turnaround_time;
    return 1;
}

void print_static()
{
    printf("Average wait time = %f\n", (float)total_wait/task_cnt);
    printf("Average turnaround time = %f\n", (float)total_turnaround/task_cnt);
    printf("Average response time = %f\n", (float)total_wait/task_cnt);
}

void schedule()
{
    while(schedule_onetask());
    print_static();
}