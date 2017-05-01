/*
 *   @author: Marcel Fest
*/

#include "PoolAllocator.h"

inline void* Heap::Allocate(size_t size)
{
    if (0 == storage_head)
            expandPoolSize ();

    storage* head = storage_head;
    storage_head = head->next;
    blockCount--;
    return head;
}
inline size_t Heap::Available()
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
void Heap::ExpandPoolSize ()
{
    size_t size = (sizeof(blockSize) > sizeof(storage*)) ?
    sizeof(blockSize) : sizeof(storage*);

    storage* head = reinterpret_cast <storage*> (new char[size]);
    storage_head = head;

    for (int i = 0; i < blockCount; i++)
    {
        head->next = reinterpret_cast <storage*> (new char [size]);
        head = head->next;
    }

    head->next = 0;
}

void Heap::cleanUp()
{
    storage* temp = storage_head;
    for (; temp; temp = storage_head)
    {
        storage_head = storage_head->next;
        delete [] temp; // remember this was a char array
    }
}
