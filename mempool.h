#ifndef mempool_h
#define mempool_h

#include <vector>

#include "multithread.h"

using namespace std;

class MemList;
class MemBlock;

class MemPool{
public:
    static MemPool &getInstance();
    ~MemPool();
    void *alloc(size_t size);
    bool free(void *,size_t size);
private:
//    MemPool();
    static MemPool mempool;
    vector<MemList *> lists;
//    MemList *firstlist;//第一个内存管理链表的指针
//    MemList *lastlist;//最后一个内存管理链表的指针
    Mutex mutex;
};

class MemList{
public:
    MemList():freehead(NULL),freeend(NULL),usedhead(NULL),usedend(NULL){}
    MemBlock *freehead;//未分配内存块链表头
    MemBlock *freeend;//未分配内存块链表尾
    MemBlock *usedhead;//已分配内存块链表尾
    MemBlock *usedend;//已分配内存块链表尾
    int size;
};

class MemBlock{
public:
    MemBlock():prev(NULL),next(NULL),block(NULL){}
    /*内存块，双向链表*/
    MemBlock *prev;
    MemBlock *next;
    void *block;
};

class BlockMeta{
public:
    BlockMeta();
    bool marked;
private:
    ~BlockMeta();
};

#endif
