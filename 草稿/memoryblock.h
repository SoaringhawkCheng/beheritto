/*#ifndef memoryblock_h
#define memoryblock_h

#include <iostream>

class MemoryBlock{
public:
    MemoryBlock();
    MemoryBlock(unsigned short,unsigned short);
    static void *operator new(unsigned,unsigned short,unsigned short);
    static void operator delete(void *);
    MemoryBlock *prev;
    MemoryBlock *next;
    int totalsize;
    int free;
    int firstindex;
    char pad[2];
};

#endif*/
