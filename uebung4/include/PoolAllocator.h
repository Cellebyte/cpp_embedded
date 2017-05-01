/*
 *  @author Marcel Fest
 */
#ifndef POOLALLOCATOR_H
#define POOLALLOCATOR_H

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
