struct Arena {
    void* memory;       // Pointer to the memory block
    size_t size;        // Total size of the arena
    size_t used;        // Amount of memory used
    int active_objects; // Counter for active objects

    Arena(size_t block_size) 
        : memory(malloc(block_size)), size(block_size), used(0), active_objects(0) {}

    ~Arena() {
        free(memory);
    }

    bool can_allocate(size_t size, size_t alignment) const {
        size_t adjust = alignment - (used % alignment);
        adjust = (adjust == alignment) ? 0 : adjust;
        return (used + adjust + size) <= this->size;
    }
};

class ArenaAllocator {
private:
    std::vector<std::unique_ptr<Arena>> arenas;
    size_t default_arena_size;

public:
    ArenaAllocator(size_t default_size = 4096) 
        : default_arena_size(default_size) {}

    // Allocates memory from a suitable arena
    void* allocate(size_t size, size_t alignment = alignof(std::max_align_t)) {
        // Search for an arena with enough free space
        for (auto& arena : arenas) {
            if (arena->can_allocate(size, alignment)) {
                return allocate_in_arena(*arena, size, alignment);
            }
        }

        // Create a new arena if none are suitable
        auto new_arena = std::make_unique<Arena>(default_arena_size);
        arenas.push_back(std::move(new_arena));
        return allocate_in_arena(*arenas.back(), size, alignment);
    }

    // Deallocates an object (decrements counter and resets arena if needed)
    static void deallocate(void* ptr) {
        Arena* arena = get_arena_from_pointer(ptr);
        arena->active_objects--;

        // If no active objects remain, reset arena usage
        if (arena->active_objects == 0) {
            arena->used = 0; // Memory is ready for new allocations
        }
    }

private:
    // Internal allocation in a specific arena
    void* allocate_in_arena(Arena& arena, size_t size, size_t alignment) {
        size_t adjust = alignment - (arena.used % alignment);
        adjust = (adjust == alignment) ? 0 : adjust;

        assert(arena.used + adjust + size <= arena.size && "Arena overflow");

        void* ptr = static_cast<char*>(arena.memory) + arena.used + adjust;
        arena.used += adjust + size;
        arena.active_objects++;
        return ptr;
    }

    // Retrieve arena from pointer (object header)
    // Small trick: the pointer stores a reference to the arena
    static Arena* get_arena_from_pointer(void* ptr) {
        // Simplified: assume the arena pointer is stored before the allocation (may vary by implementation)
        return reinterpret_cast<Arena*>(static_cast<char*>(ptr) - sizeof(Arena*));
    }
};
