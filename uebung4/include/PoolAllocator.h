/*
 *  @author Marcel Fest
 */
#ifndef POOLALLOCATOR_H
#define POOLALLOCATOR_H

#include <cstddef>
#include <cstdint>
#include <cstdlib>

# define CREATE(varName, blockCount, blockSize) IHeap<blockCount> varName = IHeap<blockCount>::Allocate(blockSize);

template <size_t blockCount>
class IHeap
{
    public:
        virtual void * Allocate (size_t sizeInBytes) = 0;
        virtual void Deallocate (void *)= 0;
        /* Returns remaining # of available bytes */
        virtual size_t Available () const = 0;
    private:
        size_t size;
        void* block_allocated[blockCount];
};

template<size_t blockCount>
void* IHeap<blockCount>::Allocate(size_t sizeInBytes)
{
    size = sizeInBytes;
    for(size_t i = 0; i < blockCount; i++)
    {
        block_allocated[i]=alloca(sizeof(int8_t)*sizeInBytes);
    }
    return block_allocated;
}
template<size_t blockCount>
void IHeap<blockCount>::Deallocate(void* block_allocated)
{
    for(size_t i = 0; i< blockCount; i++)
    {
        free(block_allocated);
    }
}
template<size_t blockCount>
size_t IHeap<blockCount>::Available() const
{
    return blockCount*size;
}

#endif
