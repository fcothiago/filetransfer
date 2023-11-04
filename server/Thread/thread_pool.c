#include "thread_pool.h"
#include <stdio.h>

void * thread_job(void * arg)
{
    struct thread_pool * pool = (struct thread_pool *) arg;
    struct thread_job job;
    for(;pool->active;)
    {
        pthread_mutex_lock(&pool->lock);
        pthread_cond_wait(&pool->signal,&pool->lock);
        job = popJobFromQueue(pool);
        pthread_mutex_unlock(&pool->lock);
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
    pool->queue = createThreadQueue(size);
    pool->lock = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
    pool->signal = (pthread_cond_t) PTHREAD_COND_INITIALIZER;
    for(int i = 0; i < size ; i++)
    {
        pthread_create(&pool->threads[i],NULL,thread_job,pool);
    }
    return pool;
}

void stopThreadPool(struct thread_pool * pool)
{
    pool->active = false;
    pthread_cond_broadcast(&pool->signal);
    for(int i = 0; i < pool->size ; i++)
    {
        pthread_join(pool->threads[i],NULL);
    }
    deleteThreadQueue(pool->queue);
    free(pool->threads);
    free(pool);
}