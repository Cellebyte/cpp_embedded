/*
 *  @author Marcel Fest
 *
 *  include stdarg.h from c++ for variable parameters
 */

#include <cstdarg>
#include <cstring>
#include "Printf.h"
/*
 *  @function Printf
 *  @param char* destination
 *  @param end of given destination array
 *  @param format String
 *  @param variable arg_list
 *  @return substituted Format String
 */

char* Printf( char* dst, const void* end, const char* fmt, ... )
{
    /*
     *  1.  %d für signed int
     *  2.  %u für unsigned int
     *  3.  %c für ein einzelnes Zeichen
     *  4.  %s für eine Zeichenkette
     *  5.  %x für die Ausgabe in Hexadezimalform (0xaf)
     *  6.  %b für die Ausgabe in Binärform (0b10101)
     *  7.  %% für die Ausgabe des Prozentzeichens
     *
     */
    va_list vl;                     // variable arg list initialization
    va_start(vl, fmt);              // define variable params after fmt
    char temp=0;                    // define buffer character
    char* iter=dst;                 // define iterator over array;
    char* erg=0;                    // result of switch case
    bool is_array=false;             // bool for ext copy function
    while(fmt!='\0')
    {
        temp=*(fmt);

        if (temp!='%')
        {
             iter=&temp;
        }
        else
        {
            fmt++;
            temp=*(fmt);
            switch(temp)
            {
                case 'd':
                    {
                        int val=va_arg(vl,int);
                        is_array=true;
                        break;
                    }
                case 'u':
                    {
                        unsigned int val=va_arg(vl,unsigned int);
                        is_array=true;
                        break;
                    }
                case 'c':
                    {
                        char val=va_arg(vl,char);
                        is_array=false;
                        iter=&val;
                        break;
                    }
                case 's':
                    {
                        char* val=va_arg(vl,char*);
                        is_array=true;
                        break;
                    }
                case 'x':
                    {
                        is_array=true;
                        break;
                    }

                case 'b':break;
                case '%':
                    {
                        iter=&temp;
                        break;
                    }
                default:break;
            }
        }
        if(is_array)
        {
            while(erg!='\0')
            {
                iter=erg;
                iter++;
                erg++;
            }
        }
        iter++;
        fmt++;
    }
    va_end(vl);
    return dst;
}
