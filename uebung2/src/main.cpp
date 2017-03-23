/*
 *  @author Marcel Fest
 */

#include "Printf.h"
#include <cstdio>

int main()
{
    char a[1000];
    unsigned int test=16;
    char* result=Printf(a+0,a+999,"%x, %d, %d, %s, %c, %b, %u, %% ",test,(int)-12345,(int)12345,"Hallo",'a',test,test);
    printf("%s\n",result);
}
