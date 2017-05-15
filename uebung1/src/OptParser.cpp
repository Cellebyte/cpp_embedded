
/*
 *  @author Marcel Fest
 */

#include "OptParser.h"

#define END_OF_STRING '\0'
#define FLAG '-'

#define NULLING(x) x=0

/*  @function Parse
 *  @param argument counter [argc] (length of argument array)
 *  @param argument vector [argv] (argument array except the program name)
 *  @return boolean (true|false) true if successfull
 */

bool CmdLineOptParser::Parse(int argc, char* argv[])
{
    char* argument = nullptr;
    char NULLING(option);
    int NULLING(i);
    if (FLAG != argv[0][0])
    {
        i=1;
    }

    for (; i<argc; i++)
    {
        //parse if an option exist and store this option in an temporary variable
        argument = argv[i];
        if(nullptr == argument) return false;
        if(FLAG != argument[0]) return false;
        if(END_OF_STRING == argument[1]) return false;
        option = argument[1];

        /*  Parse different types of arguments
         *  x stands for an option
         *  1. -x
         *  2. -xstring
         *  3. -x=string
         *  4. -x string
         */

        // check if variant 2. or 3. are given
        if(END_OF_STRING != argument[2])
        {
            if('=' == argument[2])
            {
                    argument = &(argument[3]);
            }
            // now it is variant without the '='
            else
            {
                argument = &(argument[2]);
            }
        }
        else
        // check if variant 1. or 4.
        {
            //  lookahead
            if(argv[i+1])
            {
                // Variant 4 option and string are divided by a space
                if(FLAG != argv[i+1][0])
                {
                    argument = argv[++i];
                }
                else
                {
                    argument = END_OF_STRING;
                }
            }
            // no lookahead --> their is no string for the argument so only check the option
            else
            {
                argument = END_OF_STRING;
            }
        }
        if(! Option(option,argument)) return false;
    }
    // when the while loop parse everything
    return true;
}

// @Override
bool CmdLineOptParser::Option(const char, const char*)
{
    return true;
}
