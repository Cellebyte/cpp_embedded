/*
 *  @author Marcel Fest
 *
 *  include stdarg.h from c++ for variable parameters
 */
#include "Printf.h"

#define END_OF_STRING       '\0'
#define NULLING(x)           x=0

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
 *  @param end pointer --> Fix Bufferoverflow
 *  @return char* pointer on the last free element in buffer.
 */

char* unsigned_int_to_number_system_string(char*, unsigned int, int, const void*);
static const char digits[] = "0123456789abcdef";

/*
 *  @function Printf
 *  @param char* destination
 *  @param end of given destination array
 *  @param format String
 *  @param variable arg_list
 *  @return substituted Format String
 */

char* Printf( char* dst, const void* end, const char* fmt, va_list vl )
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
    int NULLING(type);          //   type of NumberString
    char NULLING(temp);         //   define buffer character
    char* NULLING(erg);         //   result of switch case
    unsigned int NULLING(val);  //   value for 2. 5. 6.
    int NULLING(value);         //   value for 1.

    char* iter=dst;             // define an iterator over the array

    while(END_OF_STRING != *fmt && iter < end)
    {
        temp = *fmt;
        if (PROCENT != temp)    //  if not a format string copy char to destination array
        {
            *iter++ = temp;
        }
        else
        {
            NULLING(type);
            fmt++;              //  --> Next Character
            temp = *fmt;
            switch(temp)        // goto right format and add its formatted string to destination
            {
                case INTEGER:
                    value = va_arg(vl,int);   //  get element from stack
                    if (0 > value)  //  check if integer is negative
                    {
                        *iter++ = '-';
                        value = value*-1;
                    }
                    val = static_cast<unsigned int>(value);
                    type = 10;
                    break;
                case UNSIGNED_INTEGER:
                    val = va_arg(vl,unsigned int);
                    type = 10;
                    break;
                case CHARACTER:
                    *iter++ = va_arg(vl,int);
                    break;
                case STRING:        //  --> Char Array
                    erg = va_arg(vl,char*);
                    while(END_OF_STRING != *erg && iter < end)       //append given string without \0 to destination array
                    {
                        *iter++ = *erg++;
                    }
                    break;
                case HEXADECIMAL:
                    val = va_arg(vl,unsigned int);
                    *iter++ = '0';
                    *iter++ = HEXADECIMAL;
                    type = 16;
                    break;

                case BINARY:
                    val=va_arg(vl,unsigned int);
                    *iter++ = '0';
                    *iter++ = BINARY;
                    type = 2;
                    break;

                case PROCENT:
                    *iter++ = temp;
                    break;
                default:            // if not defined return empty String
                    return END_OF_STRING;
            }
            if (0 != type)
            {
                iter=unsigned_int_to_number_system_string(iter,val,type,end);
            }
        }
        fmt++;      //   --> next character
    }
    va_end(vl);     //  close the variable parameters
    *iter = END_OF_STRING;     //  append end of String
    return dst;     //  return the created String
}
char* unsigned_int_to_number_system_string(char* buffer,unsigned int value, int type, const void* end)
{
    char digit = digits[value%type];       // map right char in array with modulo function
    value = value/type;                     // decrease value divided by type
    if (value) buffer = unsigned_int_to_number_system_string(buffer,value, type, end); // recursion if value not zero
    if (buffer < end)
    {
        *buffer++ = digit;    // add digit to string
        return buffer;        // return pointer on last empty array field
    }
    else
    {
        return END_OF_STRING;
    }
}
