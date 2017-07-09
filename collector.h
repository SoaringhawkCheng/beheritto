#ifndef collector_h
#define collector_h

#include "mempool.h"
#include "astree.h"

extern int state;

class GarbageCollector{
public:
    static GarbageCollector *getInstance();
    ~GarbageCollector();
private:
    GarbageCollector();
    GarbageCollector(const GarbageCollector &);
    GarbageCollector &operator=(GarbageCollector &);
    void init();
    void mark();//标记
    void sweep();//清除垃圾
    void clear();//清除标记
private:
    void markAnalyze(DeclModule *program);
    void markRuntime(RuntimeStack runtimestack);
    void mark(ASTree *node);
    void sweep(ASTree *node);
private:
    static GarbageCollector *gc;//垃圾收集器
    MemPool *mempool;//指向的内存池
};

#endif
