/**
 * Implementation of thread pool.
 */

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "threadpool.h"
#include <stdbool.h>

#define QUEUE_SIZE 10
#define NUMBER_OF_THREADS 3

#define TRUE 1

bool shutdown = false;

// this represents work that has to be 
// completed by a thread in the pool
typedef struct 
{
    void (*function)(void *p);
    void *data;
}
task;

// the work queue
struct worknode
{
    task worktodo;
    struct worknode *next;
} *head, *tail;

// the worker bee
pthread_t bee[NUMBER_OF_THREADS];

pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t sem;
// insert a task into the queue
// returns 0 if successful or 1 otherwise, 
int enqueue(task t) 
{
    tail->next = (struct worknode*)malloc(sizeof(struct worknode));
    if (tail->next == NULL)
    {
        printf("Queue is full\n");
        return 1;
    }
    tail = tail->next;
    tail->worktodo = t;
    return 0;
}

// remove a task from the queue
task dequeue() 
{
    if (head == tail)
    {
        printf("Queue is empty\n");
        exit(0);
    }
    struct worknode *old_head = head;
    head = head->next;
    free(old_head);
    return head->worktodo;
}

// the worker thread in the thread pool
void *worker(void *param)
{
    // execute the task
    // execute(worktodo.function, worktodo.data);
    static task worktodo;
    // printf("worker created\n");
    while(TRUE)
    {
        sem_wait(&sem);
        if (shutdown)
        {
            pthread_exit(0);
        }
        pthread_mutex_lock(&queue_mutex);
        worktodo = dequeue();
        pthread_mutex_unlock(&queue_mutex);
        execute(worktodo.function, worktodo.data);
    }
    pthread_exit(0);
}

/**
 * Executes the task provided to the thread pool
 */
void execute(void (*somefunction)(void *p), void *p)
{
    (*somefunction)(p);
}

/**
 * Submits work to the pool.
 */
int pool_submit(void (*somefunction)(void *p), void *p)
{
    // worktodo.function = somefunction;
    // worktodo.data = p;
    // printf("submitting work\n");
    task worktodo;
    worktodo.function = somefunction;
    worktodo.data = p;
    // printf("worktodo created\n");
    pthread_mutex_lock(&queue_mutex);
    enqueue(worktodo);
    pthread_mutex_unlock(&queue_mutex);
    sem_post(&sem);
    // printf("work submitted\n");
    return 0;
}

// initialize the thread pool
void pool_init(void)
{
    head = (struct worknode*)malloc(sizeof(struct worknode));
    tail = head;
    sem_init(&sem, 0, 0);
    for(int i = 0; i < NUMBER_OF_THREADS; i++)
    {
        pthread_create(&bee[i], NULL, worker, NULL);
        printf("thread %d created\n", i);
    }
}

// shutdown the thread pool
void pool_shutdown(void)
{
    shutdown = true;
    for (int i = 0; i < NUMBER_OF_THREADS; ++ i)
		sem_post(&sem);
    for(int i = 0; i < NUMBER_OF_THREADS; i++)
    {
        pthread_join(bee[i], NULL);
    }
    printf("Join the threads successfully!\n");
    sem_destroy(&sem);
    pthread_mutex_destroy(&queue_mutex);
}
