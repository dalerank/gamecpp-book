class StackAllocator {
public:
    StackAllocator(size_t size) {
        buffer_ = std::malloc(size);
        capacity_ = size;
        current_ = buffer_;
    }

    ~StackAllocator() {
        std::free(buffer_);
    }

    void* allocate(size_t size) {
        const size_t required_space = sizeof(Header) + size;
        if (current_ + required_space > buffer_ + capacity_) {
            return nullptr;
        }

        Header* header = (Header*)current_;
        header->size = size;
        current_ += sizeof(Header);
        void* user_ptr = current_;
        current_ += size;
        return user_ptr;
    }

    void deallocate(void* ptr) {
        if (!ptr) {
            return;
        }
        char* user_ptr = static_cast<char*>(ptr);
        Header* header = (Header*)(user_ptr - sizeof(Header));
        if (user_ptr + header->size != current_) {
            assert("free inproperly");
        }
        current_ = header;
    }

private:
    struct Header {
        size_t size;
    };
    void* buffer_;
    size_t capacity_;
    void* current_;
};
