/*
 *  @author Marcel Fest
 */
#ifndef POOLALLOCATOR_H
#define POOLALLOCATOR_H
#define CREATE(varName, blockCount, blockSize) Pool<blockCount, blockSize> varName;
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

struct Block
{
    bool allocated;
    size_t size;
    uint8_t* begin;
    uint8_t* end = begin + size -1;
};

class Heap: public IHeap
{
    private:
        /* bytewise memory, will by only given out as blocks of blocksize */
        uint8_t* const pool;
        /* bitmap of blocks, true=alocated, false=free */
        Block* const sliced;
        const size_t block_count;
        const size_t block_size;
    public:
        Heap(uint8_t* pool, Block* sliced, size_t block_count, size_t block_size): pool(pool), sliced(sliced), block_count(block_count), block_size(block_size)
        {
            
        }
        void * Allocate (size_t sizeInBytes);
        void Deallocate (void *);
        /* Returns remaining # of available bytes */
        size_t Available () const;
};

template <size_t blockCount, size_t blockSize>
class Pool: public Heap
{
    private:
        uint8_t pool [blockSize * blockCount] = {0};
        Block sliced [blockCount];
    public:
        Pool() : Heap(pool, sliced, blockCount, blockSize) {}
};
#endif
