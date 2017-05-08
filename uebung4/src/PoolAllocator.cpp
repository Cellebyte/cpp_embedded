/*
 *   @author: Marcel Fest
*/

#include "PoolAllocator.h"

inline void* Heap::Allocate(size_t size)
{
    void* allocated = nullptr;
    if(static_cast<size_t>(0) == size) return nullptr;
    size_t blocks = size / block_size;
    size_t rest = size % block_size;
    for(size_t block=0; block < block_count; block++)
    {
        //Difficult Case of size bigger than block_size
        if(block_size <= size)
        {
            if(0 != rest)
            {
                blocks++;
            }
            if(blocks > block_count)
            {
                return nullptr;
            }
            if(IsChainable(block,blocks))
            {
                return CreateBlockChain(block,blocks);
            }
        }
        //Easy Case of size equal or less than block_size
        else
        {
            if(! slice[block].allocated)
            {
                allocated = static_cast<void*>(slice[block].begin);
                slice[block].allocated = true;
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
            //Easy Case;
            if(! slice[block].first)
            {
                //#DEBUG printf("I am not a chain\n");
                slice[block].allocated = false;
                return;
            }
            //Difficult Case;
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
inline bool Heap::IsChainable(int block, size_t blocks)
{
    //lookahead if chain fit from block with ammount of blocks
    for(size_t chain = 0; chain < blocks; chain++)
    {
        //#DEBUG printf("Block: %u\n", block + chain);
        if(slice[block + chain].allocated)
        {
            return false;
        }
    }
    return true;
}

inline void* Heap::CreateBlockChain(int block, size_t blocks)
{
    slice[block].first = true;
    void* allocated = static_cast<void*>(slice[block].begin);

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
