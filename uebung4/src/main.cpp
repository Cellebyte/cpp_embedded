/*
 *  @author Marcel Fest
 */

#include "PoolAllocator.h"
#include <cstdio>
int main()
{
    int counter=1;
    printf("\t-->Test %d. Normal Behaviour:\n",counter++);
    CREATE(test,15,10);
    printf("\t\tAvailable Space:\t%u\n",test.Available()); //working
    void* a = test.Allocate(10);
    printf("\t\tAllocated Address:\t%x\n", a);
    printf("\t\tAvailable Space:\t%u\n",test.Available());
    void* b = test.Allocate(20);
    printf("\t\tAllocated Address:\t%x\n", b);
    printf("\t\tAvailable Space:\t%u\n",test.Available());
    test.Deallocate(a);
    test.Deallocate(b);
    printf("\t\tAvailable Space:\t%u\n",test.Available());

    printf("\t-->Test %d. Overflow:\n",counter++);
    void* c = test.Allocate(160);
    printf("\t\tAllocated Address:\t%x\n", c);
    printf("\t\tAvailable Space:\t%u\n",test.Available());

    printf("\t-->Test %d. Behaviour on 0:\n",counter++);
    void* d = test.Allocate(0);
    printf("\t\tAllocated Address:\t%x\n", d);
    printf("\t\tAvailable Space:\t%u\n",test.Available());

    printf("\t-->Test %d. Full chain over whole pool:\n",counter++);
    void* e = test.Allocate(150);
    printf("\t\tAllocated Address:\t%x\n", e);
    printf("\t\tAvailable Space:\t%u\n",test.Available());
}
