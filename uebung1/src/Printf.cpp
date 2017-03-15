/*
 *  @author Marcel Fest
 *
 *  include stdarg.h from c++ for variable parameters
 */

#include <cstdarg>
#include <cstdio>
#include "Printf.h"
/*
 *  Defined Functions
 */
char* int_to_string(char*,int,int);
char* unsigned_int_to_number_system_string(char*, unsigned int, int);

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
        if(iter>end)
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
                        value=value*-1;
                    }
                    iter=int_to_string(iter,value,10);
                    break;
                case 'u':
                    val=va_arg(vl,unsigned int);
                    iter=unsigned_int_to_number_system_string(iter,val,10);
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
                    iter=unsigned_int_to_number_system_string(iter,val,16);
                    break;

                case 'b':
                    val=va_arg(vl,unsigned int);
                    *iter++='0';
                    *iter++='b';
                    iter=unsigned_int_to_number_system_string(iter,val,2);
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
    *iter='\0';
    return dst;
}
char* int_to_string(char* buffer,int val, int type)
{
    char dig_its[]="0123456789";
    char digit=dig_its[val%type];
    printf("Type: %d, Value: %d, Value%%Type %d\n",type,val,val%type);
    val=val/type;
    if(val) buffer=int_to_string(buffer,val,type);
    *buffer++=digit;
    return buffer;
}
char* unsigned_int_to_number_system_string(char* buffer,unsigned int val, int type)
{
    char dig_its[]="0123456789abcdef";
    printf("Type: %d, Value: %u, Value%%Type %u\n",type,val,val%type);
    char digit=dig_its[val%type];
    val=val/type;
    if(val) buffer=unsigned_int_to_number_system_string(buffer,val, type);
    *buffer++=digit;
    return buffer;
}
