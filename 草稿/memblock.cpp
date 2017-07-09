#include "memblock.h"

MemBlock::MemBlock(ushort num,ushort size)
:prev(NULL),next(NULL),size(num*(size+2)),free(num-1),first(1){
    
}

void *MemBlock::operator new(size_t, ushort num, ushort size){
    return ::operator new(sizeof(MemBlock)+num*(size+2));
}
void MemBlock::operator delete(void *ptr){
    ::operator delete(ptr);
}
