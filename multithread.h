#ifndef multithread_h
#define multithread_h

#include <pthread.h>

#include "error.h"

class Mutex{
public:
    Mutex(){
        if(pthread_mutex_init(&m_mutex,NULL)!=0)
            throw MemoryError();
    }
    ~Mutex(){
        pthread_mutex_destroy(&m_mutex);
    }
    bool lock(){
        return pthread_mutex_lock(&m_mutex)==0;
    }
    bool unlock(){
        return pthread_mutex_unlock(&m_mutex)==0;
    }
private:
    pthread_mutex_t m_mutex;
};

#endif
