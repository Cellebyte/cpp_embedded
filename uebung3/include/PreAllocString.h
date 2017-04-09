#ifndef PREALLOCSTRING_H
#define PREALLOCSTRING_H
#define CREATE(varName,size) PreAllocString<size> varName;

#include <cstdarg>
#include <cstddef>
#include "Printf.h"

template <size_t MAX_LENGTH>
class PreAllocString
{
    public:
        PreAllocString()
        {
            Empty();
        }

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

        /*  Set to '\0' on all cells and point to start*/
        void Empty()
        {
            for(size_t n = 0; n <= max_length; n++)
            {
                string_storage[n] = '\0';
            }
            used_length = 0;
        }
        void AddFormat(const char* format, ...)
        {
            va_list vl;
            va_start(vl,format);
            char* temp;
            temp = Printf(string_storage+used_length,string_storage+max_length,format,vl);
            used_length = temp - string_storage;
            va_end(vl);
        }

        void AddWhiteSpace()
        {
            string_storage[used_length++]=' ';
        }
        operator const char *() const
        {
            return GetFrontPointer();
        }
        operator const void *() const
        {
            return static_cast<void> (GetFrontPointer());
        }
        const char& operator[](const int idx);

        PreAllocString& operator=( char rhs )
        {
            Empty();
            AddFormat("%c", rhs);
            return *this;
        }
        PreAllocString& operator=( const char* rhs)
        {
            Empty();
            AddFormat("%s", rhs);
            return *this;
        }
        PreAllocString& operator=( char* const rhs)
        {
            Empty();
            AddFormat("%s", rhs);
        }
        PreAllocString& operator+=( char rhs)
        {
            AddFormat("%c", rhs);
        }
        PreAllocString& operator+=( char const* rhs)
        {
            AddFormat("%s", rhs);
        }
    private:
        size_t used_length;
        const size_t max_length = MAX_LENGTH;
        char string_storage[MAX_LENGTH];
        const char* GetFrontPointer() const
        {
            return &string_storage[0];
        }

};

#endif
