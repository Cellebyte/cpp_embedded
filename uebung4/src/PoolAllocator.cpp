/*
 *   @author: Marcel Fest
*/
#include "PoolAllocator.h"

inline void* Heap::Allocate(size_t size)
{
    if((size > block_size) || (static_cast<size_t>(0) == size)) return nullptr;

    for(size_t block=0; block < block_count; block++)
    {
        if(! slice[block].allocated)
        {
            slice[block].allocated = true;
            return static_cast<void*>(pool+(block_size*block));
        }
    }
    return nullptr;
}
inline size_t Heap::Available() const
{
    size_t temp = 0;
    for(size_t block=0; block < block_count; block++)
    {
        if(! slice[block].allocated)
        {
            temp = temp + block_size;
        }
    }
    return temp;
}
inline void Heap::Deallocate(void* deleted)
{
    if (nullptr == deleted) return;
    for(size_t block=0; block < block_count; block++)
    {
        if(pool+(block_size*block) == static_cast<uint8_t*>(deleted))
        {
            slice[block].allocated = false;
            return;
        }
    }
}
