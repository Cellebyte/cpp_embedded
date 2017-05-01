/*
 *   @author: Marcel Fest
*/

#include "PoolAllocator.h"
class Heap: public IHeap
{
    struct FreeStore
    {
        FreeStore *next;
    };
    void expandPoolSize ();
    void cleanUp ();
    FreeStore* freeStoreHead;
    size_t allocated_size
    public:
    Heap ()
    {
        freeStoreHead = 0;
        expandPoolSize ();
    }
    virtual ~Heap ()
    {
        cleanUp ();
    }
    virtual void* Allocate(size_t);
    virtual void  Deallocate(void*);
    size_t Available () const
    {
        return allocated_size;
    }
};

inline void* Heap::Allocate(size_t size)
{
    if (0 == freeStoreHead)
    expandPoolSize ();

    FreeStore* head = freeStoreHead;
    freeStoreHead = head->next;
    return head;
}
inline void Heap::Deallocate(void* deleted)
{
    FreeStore* head = static_cast <FreeStore*> (deleted);
    head->next = freeStoreHead;
    freeStoreHead = head;
}
void Heap::expandPoolSize ()
{
    size_t size = (sizeof(Complex) > sizeof(FreeStore*)) ?
    sizeof(Complex) : sizeof(FreeStore*);
    FreeStore* head = reinterpret_cast <FreeStore*> (new char[size]);
    freeStoreHead = head;

    for (int i = 0; i < POOLSIZE; i++) {
    head->next = reinterpret_cast <FreeStore*> (new char [size]);
    head = head->next;
}

head->next = 0;
}

void Heap::cleanUp()
{
FreeStore* nextPtr = freeStoreHead;
for (; nextPtr; nextPtr = freeStoreHead) {
freeStoreHead = freeStoreHead->next;
delete [] nextPtr; // remember this was a char array
}
}
