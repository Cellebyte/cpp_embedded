/*
 *  @author Marcel Fest
 */

#include "PoolAllocator.h"
#include <cstdio>
int main()
{
    const int size_20 = 20;
    const int size_50 = 50;
    const int size_100 = 100;
    CREATE(test_20,15,static_cast<size_t>(size_20));
    CREATE(test_50,15,static_cast<size_t>(size_50));
    CREATE(test_100,15,static_cast<size_t>(size_100));
    printf("%x%x%x",test_20,test_50,test_100);
    void* a [15] = {0};
    void* b [15] = {0};
    void* c [15] = {0};
    for(int i = 0; i < 15; i++)
    {
        printf("\n\tblock_size = :\t%d\n", i*10);
        printf("\tAvailable Space (block_size = %d):\t%u\n",size_20,test_20.Available());
        printf("\tAvailable Space (block_size = %d):\t%u\n",size_50,test_50.Available());
        printf("\tAvailable Space (block_size = %d):\t%u\n",size_100,test_100.Available());
        printf("\n\tAllocating...\n\n");
        a[i] = test_20.Allocate(static_cast<size_t>(i*10));
        b[i] = test_50.Allocate(static_cast<size_t>(i*10));
        c[i] = test_100.Allocate(static_cast<size_t>(i*10));
        printf("\tAvailable Space (block_size = %d):\t%u\n",size_20,test_20.Available());
        printf("\tAvailable Space (block_size = %d):\t%u\n",size_50,test_50.Available());
        printf("\tAvailable Space (block_size = %d):\t%u\n",size_100,test_100.Available());
    }
    printf("\tAllocation Adresses:\n");
    for(int i = 0; i < 15; i++)
    {
        printf("\tblock_size = :\t%d\n", i*10);
        printf("\t\tAllocated Address (block_size = %d):\t%x\n",size_20,a[i]);
        printf("\t\tAllocated Address (block_size = %d):\t%x\n",size_50, b[i]);
        printf("\t\tAllocated Address (block_size = %d):\t%x\n",size_100,c[i]);
    }
    for(int i = 0; i < 15; i++)
    {
        printf("%d <- Array Position\n",i);
        printf("\tAvailable Space (block_size = %d):\t%u\n",size_20,test_20.Available());
        printf("\tAvailable Space (block_size = %d):\t%u\n",size_50,test_50.Available());
        printf("\tAvailable Space (block_size = %d):\t%u\n",size_100,test_100.Available());
        printf("\n\tDeallocating...\n\n");
        test_20.Deallocate(a[i]);
        test_50.Deallocate(b[i]);
        test_100.Deallocate(c[i]);
        printf("\tAvailable Space (block_size = %d):\t%u\n",size_20,test_20.Available());
        printf("\tAvailable Space (block_size = %d):\t%u\n",size_50,test_50.Available());
        printf("\tAvailable Space (block_size = %d):\t%u\n",size_100,test_100.Available());
    }
}
