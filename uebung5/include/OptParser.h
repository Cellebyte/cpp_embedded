/*
 *  @author Marcel Fest
 */

#ifndef CMD_PARSER_H
#define CMD_PARSER_H
#include <unistd.h> /* for write*/
#include <cstring>
#include "Communication.h"
#include "Printf.h"
#include "stdout.h"

class CmdLineOptParser
{
    public:
        // returns true , if all arguments are successfully parsed .
        bool Parse(int argc , char* argv []);

    protected:
        // returns true , if option was successfully parsed .
        virtual bool Option(const char , const char*);
};


#endif
