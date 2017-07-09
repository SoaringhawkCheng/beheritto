/*
 #ifndef memorypool_h
#define memorypool_h

#include <map>

using namespace std;

//全局内存池使用单例类
class MemoryPool{
public:
    void *Alloc(int);//分配内存
    void FreeAlloc(void*);//释放内存
    static MemoryPool *GetInstance();//返回全局内存池
    ~MemoryPool();
protected:
    static MemoryPool *memoryPoolInstance;
    MemoryPool(int initBlockSize=1024,int growBlockSize=256);
private:
    static unsigned short index;//
    map<int,int> blockmap;//分配不同内存块时对应的映射表
    const int ALIGNMENT=8;//内存池对齐大小
    int initBlockCount;//初始化内存块
    int growBlockCount;//内存块不足增长的块数
    unsigned first;//第一个子内存池下标
    unsigned last;//最后一个内存池下标
    //最多16中不同内存块大小，也就是说子内存池最多有16
    MemoryBlock *memoryHashMap[16];
    MemoryBlock **mpPtr;//
    int hash(int);//计算不同内存块对应的hashcode
    int alignBytes(int);//对齐字节
    int getUnitSize(int);//返回分配块大小
};

#endif
*/
*/
