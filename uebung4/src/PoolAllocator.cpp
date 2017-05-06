/*
 *   @author: Marcel Fest
*/

#include "PoolAllocator.h"

inline void* Heap::Allocate(size_t size)
{
    void* allocated = nullptr;
    if(static_cast<size_t>(0) == size) return nullptr;
    if(block_size >= size)
    {
        for(size_t block=0; block < block_count; block++)
        {
            if(! slice[block].allocated)
            {
                allocated = static_cast<void*>(slice[block].begin);
                slice[block].allocated = true;
                break;
            }
        }
        return allocated;
    }
    else
    {
        size_t blocks = size / block_size;
        size_t rest = size % block_size;
        if(blocks > block_count)
        {
            return nullptr;
        }
        if(static_cast<size_t>(0) != rest)
        {
            blocks++;
        }
        //#DEBUG printf("Blocks: %d\tRest: %d\n",blocks,rest);
        for(size_t block = 0; block < blocks; block++)
        {
            //#DEBUG printf("Chainable?\n");
            bool chainable = true;
            for(size_t chain = 0; chain < blocks; chain++)
            {
                //#DEBUG printf("Block: %u\n", block + chain);
                if(slice[block + chain].allocated)
                {
                    chainable = false;
                    break;
                }
            }
            if(chainable)
            {
                //#DEBUG printf("Chainable Yes\n");
                slice[block].first = true;
                allocated = static_cast<void*>(slice[block].begin); // first block of blockchain

                for(size_t chain = 0; chain < blocks; chain++)
                {
                    //#DEBUG printf("Chain: %u\n",chain);
                    //#DEBUG printf("Block: %u\n", block + chain);
                    slice[(block + chain)].allocated = true;

                    if(chain == blocks-1)
                    {
                        //#DEBUG printf("If:\n");
                        slice[(block + chain)].next = nullptr;
                        //#DEBUG printf("Block: %u\n", block + chain);
                        break;
                    }
                    else
                    {
                        //#DEBUG printf("Else:\n");
                        slice[block + chain].next = &slice[block + chain + 1];
                        //#DEBUG printf("Next_block: %x\n",slice[block + chain].next->begin);
                    }

                }
                return allocated;
            }
        }
    }
    return allocated;
}
inline size_t Heap::Available() const
{
    size_t temp = 0;
    for(size_t block=0; block < block_count; block++)
    {
        if(! slice[block].allocated)
        {
            temp = temp + slice[block].size;
        }
    }
    return temp;
}
inline void Heap::Deallocate(void* deleted)
{
    if (nullptr == deleted) return;
    //#DEBUG printf("Before loop\n");
    for(size_t block=0; block < block_count; block++)
    {
        if(slice[block].begin == static_cast<uint8_t*>(deleted))
        {
            if(! slice[block].first)
            {
                //#DEBUG printf("I am not a chain\n");
                slice[block].allocated = false;
                return;
            }
            slice[block].first = false;
            slice[block].allocated = false;
            Block* temp = &slice[block];
            Block* temp2 = static_cast<Block*>(nullptr);
            while(temp->next)
            {
                //#DEBUG printf("This is Block:\t %d\n",block+1);
                temp2 = temp;
                temp = temp->next;
                temp->allocated = false;
                temp2->next = static_cast<Block*>(nullptr);

            }
        }
    }
    return;
}
