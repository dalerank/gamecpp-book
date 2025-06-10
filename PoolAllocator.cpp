class PoolAllocator {
    struct Block {
        Block* next;
    };

    void* memoryPool;      // Выделенная память
    Block* freeList;       // Список свободных блоков
    size_t blockSize;      // Размер одного блока
    size_t totalBlocks;    // Общее количество блоков

public:
    PoolAllocator(size_t objectSize, size_t numObjects) {
        // Делаем размер блока под выравнивание и размер указателя
        blockSize = (objectSize < sizeof(Block)) ? sizeof(Block) : objectSize;
        totalBlocks = numObjects;

        // Большой блок
        memoryPool = std::malloc(blockSize * totalBlocks);
        
        // Маленькие блоки
        freeList = static_cast<Block*>(memoryPool);
        Block* current = freeList;
        for (size_t i = 0; i < totalBlocks - 1; ++i) {
            current->next = reinterpret_cast<Block*>(reinterpret_cast<char*>(current) + blockSize);
            current = current->next;
        }
        current->next = nullptr;
    }

    ~PoolAllocator() {
        std::free(memoryPool);
    }

    void* allocate() {
        if (!freeList) return nullptr; // Пул пуст
        Block* allocatedBlock = freeList;
        freeList = freeList->next;
        return allocatedBlock;
    }

    void deallocate(void* ptr) {
        if (!ptr) return;
        Block* freedBlock = static_cast<Block*>(ptr);
        freedBlock->next = freeList;
        freeList = freedBlock;
    }
};
