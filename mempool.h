#ifndef mempool_h
#define mempool_h

#include <stdlib.h>
#include <vector>
#include <iterator>

#include "error.h"
#include "astree.h"
#include "mylist.h"

class GarbageCollector;


#define METASIZE 4

using namespace std;

class MemList;
class MemBlock;

class MemPool{
    friend class GarbageCollector;
public:
    ~MemPool();
    static MemPool *getInstance();
    void *alloc(size_t size);
    bool dealloc(void *buff);
private:
    MemPool()=delete;
    MemPool &operator=(const MemPool&)=delete;
    MemPool(const MemPool&)=delete;
    void init();
    void destroy();
    size_t alignBytes(size_t size);
    void setBlockMeta(void *&buff,size_t alignbytes,MemList *&curlist,MemBlock *&curblock);
    void jumpBlockMeta(void *&buff);
    void getBlockMeta(void *&buff);
private:
    static MemPool *mempool;
    MemList *firstlist;
    MemList *lastlist;
    GarbageCollector *gc;
    int count;
};

class MemList{
public:
    void init();
    MemList *nextlist;
    MemBlock *freelisthead;
    MemBlock *freelisttail;
    MemBlock *busylisthead;
    MemBlock *busylisttail;
    size_t size;
};

class MemBlock{
public:
    void init();
    void *block;
    MemBlock *prev;
    MemBlock *next;
};

class MemBlockMeta{
public:
    void init();
    MemList *curlist;
    MemBlock *curblock;
    bool mark;
    size_t size;
};

#endif
