/*
 *   @author: Marcel Fest
*/
#include "PoolAllocator.h"

inline void* Heap::Allocate(size_t size)
{
    if((size > block_size) || (static_cast<size_t>(0) == size)) return nullptr;

    for(size_t block=0; block < block_count; block++)
    {
        if(! sliced[block].allocated)
        {
            sliced[block].allocated = true;
            return static_cast<void*>(sliced[block].storage);
        }
    }
    return nullptr;
}

inline size_t Heap::Available() const
{
    size_t available = 0;
    for(size_t block=0; block < block_count; block++)
    {
        if(! sliced[block].allocated)
        {
            available += block_size;
        }
    }
    return available;
}

inline void Heap::Deallocate(void* deleted)
{
    if (nullptr == deleted) return;
    for(size_t block=0; block < block_count; block++)
    {
        if( sliced[block].storage == static_cast<uint8_t*>(deleted))
        {
            sliced[block].allocated = false;
            return;
        }
    }
}
