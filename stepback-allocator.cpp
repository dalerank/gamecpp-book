class StepBackAllocator {
public:
    StepBackAllocator(size_t size)
        : buffer(new char[size]), capacity(size), offset(0) {}

    ~StepBackAllocator() {
        delete[] buffer;
    }

    void* allocate(size_t size) {
        if (offset + size > capacity) {
            return nullptr;
        }

        void* ptr = buffer + offset;
        history.push_back(ptr);
        offset += size;
        return ptr;
    }

    void deallocate(void* ptr) {
        if (history.empty()) {
            assert("No allocations to deallocate.");
        }

        if (history.back() == ptr) {
          history.pop_back();
        }
    }

    void reset() {
        offset = 0; 
        history.clear();
    }

private:
    char* buffer;                  
    size_t capacity;               
    size_t offset;                 
    std::vector<void*> history; 
};
