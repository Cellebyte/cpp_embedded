/*
 *  @author Marcel Fest
 */
#ifndef POOLALLOCATOR_H
#define POOLALLOCATOR_H
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstdio>

class IHeap
{
    public:
        virtual void * Allocate (size_t) = 0;
        virtual void Deallocate (void *)= 0;
        /* Returns remaining # of available bytes */
        virtual size_t Available () const = 0;
};

struct Block
{
    /*  Information on pool */
    bool allocated = false;
    uint8_t* storage = nullptr;
};

class Heap: public IHeap
{
    private:
        /* bytewise memory, will by only given out as blocks of blocksize */
        /* bitmap of blocks, true=alocated, false=free */
        Block* const sliced;
        const size_t block_count;
        const size_t block_size;
    protected:
        Heap(
            Block* sliced,\
            size_t block_count,\
            size_t block_size\
        ):
            sliced(sliced),\
            block_count(block_count),\
            block_size(block_size)
            {}
    public:
        /*
        *   @param <size_t> sizeInBytes
        *   @return <void*> memory  or nullptr if sizeInBytes
        *                                       larger the blockSize
        *                                       or Pool is full
        */
        void * Allocate (size_t sizeInBytes);
        /*
        *   @param <void*> block pointer of allocated
        *   try to delete given blockpointer
        */
        void Deallocate (void * block);
        /* Returns remaining # of available bytes */
        size_t Available () const;
};

template <size_t blockCount, size_t blockSize>
class PoolAllocator: public Heap
{
    private:
        Block sliced [blockCount];
        uint8_t storage [blockSize*blockCount] = {0};
    public:
        PoolAllocator() : Heap(sliced, blockCount, blockSize) {
            for(size_t block = 0; block < blockCount; block++)
            {
                sliced[block].storage = storage+(block*blockSize);
            }
        }
};
#endif
