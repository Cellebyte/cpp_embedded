/*
 *  @author Marcel Fest
 */
#ifndef PREALLOCSTRING_H
#define PREALLOCSTRING_H
#define CREATE(varName,size) PreAllocString<size> varName;
#define DISALLOW_COPY(class_name) \
class_name(const class_name&); \
class_name& operator =(const class_name&);

#include <cstdarg>
#include <cstddef>
#include "Printf.h"

template <size_t MAX_LENGTH>
class PreAllocString
{
    private:
        size_t used_length;
        const size_t max_length = MAX_LENGTH;
        char string_storage[MAX_LENGTH];

    private:
        const char* GetFrontPointer() const
        {
            return &string_storage[0];
        }
        const char& GetOnPosition(const int idx) const
        {
            return string_storage[idx];
        }

    public:
        PreAllocString()
        {
            Empty();
        }

        DISALLOW_COPY(PreAllocString);

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
        /*  uses Printf to append or add different Formats to string_storage
         *  for reference which types are supported
         *  view into the implementation of Printf under Printf.cpp
         */
        void AddFormat(const char* format, ...)
        {
            va_list vl;
            va_start(vl,format);
            char* temp;
            temp = Printf(string_storage+used_length,string_storage+max_length,format,vl);
            used_length = temp - string_storage;
            va_end(vl);
        }
        /* Adds one Space into the next Writable Character and increments*/
        void AddWhiteSpace()
        {
            if (used_length > max_length) return;

            string_storage[used_length++]=' ';
        }

        /*  Overloaded operators:
         *  1. '='      ->> varName = "String"
         *  2. '='      ->> varName = 'char'
         *  3. '+='     ->> varName += "String" ⁼   varName.append("String")
         *  4. '+='     ->> varName += 'char'   ⁼   varName.append('char')
         *  5. '[]'     ->> varName[index] (only readable) ->> returns char
         *  6. 'cast'   ->> cast to 'const void*' and cast to 'const char*'
         */
        PreAllocString& operator=( const char* rhs)
        {
            Empty();
            AddFormat("%s", rhs);
            return *this;
        }
        PreAllocString& operator=( char rhs )
        {
            Empty();
            AddFormat("%c", rhs);
            return *this;
        }
        PreAllocString& operator=( char* const rhs)
        {
            Empty();
            AddFormat("%s", rhs);
            return *this;
        }

        PreAllocString& operator+=( char const* rhs )
        {
            AddFormat("%s", rhs);
            return *this;
        }
        PreAllocString& operator+=( char rhs)
        {
            AddFormat("%c", rhs);
            return *this;
        }
        /*  Defined casts for this class*/
        operator const char *() const
        {
            return GetFrontPointer();
        }
        operator const void *() const
        {
            return static_cast<void> (GetFrontPointer());
        }
        /*  Array [index] implementation */
        const char& operator[](const int idx)
        {
            return GetOnPosition(idx);
        }
};

#endif
