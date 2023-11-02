#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>

struct thread_job
{
    void * ( * job)(void * argc);
    void * args;
};

struct thread_queue
{
    int size, top, bottom;
    struct thread_job * threads;
};

struct thread_pool
{
    int size;
    bool active;
    pthread_mutex_t lock;
    pthread_cond_t signal;
    pthread_t * threads;
    struct thread_queue * queue;
};

/*Thread Queue Operations*/
struct thread_queue * createThreadQueue(int size);
int addJobToQueue(struct thread_pool * pool, struct thread_job job);
struct thread_job popJobFromQueue(struct thread_pool * pool);
/*Thread Pool Operations*/
struct thread_pool * startThreadPool(int size);
bool startJob(struct thread_job job); 

#endif