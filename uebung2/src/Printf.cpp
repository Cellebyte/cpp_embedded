/*
 *  @author Marcel Fest
 *
 *  include stdarg.h from c++ for variable parameters
 */

#include <cstdarg>
//#include <cstdio>
#include "Printf.h"
/*
 *  Defined Functions
 *  @function unsigned_int_to_number_system_string
 *  @param char* buffer
 *  @param unsigned int value to be transferred
 *  @param int give typ of number_system (hex=16, oct=8, dec=10, usw. ..)
 *  @return char* pointer on the last free element in buffer.
 */
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
     */
    va_list vl;                     // variable arg list initialization
    va_start(vl, fmt);              // define variable params after fmt
    char temp=0;                    // define buffer character
    char* iter=dst;                 // define iterator over array;
    char* erg=0;                    // result of switch case
    unsigned int val=0;             // value for 2. 5. 6.
    int value=0;                    // value for 1.
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
        if (temp!='%')      //  if not a format string copy char to destination array
        {
            *iter++=temp;
        }
        else
        {
            fmt++;
            temp=*fmt;
            // DEBUG printf("%c",*fmt);
            switch(temp)    // goto right format and add its formatted string to destination
            {
                //  signed Integer
                case 'd':
                    value=va_arg(vl,int);   //  get element from stack
                    if (value<0)  //  check if integer is negative
                    {
                        *iter++='-';
                        value=value*-1;
                    }
                    iter=unsigned_int_to_number_system_string(iter,(unsigned int)value,10); // use modulo to map right string
                    break;
                //  unsigned Integer
                case 'u':
                    val=va_arg(vl,unsigned int);
                    iter=unsigned_int_to_number_system_string(iter,val,10); // use modulo to map right string
                    break;
                //  character
                case 'c':
                    *iter++=va_arg(vl,int);
                    break;
                //  String --> char Array
                case 's':
                    erg=va_arg(vl,char*);
                    while(*erg!='\0')       //append given string without \0 to destination array
                    {
                        *iter++=*erg++;
                    }
                    break;
                //  unsigned Integer to hex presentation
                case 'x':
                    val=va_arg(vl,unsigned int);
                    *iter++='0';
                    *iter++='x';
                    iter=unsigned_int_to_number_system_string(iter,val,16);
                    break;
                //  unsigned Integer to byte presentation
                case 'b':
                    val=va_arg(vl,unsigned int);
                    *iter++='0';
                    *iter++='b';
                    iter=unsigned_int_to_number_system_string(iter,val,2);
                    break;
                //  append a simple % to the String
                case '%':
                    *iter++=temp;
                    break;
                //  if not defined return an empty destination
                default:
                    return '\0';
            }
        }
        fmt++; //   go to next format character
    }
    va_end(vl);     //  close the variable parameters
    *iter++='\n';   //  append \n --> #TODO \r\n for Windows and \r for OSX
    *iter='\0';     //  append end of String
    return dst;     //  return the created String
}
char* unsigned_int_to_number_system_string(char* buffer,unsigned int val, int type)
{
    char dig_its[]="0123456789abcdef";
    // DEBUG printf("Type: %d, Value: %u, Value%%Type %u\n",type,val,val%type);
    char digit=dig_its[val%type];       // map in right char in array with modulo function
    val=val/type;                       // decrease value
    if(val) buffer=unsigned_int_to_number_system_string(buffer,val, type); // recursion if value not zero
    *buffer++=digit;    // add digit to string
    return buffer;      // return pointer on free element in buffer
}
