/*
 *  @author Marcel Fest
 */
#ifndef POOLALLOCATOR_H
#define POOLALLOCATOR_H
#define CREATE(varName, blockCount, blockSize) Heap varName = Heap(blockCount,blockSize);
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
    void ExpandPoolSize ()
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
    void CleanUp ()
    {
        storage* temp = storage_head;
        for (; temp; temp = storage_head)
        {
            storage_head = storage_head->next;
            delete [] temp; // remember this was a char array
        }
    }
    storage* storage_head;
    int blockCount;
    size_t blockSize;

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
        void * Allocate (size_t sizeInBytes);
        void Deallocate (void *);
        /* Returns remaining # of available bytes */
        size_t Available () const;
};
#endif
