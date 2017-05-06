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
    bool allocated;
    bool first;         //set by chain allocation size >= blockSize
    size_t size;        //sizeof block
    uint8_t* begin;     // pointer to begin of block in pool
    uint8_t* end;       // pointer to end of block in pool
    Block* next;        //set by chain allocation for next block in chain

    /*#DEBUG
    void getInfo()
    {
        printf("\tisAllocated:\t%d (0=False,1=True)\n\
                isFirst:\t\t%d\n\
                size:\t\t%d\n\
                begin:\t\t%x\n\
                end:\t\t%x\n\
                next:\t\t%x\n",\
                allocated,\
                first,\
                size,\
                begin,\
                end,\
                next\
            );
    }
    */
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
    public:
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
                slice[block].first = false;
                slice[block].size = block_size;
                //#DEBUG printf("Block: %u",block+1);
                if( static_cast<size_t>(-1) != block - 1)
                {
                    slice[block].begin = (slice[block-1].end + 1);
                    slice[block].end = (slice[block].begin + block_size-1); //Offset Fix
                }
                else
                {
                    //different allocation first block
                    slice[block].begin = pool;
                    slice[block].end = (pool+block_size);
                }
                slice[block].next = nullptr;
                //no chains on begin
                //#DEBUG slice[block].getInfo();
            }
            /*
            //#DEBUG
            printf("Start");
            slice[0].getInfo();
            printf("Start_Array = %x\n",pool );
            printf("End");
            slice[block_count-1].getInfo();
            printf("End_Array = %x\n",pool+(block_count*block_size));
            */
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
        //Byte pool for allocation
        uint8_t pool [blockSize * blockCount] = {0};
        //management Array
        Block slice [blockCount];
    public:
        Pool() : Heap(pool, slice, blockCount, blockSize) {}
};
#endif
