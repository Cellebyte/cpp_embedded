/*
 *  @author Marcel Fest
 */

//returns the next writable character
#ifndef PRINTF_H
#define PRINTF_H

#include <cstdarg>

char* Printf(char* dst, const void* end, const char* fmt, va_list vl);

#endif
