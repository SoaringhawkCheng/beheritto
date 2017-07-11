#include "mempool.h"
#include "collector.h"

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
    gc=GarbageCollector::getInstance();
    firstlist=lastlist=NULL;
}

void *MemPool::alloc(size_t size){
    if(state==StateType::STATEINTERM)
    if(state==StateType::STATERUNTIME){
        if(++count==16){
            count=0;
            gc->process();
        }
    }
    size_t alignbytes=alignBytes(size);
    cout<<size<<" "<<alignbytes<<endl;
    cout<<"Allocate memory "<<alignbytes<<" bytes!"<<endl;
    void *buff=NULL;
    /*内存池为空*/
    if(firstlist==NULL){
        buff=malloc(alignbytes);
        if(buff==NULL) return NULL;
        MemList *memlist=(MemList *)malloc(sizeof(MemList));
        if(memlist==NULL){
            free(buff);
            return NULL;
        }
        memlist->init();
        MemBlock *memblock=(MemBlock *)malloc(sizeof(MemBlock));
        if(memblock==NULL){
            free(buff);
            free(memlist);
            return NULL;
        }
        memblock->init();
        memblock->block=buff;
        
        memlist->size=alignbytes;
        memlist->busylisthead=memlist->busylisttail=memblock;
        
        firstlist=lastlist=memlist;
       
        setBlockMeta(buff,alignbytes,memlist,memblock);
        
        memblock=NULL;
        memlist=NULL;
       
        return buff;
    }
    /*内存池不为空*/
    else{
        for(MemList *list_pointer=firstlist;list_pointer!=NULL;list_pointer=list_pointer->nextlist){
            /*存在合适大小的内存链表,则在该链表中寻找空闲块*/
//            cout<<"current memlist size = "<<((*list_iter)->size)<<endl;
//            cout<<" need to alloc = "<<alignbytes<<endl;
            if(list_pointer->size==alignbytes){
                /*链表中没有剩余的空闲块*/
                if(list_pointer->freelisthead==NULL){
                    buff=malloc(alignbytes);
                    if(buff==NULL) return NULL;
                    MemBlock *memblock=(MemBlock *)malloc(sizeof(MemBlock));
                    if(memblock==NULL){
                        free(buff);
                        return NULL;
                    }
                    memblock->init();
                    memblock->block=buff;
                    
                    if(list_pointer->busylisthead==NULL){
                        list_pointer->size=alignbytes;
                        list_pointer->busylisthead=memblock;
                        list_pointer->busylisttail=memblock;
                    }
                    else{
                        list_pointer->busylisttail->next=memblock;
                        memblock->prev=list_pointer->busylisttail;
                        list_pointer->busylisttail=memblock;
                    }
                    
                    setBlockMeta(buff,alignbytes,list_pointer,memblock);
                    
                    memblock=NULL;
                    
                    return buff;
                }
                /*链表中有剩余的空闲块*/
                else{
                    MemBlock *memblock=list_pointer->freelisttail;
                    if(list_pointer->freelisthead==list_pointer->freelisttail)
                        list_pointer->freelisthead=list_pointer->freelisttail=NULL;
                    else{
                        list_pointer->freelisttail=list_pointer->freelisttail->prev;
                        list_pointer->freelisttail->next=NULL;
                    }
                    if(list_pointer->busylisthead==NULL)
                        list_pointer->busylisthead=list_pointer->busylisttail=memblock;
                    else{
                        list_pointer->busylisttail->next=memblock;
                        list_pointer->busylisttail=memblock;
                    }

                    memblock=NULL;
                    jumpBlockMeta(buff);
                    return buff;
                }
            }
        }
        /*不存在合适大小的内存链表，新建一个链表*/
        buff=malloc(alignbytes);
        if(buff==NULL) return NULL;
        MemList *memlist=(MemList *)malloc(sizeof(MemList));
        if(memlist==NULL){
            free(buff);
            return NULL;
        }
        memlist->init();
        MemBlock *memblock=(MemBlock *)malloc(sizeof(MemBlock));
        if(memblock==NULL){
            free(buff);
            free(memlist);
        }
        memblock->init();
        memblock->block=buff;
        
        memlist->busylisthead=memlist->busylisttail=memblock;
        memlist->size=alignbytes;
        
        lastlist->nextlist=memlist;
        lastlist=memlist;
        
        setBlockMeta(buff,alignbytes,memlist,memblock);
        
        memblock=NULL;
        memlist=NULL;
        return buff;
    }
}

bool MemPool::dealloc(void *buff){
    cout<<"Deallocating Memory!"<<endl;
    MemList *memlist=NULL;
    MemBlock *memblock=NULL;
    getBlockMeta(buff);
    MemBlockMeta *meta=(MemBlockMeta *)buff;
    memlist=meta->curlist;
    memblock=meta->curblock;
    
    if(memlist!=NULL&&memblock!=NULL){
        if(memlist->busylisthead==memblock)
            memlist->busylisthead=memblock->next;
        else
            memblock->prev->next=memblock->next;
        if(memlist->busylisttail==memblock)
            memlist->busylisttail=memblock->prev;
        else
            memblock->next->prev=memblock->prev;
        if (memlist->freelisthead==NULL) {
            memblock->prev=memblock->next=NULL;
            memlist->freelisthead=memlist->freelisttail=memblock;
        }
        else{
            memblock->prev=memlist->freelisttail;
            memblock->next=NULL;
            memlist->freelisttail=memblock;
            memlist->freelisttail=memblock;
        }
        return true;
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

void MemPool::setBlockMeta(void *&buff,size_t alignbytes,MemList *&curlist,MemBlock *&curblock){
    MemBlockMeta *meta=(MemBlockMeta *)buff;
    meta->init();
    meta->size=alignbytes;
    meta->mark=false;
    meta->curlist=curlist;
    meta->curblock=curblock;
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

void MemPool::destroy(){
    for(MemList *memlist=firstlist;memlist!=NULL;){
        for(MemBlock *memblock=memlist->busylisthead;memblock!=NULL;){
            MemBlock *tmp=memblock;
            memblock=memblock->next;
            free(tmp->block);
            free(tmp);
        }
        for(MemBlock *memblock=memlist->freelisthead;memblock!=NULL;){
            MemBlock *tmp=memblock;
            memblock=memblock->next;
            free(tmp->block);
            free(tmp);
        }
        MemList *tmp=memlist;
        memlist=memlist->nextlist;
        free(tmp);
        firstlist=NULL;
        lastlist=NULL;
    }
}

void MemList::init(){
    nextlist=NULL;
    freelisthead=freelisttail=NULL;
    busylisthead=busylisttail=NULL;
    size=0;
}

void MemBlock::init(){
    prev=next=NULL;
    block=NULL;
}

void MemBlockMeta::init(){
    mark=false;
    size=0;
}

//void *operator new(size_t size){
//    cout<<"Using modified new operator!"<<endl;
//    void *buff=MemPool::getInstance()->alloc(size);
//    return buff;
//}
//
//void operator delete(void *buff){
//    if(!MemPool::getInstance()->dealloc(buff))
//        throw MemoryError(curmodname,curline);
//}

//inline void operator delete[](void *buff){
//    
//}
