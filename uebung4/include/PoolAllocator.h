/*
 *  @author Marcel Fest
 */
#ifndef POOLALLOCATOR_H
#define POOLALLOCATOR_H
#define CREATE (varName, blockCount, blockSize) Heap varName = Heap(blockCount,blockSize);
#include <cstddef>
#include <cstdint>
#include <cstdlib>

class IHeap
{
    public:
        virtual void * Allocate (size_t sizeInBytes) = 0;
        virtual void Deallocate (void *)= 0;
        /* Returns remaining # of available bytes */
        virtual size_t Available () const = 0;
};

class Heap: public IHeap
{
    struct storage
    {
        storage *next;
    };
    private:
        void ExpandPoolSize ();
        void CleanUp ();
        storage* storage_head;
        int blockCount;
        size_t blockSize

    public:
        Heap (int blockCount, size_t blockSize)
        {
            blockCount = blockCount;
            blockSize = blockSize;
            storage_head = 0;
            ExpandPoolSize ();
        }
        virtual ~Heap()
        {
            CleanUp ();
        }
        virtual void* Allocate(size_t);
        virtual void  Deallocate(void*);
        virtual size_t Available() const = 0;
};
#endif
