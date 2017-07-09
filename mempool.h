#ifndef mempool_h
#define mempool_h

#include <stdlib.h>
#include <vector>
#include <list>
#include <iterator>

class GarbageCollector;

#define METASIZE 4

using namespace std;

class MemList;
class MemBlock;

class MemPool{
public:
    friend class GarbageCollector;
    static MemPool *getInstance();
    ~MemPool();
    void destroy();
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
private:
    static MemPool *mempool;
    vector<MemList *> lists;
    GarbageCollector *gc;
    int count;
};

class MemList{
public:
    list<MemBlock *> used;
    list<MemBlock *> unused;
    int size;
};

class MemBlock{
public:
    void *block;
};

class MemBlockMeta{
public:
    bool mark;
    size_t size;
};

#endif
