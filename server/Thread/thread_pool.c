#include "thread_pool.h"

struct thread_pool * startThreadPool(int size)
{
    struct thread_pool * pool = (struct thread_pool *) malloc( sizeof(struct thread_pool) );
    pool->size = size;
    pool->active = true;
    pool->lock = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
    pool->signal = (pthread_cond_t) PTHREAD_COND_INITIALIZER;
    pool->queue = createThreadQueue(size);
    return pool;
}