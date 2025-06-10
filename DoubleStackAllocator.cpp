class DoubleStackAllocator {
public:
    DoubleEndedStackAllocator(size_t size)
        : totalSize(size), start(nullptr), end(nullptr), current(nullptr), reverse(nullptr) {
        start = malloc(size);
        end = (char*)start + size;
        current = start;
        reverse = end;
    }

    ~DoubleEndedStackAllocator() {
        free(start);
    }

    void* allocateFromStart(size_t size) {
        // Проверка на переполнение стека с начала
        if ((char*)current + size <= (char*)reverse) {
            void* result = current;
            current = (char*)current + size;
            return result;
        }
        return nullptr; // Нет достаточно памяти
    }

    void* allocateFromEnd(size_t size) {
        // Проверка на переполнение стека с конца
        if ((char*)reverse - size >= (char*)current) {
            reverse = (char*)reverse - size;
            return reverse;
        }
        return nullptr; // Нет достаточно памяти
    }

    void reset() {
        current = start;
        reverse = end;
    }

private:
    size_t totalSize;
    void* start;
    void* end;
    void* current;
    void* reverse;
};
