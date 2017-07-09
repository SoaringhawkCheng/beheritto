#include "mempool.h"

MemPool *MemPool::mempool=NULL;

MemPool *MemPool::getInstance(){
    if(mempool==NULL){
        mempool=(MemPool *)malloc(sizeof(MemPool));
        mempool->init();
    }
    return mempool;
}

MemPool::~MemPool(){
    destroy();
}

void MemPool::init(){
    count=0;
}

void *MemPool::alloc(size_t size){
    size_t alignbytes=alignBytes(size);
    void *buff=NULL;
    /*内存池为空*/
    if(this->lists.empty()){
        buff=malloc(alignbytes);
        if(buff==NULL) return NULL;
        MemList *memlist=(MemList *)malloc(sizeof(MemList));
        if(memlist==NULL){
            free(buff);
            return NULL;
        }
        MemBlock *memblock=(MemBlock *)malloc(sizeof(MemBlock));
        if(memblock==NULL){
            free(buff);
            free(memlist);
            return NULL;
        }
        
        memblock->block=buff;
        memlist->used.push_back(memblock);
        
        memlist->size=size;
        this->lists.push_back(memlist);
       
        setBlockMeta(buff,alignbytes);
        return buff;
    }
    /*内存池不为空*/
    else{
        for(auto list_iter=this->lists.begin();list_iter!=this->lists.end();++list_iter){
            /*存在合适大小的内存链表,则在该链表中寻找空闲块*/
            if((*list_iter)->size==alignbytes){
                /*链表中没有剩余的空闲块*/
                if((*list_iter)->unused.empty()){
                    buff=malloc(alignbytes);
                    if(buff==NULL) return buff;
                    MemBlock *memblock=(MemBlock *)malloc(sizeof(MemBlock));
                    if(memblock==NULL){
                        free(buff);
                        return NULL;
                    }
                    (*list_iter)->used.push_back(memblock);
                    memblock->block=buff;
                    setBlockMeta(buff,alignbytes);
                    return buff;
                }
                /*链表中有剩余的空闲块*/
                else{
                    MemBlock *memblock=(*list_iter)->unused.back();
                    (*list_iter)->unused.pop_back();
                    buff=memblock->block;
                    
                    jumpBlockMeta(buff);
                    return buff;
                }
            }
        }
        /*不存在合适大小的内存链表，新建一个链表*/
        buff=malloc(size+sizeof(MemBlockMeta));
        if(buff==NULL) return NULL;
        MemList *memlist=(MemList *)malloc(sizeof(MemList));
        if(memlist==NULL){
            free(buff);
            return NULL;
        }
        MemBlock *memblock=(MemBlock *)malloc(sizeof(MemBlock));
        if(memblock==NULL){
            free(buff);
            free(memlist);
        }
        
        memblock->block=buff;
        memlist->used.push_back(memblock);
        
        memlist->size=alignbytes;
        this->lists.push_back(memlist);
        
        setBlockMeta(buff,alignbytes);
        return buff;
    }
}

bool MemPool::dealloc(void *buff){
    getBlockMeta(buff);
    MemBlockMeta *meta=(MemBlockMeta *)buff;
    for(auto list_iter=this->lists.begin();list_iter!=this->lists.end();++list_iter){
        if((*list_iter)->size==meta->size){
            MemList *memlist=*list_iter;
            for(auto block_iter=memlist->used.begin();block_iter!=memlist->used.end();++block_iter){
                if((*block_iter)==buff){
                    MemBlock *memblock=(*block_iter);
                    memlist->used.erase(block_iter);
                    memlist->unused.push_back(memblock);
                    (*list_iter)->unused.push_back(memblock);
                    --block_iter;
                    if((*list_iter)->used.empty()){
                        MemList *memlist=*list_iter;
                        while(!memlist->unused.empty()){
                            MemBlock *unusedmemblock=memlist->unused.back();
                            memlist->unused.pop_back();
                            free(unusedmemblock);
                        }
                        
                    }
                        mempool->lists.erase(list_iter);
                        --list_iter;
                    return true;
                }
            }
        }
    }
    return false;
}

size_t MemPool::alignBytes(size_t size){
    size=size+sizeof(MemBlockMeta);
    int bit=0;
    while(size){
        ++bit;
        size>>=1;
    }
    return 1<<bit;
}

void MemPool::setBlockMeta(void *&buff,size_t alignbytes){
    MemBlockMeta *meta=(MemBlockMeta *)buff;
    meta->size=alignbytes;
    buff=meta+sizeof(MemBlockMeta);
}

void MemPool::jumpBlockMeta(void *&buff){
    MemBlockMeta *meta=(MemBlockMeta *)buff;
    buff=meta+sizeof(MemBlockMeta);
}

void MemPool::getBlockMeta(void *&buff){
    MemBlockMeta *meta=(MemBlockMeta *)buff;
    buff=meta-sizeof(MemBlockMeta);
}

inline void *operator new(size_t size){
    void *buff=MemPool::getInstance()->alloc(size);
    return buff;
}

inline void operator delete(void *buff){
    
}

inline void operator delete[](void *buff){
    
}
