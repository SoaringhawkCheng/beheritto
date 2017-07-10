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

void *operator new(size_t size);

void operator delete(void *buff);

class MemPool{
public:
    friend class GarbageCollector;
    static MemPool *getInstance();
    ~MemPool();
    void *alloc(size_t size);
    bool dealloc(void *buff);
private:
    MemPool();
    MemPool &operator=(MemPool&);
    MemPool(const MemPool&);
    void init();
    size_t alignBytes(size_t size);
    void setBlockMeta(void *&buff,size_t alignbytes);
    void jumpBlockMeta(void *&buff);
    void getBlockMeta(void *&buff);
    void destroy();
private:
    static MemPool *mempool;
    MyList<MemList *> lists;
    GarbageCollector *gc;
    int count;
};

class MemList{
public:
    void init();
//    void destroy();
    MyList<MemBlock *> used;
    MyList<MemBlock *> unused;
    MemList *prev;
    MemList *next;
//    MemBlock *busylist;
//    MemBlock *busylistend;
//    MemBlock *freelist;
//    MemBlock *freelistend;
    size_t size;
};

class MemBlock{
public:
    void init();
//    void destroy();
    void *block;
    MemBlock *prev;
    MemBlock *next;
};

class MemBlockMeta{
public:
    bool mark;
    size_t size;
};

#endif
