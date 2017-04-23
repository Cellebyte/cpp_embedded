/*
 *  @author Marcel Fest
 */
#include "PreAllocString.h"
#include <cstdio>
int main()
{
    CREATE(test_string_5,5);
    CREATE(test_string_20,20);
    CREATE(test_string_100,100);
    int counter=1;

    printf("-->Test %d. Overflow:\n",counter++);
    /* Test overflow */
    test_string_5 = "abcdefghijk";
    printf("Inhalt: %s\t Länge: %d\t Maximum: %d\n",\
            (const char*)test_string_5,\
            test_string_5.GetLength(),\
            test_string_5.SizeOf());
    printf("-->Test %d. char operators:\n",counter++);
    /* Test operator = for char */
    test_string_20 = 'a';
    test_string_20.AddWhiteSpace();
    /* Test operator += for char */
    test_string_20 += 'b';
    printf("Inhalt: %s\t Länge: %d\t Maximum: %d\n",\
            (const char*)test_string_20,\
            test_string_20.GetLength(),\
            test_string_20.SizeOf());
    printf("-->Test %d. String operators:\n",counter++);
    /* Test operator = for char array*/
    test_string_100 = "abcdef";
    test_string_100.AddWhiteSpace();
    /* Test operator += for char array*/
    test_string_100 += "ghijklo";
    printf("Inhalt: %s\t Länge: %d\t Maximum: %d\n",\
            (const char*)test_string_100,\
            test_string_100.GetLength(),\
            test_string_100.SizeOf());
    printf("-->Test %d. array[index] operator:\nPosition: %d\tValue: %d\n",
            counter++,
            1,
            test_string_100[1]);
    test_string_100[1];
    printf("Inhalt: %s\t Länge: %d\t Maximum: %d\n",\
            (const char*)test_string_100,\
            test_string_100.GetLength(),\
            test_string_100.SizeOf());
    printf("-->Test %d. indirekt test of AddFormat and Empty:\n",counter++);
    test_string_100="abcdefghijklmnopqrstuvwxyz";
    printf("Inhalt: %s\t Länge: %d\t Maximum: %d\n",\
            (const char*)test_string_100,\
            test_string_100.GetLength(),\
            test_string_100.SizeOf());

    CREATE(test_string20_2,20);
    /*printf("-->Test %d. Copy Class:\n",counter++);
     *test_string20_2="zyxw";
     * fails-->> test_string_20=test_string20_2;
     */

    printf("--> Test ENDE \n");
}
