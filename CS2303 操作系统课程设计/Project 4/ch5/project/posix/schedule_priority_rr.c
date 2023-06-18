#include <stdio.h>
#include <stdlib.h>

#include "task.h"
#include "list.h"
#include "schedulers.h"
#include "cpu.h"

struct node *head[11] = {NULL};

tid_value = 0;

// time now
int time = 0;
int last_time = 0;
// task number
int task_cnt = 0;
// wait time
// int time_wait = 0;
int total_wait = 0;
// turnaround time
// int time_turnaround = 0;
int total_turnaround = 0;
// total response time = wait time
// int time_response = 0;
int total_response = 0;

void add(char *name, int priority, int burst) 
{
    Task *newTask = malloc(sizeof(Task));
    newTask->tid = __sync_fetch_and_add(&tid_value,1);
    newTask->name = name;
    newTask->priority = priority;
    newTask->burst = burst;
    newTask->burst_left = burst;
    newTask->flag = 0;
    insert(&head[priority],newTask);
    task_cnt++;
}

int schedule_onetask(int priority)
{
    if (head[priority] == NULL) {
        return 0;
    }
    struct node *cur = head[priority];

    while(cur != NULL)
    {
        int onetime = QUANTUM;
        if (cur->task->flag == 0) 
        {
            cur->task->response_time = time;
            cur->task->flag = 1;
            total_response += cur->task->response_time;
        }
        if (cur->task->burst_left <= QUANTUM) 
        {
            onetime = cur->task->burst_left;
            run(cur->task, onetime);
            cur->task->turnaround_time = time + onetime;  //终止时间
            cur->task->wait_time = cur->task->turnaround_time - cur->task->burst;
            total_wait += cur->task->wait_time;
            total_turnaround += cur->task->turnaround_time;
            delete(&head[priority], cur->task);
        }
        else
        {
            run(cur->task, onetime);
            cur->task->burst_left -= onetime;
        }
        time += onetime;
        cur = cur->next;
    }
    return 1;
}

void print_static()
{
    printf("Average wait time = %f\n", (float)total_wait/task_cnt);
    printf("Average turnaround time = %f\n", (float)total_turnaround/task_cnt);
    printf("Average response time = %f\n", (float)total_response/task_cnt);
}
struct node *reverse( struct node *head )
{
    struct node *p1=head,*p2=NULL,*t=head;
    while(p1)
    {
        t=p1;
        p1=p1->next;
        t->next=p2;
        p2=t;
    }
    return p2;
}
void schedule()
{
    for (int priority = 10; priority >= 1; priority--)
    {
        head[priority] = reverse(head[priority]);
        while(schedule_onetask(priority));
    }
    print_static();
}