/**
 * @author Marcel Fest
 */


#include "OptParser.h"

int main(int argc, char *argv[]) {
    CmdLineOptParser optparse;
    if(!optparse.Parse(argc,argv)) return 1;
}
