/*
 *  @author Marcel Fest
 */
#ifndef POOLALLOCATOR_H
#define POOLALLOCATOR_H
#define CREATE(varName, blockCount, blockSize) Pool<blockCount, blockSize> varName;
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
    bool allocated;
};

class Heap: public IHeap
{
    private:
        /* bytewise memory, will by only given out as blocks of blocksize */
        uint8_t* const pool;
        /* bitmap of blocks, true=alocated, false=free */
        Block* const slice;
        const size_t block_count;
        const size_t block_size;
    protected:
        Heap(
            uint8_t* pool,\
            Block* slice,\
            size_t block_count,\
            size_t block_size\
        ):
            pool(pool),\
            slice(slice),\
            block_count(block_count),\
            block_size(block_size)\
        {
            for(size_t block = 0; block < block_count; block++)
            {
                slice[block].allocated = false;
            }
            printf("2.Block:%x\n",pool+10);
            printf("Start:\t%x\tEnde:\t%x\n",pool, pool+(block_size*block_count) );
        }
    public:
        /*
        *   @param <size_t> sizeInBytes
        */
        void * Allocate (size_t sizeInBytes);
        /*
        *   @param <void*> block
        *   * try to delete given block | blockchain
        */
        void Deallocate (void * block);
        /* Returns remaining # of available bytes */
        size_t Available () const;
};

template <size_t blockCount, size_t blockSize>
class Pool: public Heap
{
    private:
        //Byte pool for allocation
        uint8_t pool [blockSize * blockCount] = {0};
        //management Array
        Block slice [blockCount];
    public:
        Pool() : Heap(pool, slice, blockCount, blockSize) {}
};
#endif
