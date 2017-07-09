#ifndef mempool_h
#define mempool_h

#include <unordered_map>
#include <vector>
#include "memblock.h"

using namespace std;

typedef unsigned short ushort;

class MemPool{
public:
    static MemPool *getInstacne();
    ~MemPool();
    void *alloc(int);
    void free(void *);
protected:
    MemPool(int initBlockSize=1024,int growBlockSize=256);
    static MemPool *mempool;
private:
    unordered_map<int, int> blockmap;//内存块映射表
    static ushort index;
    
    const int ALIGN=8;
    
    int initsize;
    int growsize;
    
    unsigned first;//内存池索引
    unsigned last;
    
    vector<MemBlock*> memblocks;
    MemBlock **ptr;
    
    int hash(int);//计算内存块哈希
    int alignBytes(int);//对齐字节
    int getSize(int);//返回分配块大小
    
};

#endif
