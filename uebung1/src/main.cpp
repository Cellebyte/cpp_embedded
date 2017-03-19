/*
 *  @author Marcel Fest
 */

#include "OptParser.h" // Header Files of Option Parser
#include <cstdio>


int main(int argc, char* argv[])
{
    CmdLineOptParser cmd;
    if (!cmd.Parse(--argc,++argv)) return 1;
}
