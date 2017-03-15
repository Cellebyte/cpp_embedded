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
    while(fmt!='\0')
    {
        if((void*)iter>=end)
        {
            /*  #TODO fix Bufferoverflow.
             * No correct solution at the moment
             */

            return '\0';
        }
        temp=*(fmt);

        if (temp!='%')      //  if not a format copy char to buffer
        {
             *iter=temp;
        }
        else
        {
            fmt++;
            temp=*(fmt);
            switch(temp)    //  else switch on format string
            {
                case 'd':
                    {
                        int val=va_arg(vl,int);
                        if (val<0)  //  check if integer is negative
                        {
                            *iter++='-';
                        }
                        while(val)  //  use modulo for convert to string
                        {
                            *iter++=val%10+'0';
                            val=val/10;
                        }
                        break;
                    }
                case 'u':
                    {
                        unsigned int val=va_arg(vl,unsigned int);
                        while(val)
                        {
                            *iter++=val%10+'0';
                            val=val/10;
                        }
                        break;
                    }
                case 'c':
                    {
                        *iter++=va_arg(vl,int);
                        break;
                    }
                case 's':
                    {
                        erg=va_arg(vl,char*);
                        while(erg!='\0')
                        {
                            *iter++=*erg++;
                        }
                        break;
                    }
                case 'x':
                    {
                        unsigned int val=va_arg(vl,unsigned int);
                        *iter++='0';
                        *iter++='x';
                        while(val)
                        {
                            *iter++=val%16+'0';
                            val=val/16;
                        }
                        break;
                    }

                case 'b':
                    {
                        unsigned int val=va_arg(vl,unsigned int);
                        *iter++='0';
                        *iter++='b';
                        while(val)
                        {
                            *iter++=val%2+'0';
                            val=val/2;
                        }
                        break;
                    }
                case '%':
                    {
                        *iter++=temp;
                        break;
                    }
                default:break;
            }
        }
        fmt++;
    }
    iter='\0';
    va_end(vl);
    return dst;
}

