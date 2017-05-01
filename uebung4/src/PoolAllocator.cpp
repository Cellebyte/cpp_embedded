/*
 *   @author: Marcel Fest
*/

#include "PoolAllocator.h"

inline void* Heap::Allocate(size_t size)
{
    size++;
    if (0 == storage_head)
            ExpandPoolSize ();

    storage* head = storage_head;
    storage_head = head->next;
    blockCount--;
    return head;
}
inline size_t Heap::Available() const
{
    return blockCount;
}
inline void Heap::Deallocate(void* deleted)
{
    storage* head = static_cast <storage*> (deleted);
    head->next = storage_head;
    storage_head = head;
    blockCount++;
}
