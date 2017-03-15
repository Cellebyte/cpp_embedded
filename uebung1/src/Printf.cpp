/*
 *  @author Marcel Fest
 *
 *  include stdarg.h from c++ for variable parameters
 */

#include <cstdarg>
#include <cstdio>
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
     *  1.  %d for signed int
     *  2.  %u for unsigned int
     *  3.  %c for single char
     *  4.  %s for String
     *  5.  %x for hex representation(0xaf)
     *  6.  %b for binary representation (0b10101)
     *  7.  %% for %
     *
     */
    va_list vl;                     // variable arg list initialization
    va_start(vl, fmt);              // define variable params after fmt
    char temp=0;                    // define buffer character
    char* iter=dst;                 // define iterator over array;
    char* erg=0;                    // result of switch case
    unsigned int val=0;
    int value=0;
    while(*fmt!='\0')
    {
        // DEBUG printf("%c",*fmt);
        if(iter>=end)
        {
            /*  #TODO fix Bufferoverflow.
             * No correct solution at the moment
             */

            return '\0';
        }
        temp=*fmt;
        if (temp!='%')      //  if not a format copy char to buffer
        {
            *iter++=temp;
        }
        else
        {
            fmt++;
            temp=*fmt;
            // DEBUG printf("%c",*fmt);
            switch(temp)    //  else switch on format string
            {
                case 'd':
                    value=va_arg(vl,int);
                    if (value<0)  //  check if integer is negative
                    {
                        *iter++='-';
                    }
                    while(value)  //  use modulo for convert to string
                    {
                        *iter++=value%10+'0';
                        val=val/10;
                    }
                    break;
                case 'u':
                    val=va_arg(vl,unsigned int);
                    while(val)
                    {
                        *iter++=val%10+'0';
                        val=val/10;
                    }
                    break;
                case 'c':
                    *iter++=va_arg(vl,int);
                    break;
                case 's':
                    erg=va_arg(vl,char*);
                    while(*erg!='\0')
                    {
                        *iter++=*erg++;
                    }
                    break;
                case 'x':
                    val=va_arg(vl,unsigned int);
                    *iter++='0';
                    *iter++='x';
                    while(val)
                    {
                        *iter++=val%16+'0';
                        val=val/16;
                    }
                     break;

                case 'b':
                    val=va_arg(vl,unsigned int);
                    *iter++='0';
                    *iter++='b';
                    do{
                        *iter++=val%2+'0';
                        val=val/2;
                    }while(val);
                    break;
                case '%':
                    *iter++=temp;
                    break;
                default:
                    return '\0';
            }
        }
        fmt++;
    }
    va_end(vl);
    iter='\0';
    return dst;
}

