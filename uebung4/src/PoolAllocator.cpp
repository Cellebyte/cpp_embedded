/*
 *   @author: Marcel Fest
*/

#include "PoolAllocator.h"

inline void* Heap::Allocate(size_t)
{
    return 0;
}
inline size_t Heap::Available() const
{
    return 0;
}
inline void Heap::Deallocate(void*)
{

}
