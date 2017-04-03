/*
 *  @author Marcel Fest
 */

#include "Printf.h"
#include <cstdio>

/* int main()
{
    char a[1000];
    unsigned int test=16;
    char* result=Printf(a+0,a+999,"%x, %d, %d, %s, %c, %b, %u, %% ",test,-12345,12345,"Hallo",'a',test,test);
    printf("%s\n",result);
}*/
int main() {
    char buffer[256] = {0};
    char small_buffer[32];
    int ret = Printf(
            buffer,
            buffer+256,
            "verbatim text %d, %u, %c, %s, %x, %b, %%",
            -4316, 2958, '&', "some string", 0xdeadbeef, 0xdeadbeef
            )? 0 : 1;
    printf("%s",buffer);
    ret |= Printf(
            small_buffer, small_buffer+32,
            "%%d%%%d",
            -123
            )? 0 : 1;
    ret |= Printf(
            small_buffer, small_buffer+32,
            "%s",
            "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
            )? 1 : 0;
    return ret;
}

