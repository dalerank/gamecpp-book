class FrameAllocator {
   void* base_pointer;
   size_t size;
   size_t offset;
   uint32_t frame;

   void* allocate(size_t alloc_size, uint32_t f) {
     if (offset + alloc_size > size) 
         return nullptr;
       
     frame = f;
     void* ptr = static_cast<char*>(base_pointer) + offset;
     offset += alloc_size;
     return ptr;
   }

   void reset() {
     offset = 0;
   }
};
