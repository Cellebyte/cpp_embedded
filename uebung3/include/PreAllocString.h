#ifndef PREALLOCSTRING_H
#define PREALLOCSTRING_H
#define CREATE(varName,size) PreAllocString<size> varName;

#include <cstdarg>
#include "Printf.h"


template <size_t MAX_LENGTH>
class PreAllocString
{
    public:
        /*  Current number of characters in string */
        size_t GetLength() const
        {
            return used_length;
        }

        /*  Maximum number of characters possible */
        constexpr size_t SizeOf()
        {
            return max_length;
        }
        void Empty();
        {
            used_length=0;
        }
        void AddFormat(const char* format, ...)
        {

        }
        void AddWhiteSpace()
        {

        }

    private:
        size_t used_length;
        const size_t max_length = MAX_LENGTH;
        char string_storage[MAX_LENGTH];

}

#endif
