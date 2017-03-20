
/*
 *  @author Marcel Fest
 */

#include "OptParser.h"
#include <ctype.h>

/*  @function Parse
 *  @param argument counter [argc] (length of argument array)
 *  @param argument vector [argv] (argument array except the program name)
 *  @return boolean (true|false) true if successfull
 */

bool CmdLineOptParser::Parse(int argc, char* argv[])
{
    for (int i=1; i<argc; i++)
    {
        //parse if an option exist and store this option in an temporary variable
        char* argument=argv[i];
        if(argument[0]!='-') return false;
        if(argument[1]=='\0'|| !isalpha(argument[1])) return false;
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
            // counter variable of the given while loop in line 49
            int hop=0;
            int begin=0;
            // check for variant 3 with '='
            if(argument[2]=='=')
            {
                hop=3;
                begin=3;
            }
            // now it is variant without the '='
            else
            {
		        hop=2;
                begin=2;
            }
            //move given string to the right Memory Addresses
            while(argument[hop]!='\0')
            {
                // copy string from offset of 3 or 2 to the front
                argument[hop-begin]=argument[hop];
                hop++;
            }
            argument[hop-begin]='\0';
            /*  end the String with \0
             *  Check if The given Option and Argument is right
             */
            if(! Option(option,argument)) return false;
        }
        else
        //check for variant 1. or 4.
        {
            /*  lookahead for the next String in the given argument vector
             *  i is the Counter Variable from above
             */
            if(argv[i+1])
            {
                // Variant 4 option and string are divided by a space
                if(argv[i+1][0]!='-') //    if not an option Check for option argument
                {
                    argument=argv[++i]; // increase i for the next option
                    if(! Option(option,argument)) return false;
                }
                else
                {
                    if(! Option(option,'\0')) return false;
                }
            }
            // no lookahead --> their is no string for the argument so only check the option
            else
            {
                if(! Option(option,'\0')) return false;
            }
        }
    }
    // when the while loop parse everything
    return true;
}

// @Override
bool CmdLineOptParser::Option(const char option, const char* argument )
{
    return true;
}
