template <typename T, size_t COUNT, size_t FIXEDSIZE = COUNT * sizeof(T)>
struct FixedMemoryResource : public std::pmr::memory_resource
{
    void* do_allocate(size_t bytes, size_t align) override
    {
        // Check if free memory in fixed buffer - else just call new
        void* currBuffer = &mFixedBuffer[FIXEDSIZE - mAvailableFixedSize];
        if (std::align(align, bytes, currBuffer, mAvailableFixedSize) 
             == nullptr)
        {
            assert(false && "allocate more that expected");
            return ::operator 
                new(bytes, static_cast<std::align_val_t>(align));
        }
        mAvailableFixedSize -= bytes;
        return currBuffer;
    }

    void do_deallocate(void* ptr, size_t bytes, size_t align) override
    {
        if (ptr < &mFixedBuffer[0] || ptr >= &mFixedBuffer[FIXEDSIZE])
        {
            ::operator 
                delete(ptr, bytes, static_cast<std::align_val_t>(align));
        }
    }

    bool do_is_equal(const std::pmr::memory_resource& other) const 
          override { return this == &other; }

  private:
    alignas(T) uint8_t mFixedBuffer[FIXEDSIZE]; // fixed-size buffer
    std::size_t mAvailableFixedSize = FIXEDSIZE; // Current available size
};
