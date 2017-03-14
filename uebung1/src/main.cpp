/*
 *  @author Marcel Fest
 */

#include "OptParser.h"
#include "Printf.h"
int main(int argc, char* argv[])
{
    CmdLineOptParser cmd;
    if(cmd.Parse(--argc,++argv)) return 0;
    else return 1;
}
