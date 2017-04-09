//This is the main

#include "PreAllocString.h"
#include <cstdio>
int main()
{
    CREATE(test_string,100);
    test_string = "asdf";
    printf("%s\n",(const char*)test_string);
}
