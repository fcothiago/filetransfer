#include "thread_pool.h"
#include <stdio.h>
void * thread_job(void * arg)
{
    struct thread_pool * pool = (struct thread_pool *) arg;
    struct thread_job job;
    for(;pool->active;)
    {
        pthread_cond_wait(&pool->signal,&pool->lock);
        job = popJobFromQueue(pool);
        if(job.job)
        {
            job.job(job.args);
        }
    }
}

struct thread_pool * startThreadPool(int size)
{
    struct thread_pool * pool = (struct thread_pool *) malloc( sizeof(struct thread_pool) );
    pool->size = size;
    pool->active = true;
    pool->threads = (pthread_t *) malloc(sizeof(pthread_t [size]));
    pool->lock = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
    pool->signal = (pthread_cond_t) PTHREAD_COND_INITIALIZER;
    pool->queue = createThreadQueue(size);
    pthread_mutex_lock(&pool->lock);
    for(int i = 0; i < size ; i++)
    {
        pthread_create(&pool->threads[i],NULL,thread_job,pool);
    }
    pthread_mutex_unlock(&pool->lock);
    return pool;
}