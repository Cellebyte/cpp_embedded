/*
 *  @author Marcel Fest
 */

#include "PoolAllocator.h"
#include <cstdio>
int main()
{
    CREATE(test,15,20);
    printf("%d",static_cast<int>(test.Available()));
}
