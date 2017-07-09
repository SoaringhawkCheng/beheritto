#include "mempool.h"

MemPool &MemPool::getInstance(){
        mempool=MemPool();
        return mempool;
}

MemPool::~MemPool(){
    mutex.lock();
    for(auto memlist:lists){
        MemBlock *memblock=memlist->usedhead;
        while(memblock!=NULL){
            MemBlock *next=memblock->next;
            ::free(memblock);
            memblock=next;
        }
        memblock=memlist->freehead;
        while(memblock!=NULL){
            MemBlock *next=memblock->next;
            ::free(memblock);
            memblock=next;
        }
    }
    mutex.unlock();
}

void *MemPool::alloc(size_t size){
    mutex.lock();
    if(lists.empty()){
        
    }
    mutex.unlock();
}



