
/*
 *  @author Marcel Fest
 */

#include "OptParser.h"

/*  @function Parse
 *  @param argument counter [argc] (length of argument array)
 *  @param argument vector [argv] (argument array except the program name)
 *  @return boolean
 */

bool CmdLineOptParser::Parse(int argc, char* argv[])
{
    for (int i=0; i<argc; i++)
    {
        //parse if an option exist and store this option in an temporary variable
        char* argument=argv[i];
        if(argument[0]!='-') return false;
        if(argument[1]=='\0') return false;
        char option=argument[1];
        /*  Parse different types of arguments
         *  x stands for an option
         *  1. -x
         *  2. -xstring
         *  3. -x=string
         *  4. -x string
         */
        // check if variant 2. or 3. are given
        if(argument[2]!='\0')
        {
            int j;
            int begin;
            // check for variant 3 with '='
            if(argument[2]=='=')
            {
                j=3;
                begin=3;
            }
            else
            {
		j=2;
                begin=2;
            }
            //move given string to the right Memory Addresses
            while(argument[j]!='\0')
            {
                argument[j-begin]=argument[j];
                j++;
            }
            argument[j-begin]='\0';
            /*  end the String with \0
             *  Check if The given Option and Argument is right
             */
            if(! Option(option,argument)) return false;
        }
        else
        //check for variant 1. or 4.
        {
            //lookahead
            if(argv[i+1])
            {
                if(argv[i+1][0]!='-')
                {
                    if(! Option(option,argv[++i])) return false;
                }
            }
            else
            {
                if(! Option(option,'\0')) return false;
            }
        }
    }
    return true;
}
bool CmdLineOptParser::Option(const char , const char* )
{
    return true;
}
