#include "mempool.h"

MemPool *MemPool::mempool=NULL;
ushort MemPool::index=0;

MemPool *MemPool::getInstacne(){
    if(mempool==NULL)
        mempool=new MemPool();
    return mempool;
}

MemPool::~MemPool(){}

void *alloc(int bytes){
    int alignbytes=alignBytes(bytes);
    int hashcode=hash(alignbytes);
    
    
}

void free(int bytes){
    
}

MemPool::MemPool(int initsize,int growsize)
:initsize(initsize),growsize(growsize),first(0),last(0),ptr(NULL){}

int MemPool::hash(int val){
    for(auto block:memhash)
        if(block) delete block;
}

int MemPool::alignBytes(int bytes){
    if(bytes<=4) return 4;
    int alignbytes=(bytes+ALIGN-1)&(~(ALIGN-1));
    return alignbytes;
}

int MemPool::getSize(int index){
    int size=blockmap[index];
    return size;
}


