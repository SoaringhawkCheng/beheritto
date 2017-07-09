#ifndef memblock_hpp
#define memblock_hpp

#include <stdio.h>

typedef unsigned short ushort;

struct MemBlock{
    MemBlock(ushort,ushort);
    static void *operator new(size_t,ushort,ushort);
    static void operator delete(void *);
    int size;
    int free;
    int first;
    char mpad[2];
    MemBlock *prev;
    MemBlock *next;
};

#endif
