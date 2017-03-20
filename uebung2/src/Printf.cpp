/*
 *  @author Marcel Fest
 *
 *  include stdarg.h from c++ for variable parameters
 */

#include <cstdarg>
//#include <cstdio>
#include "Printf.h"

#define END_OF_STRING       '\0'
#define NEWLINE             '\n'
#define NULL(x)             x=0

#define INTEGER             'd'
#define CHARACTER           'c'
#define UNSIGNED_INTEGER 	'u'
#define HEXADECIMAL         'x'
#define BINARY              'b'
#define STRING              's'
#define PROCENT             '%'

/*
 *  Defined Functions
 *  @function unsigned_int_to_number_system_string
 *  @param char* buffer
 *  @param unsigned int value to be transferred
 *  @param int give typ of number_system (hex=16, oct=8, dec=10, usw. ..)
 *  @return char* pointer on the last free element in buffer.
 */
char* unsigned_int_to_number_system_string(char*, unsigned int, int);
static const char dig_its[] = "0123456789abcdef";

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
     */
    va_list vl;                         // variable arg list initialization
    va_start(vl, fmt);                  // define variable params after fmt
    char NULL(temp);                    // define buffer character
    char* iter=dst;                     // define iterator over array
    char* NULL(erg);                    // result of switch case
    unsigned int NULL(val);             // value for 2. 5. 6.
    int NULL(value);                    // value for 1.

    while(END_OF_STRING != *fmt && iter < end) // - two for \n and \0 at the end of the loop
    {
        // #TODO Fix Bufferoverflow!!
        temp = *fmt;
        if (PROCENT != temp)      //  if not a format string copy char to destination array
        {
            *iter++ = temp;
        }
        else
        {
            fmt++; //--> Next Character
            temp = *fmt;
            // #DEBUG printf("%c",*fmt);
            switch(temp)    // goto right format and add its formatted string to destination
            {
                case INTEGER:
                    value = va_arg(vl,int);   //  get element from stack
                    if (0 > value)  //  check if integer is negative
                    {
                        *iter++ = '-';
                        value = value*-1;
                    }
                    iter = unsigned_int_to_number_system_string(iter,(unsigned int)value,10); // use modulo to map right string
                    break;

                case UNSIGNED_INTEGER:
                    val = va_arg(vl,unsigned int);
                    iter = unsigned_int_to_number_system_string(iter,val,10); // use modulo to map right string
                    break;

                case CHARACTER:
                    *iter++ = va_arg(vl,int);
                    break;

                case STRING://--> Char Array
                    erg = va_arg(vl,char*);
                    while(END_OF_STRING != *erg)       //append given string without \0 to destination array
                    {
                        *iter++ = *erg++;
                    }
                    break;

                case HEXADECIMAL:
                    val = va_arg(vl,unsigned int);
                    *iter++ = '0';
                    *iter++ = HEXADECIMAL;
                    iter = unsigned_int_to_number_system_string(iter,val,16);
                    break;

                case BINARY:
                    val=va_arg(vl,unsigned int);
                    *iter++ = '0';
                    *iter++ = BINARY;
                    iter = unsigned_int_to_number_system_string(iter,val,2);
                    break;

                case PROCENT:
                    *iter++ = temp;
                    break;
                //  if not defined return an empty destination
                default:
                    return END_OF_STRING;
            }
        }
        fmt++; // --> next character
    }
    va_end(vl);     //  close the variable parameters
    *iter++ = NEWLINE;   //  append \n --> #TODO \r\n for Windows and \r for OSX
    *iter = END_OF_STRING;     //  append end of String
    return dst;     //  return the created String
}
char* unsigned_int_to_number_system_string(char* buffer,unsigned int value, int type)
{
    char digit = dig_its[value%type];       // map right char in array with modulo function
    value = value/type;                     // decrease value divided by type
    if(value) buffer = unsigned_int_to_number_system_string(buffer,value, type); // recursion if value not zero
    *buffer++ = digit;    // add digit to string
    return buffer;      // return pointer on free element after last element
}
