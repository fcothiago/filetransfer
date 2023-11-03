#include "thread_pool.h"
#include <stdio.h>
struct thread_queue * createThreadQueue(int size)
{
    if (size < 1)
        return NULL;
    struct thread_queue * queue = (struct thread_queue * ) malloc(sizeof(struct thread_queue));
    queue->size = size;
    queue->top = -1;
    queue->bottom = 0;
    queue->threads = (struct thread_job *) malloc(sizeof(struct thread_job[size]));
    return queue;
}

bool addJobToQueue(struct thread_pool * pool, struct thread_job job)
{
    pthread_mutex_lock(&pool->lock);
    int top = pool->queue->top;
    int bottom = pool->queue->bottom;
    int size = pool->queue->size;
    bool full = true;
    if( top != bottom  ) 
    {
        if(top == -1)
            pool->queue->top = 0;
        pool->queue->threads[bottom] = job;
        pool->queue->bottom = (bottom + 1) % size; 
        full = false;
    }
    pthread_cond_signal(&pool->signal);
    pthread_mutex_unlock(&pool->lock);
    return full;
}

struct thread_job  popJobFromQueue(struct thread_pool * pool)
{
    pthread_mutex_lock(&pool->lock);
    int top = pool->queue->top;
    int size = pool->queue->size;
    struct thread_job job;
    job.job = NULL;
    job.args = NULL; 
    if( top > -1 ) 
    {
        job = pool->queue->threads[top];
        pool->queue->top = (top+1) % size;
        if(pool->queue->top == pool->queue->bottom)
            pool->queue->top = -1;
    }
    pthread_mutex_unlock(&pool->lock);
    return job ;
}