/*
#include "memorypool.h"

MemoryPool *MemoryPool::memoryPoolInstance=NULL;
unsigned short MemoryPool::poolMapIndex=0;

MemoryPool::MemoryPool(int initBlockSize,int growBlockSize)
    :initBlockCount(initBlockSize),
    growBlockCount(growBlockSize),
    firstIndex(0),lastIndex(0),
mpPtr(NULL){
    
}

int MemoryPool::hash(int value){
    return 0;
}

int MemoryPool::alignBytes(int bytes){
    if(bytes<=4) return 4;
    int alignedBytes=(bytes+ALIGNMENT-1)&(~(ALIGNMENT-1));
    return alignedBytes;
}

int MemoryPool::getUnitSize(int index){
    int unitsize=blockmap[index];
    return unitsize;
}

MemoryPool *MemoryPool::GetInstance(){
    if(memoryPoolInstance==NULL){
        memoryPoolInstance=new MemoryPool();
    }
    return memoryPoolInstance;
}

void *MemoryPool::Alloc(int bytes){
    int alignedBytes=alignBytes(bytes);
    int hashCode=hash(alignBytes);
    if(block==NULL)
}
*/
