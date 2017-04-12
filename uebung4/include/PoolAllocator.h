/*
 *  @author Marcel Fest
 */
#ifndef POOLALLOCATOR_H
#define POOLALLOCATOR_H

#include <cstddef>

# define CREATE(varName, blockCount, blockSize) \
                IHeap<blockCount>::Allocate(blockSize) varName;

template <typename T, size_t blockCount>
class IHeap
{
    public:
        virtual void * Allocate ( size_t sizeInBytes ) = 0;
        virtual void Deallocate ( void *)= 0;
        /* Returns remaining # of available bytes */
        virtual size_t Available () const = 0;
};

template<size_t blockCount>
void* IHeap<blockCount>::Allocate(size_t sizeInBytes)
{
    return sizeof(sizeInBytes) newBlock [blockCount];
}


#endif
