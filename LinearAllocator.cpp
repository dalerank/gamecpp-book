struct LinearAllocator {
 void* base_pointer;
 size_t size;
 size_t offset;

 void* allocate(size_t alloc_size) {
   if (offset + alloc_size > size) return nullptr;
   void* ptr = static_cast<char*>(base_pointer) + offset;
   offset += alloc_size;
   return ptr;
 }

 void reset() {
   offset = 0;
 }
};

