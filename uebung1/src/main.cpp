/*
 *  @author Marcel Fest
 */

#include "OptParser.h"
#include "Printf.h"
#include <cstdio>
//int main(int argc, char* argv[])
int main()
{
    //CmdLineOptParser cmd;
    //cmd.Parse(--argc,++argv);
    char a[1000];
    unsigned int test=16;
    //printf("%s",Printf(a,(void*)&a[1000],"%x, %d, %d, %s, %c, %b, %u, %%",16,-12345,12345," Hallo \0", 'a', 16, 1233))
    char* result=Printf(a,a+999,"%b\n",test);
    printf("%s",result);
}
