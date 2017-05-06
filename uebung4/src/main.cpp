/*
 *  @author Marcel Fest
 */

#include "PoolAllocator.h"
#include <cstdio>
int main()
{
    CREATE(test,15,10);
    printf("Available Space:\t%u\n",test.Available()); //working
    void* b = test.Allocate(10);
    printf("Allocated Address:\t%x\n", b);
    printf("Available Space:\t%u\n",test.Available());
    test.Deallocate(b);
    void* a = test.Allocate(20);
    printf("Available Space:\t%u\n",test.Available());
    test.Deallocate(a);
    printf("Available Space:\t%u\n",test.Available());
}
