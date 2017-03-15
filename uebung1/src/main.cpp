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
    char* result=Printf(a+0,a+999,"%x, %d, %d, %s, %c, %b, %u, %% \n",test,(int)-12345,(int)12345," Hallo \0",'a',test,test);
    //char* result=Printf(a,a+999,"%b\n",test);
    printf("%s",result);
}
