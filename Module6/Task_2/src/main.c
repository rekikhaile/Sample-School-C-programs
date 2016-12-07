#include <stdio.h>
#include "stringman.h"


int main(void)
{
    char test[][10] = { "testi", "sokos", "toimii", "12345678" };
    for (int i = 0; i < 4; i++) {
        reverse_string(test[i]);
        printf("%d: %s -- palindrome: %d\n", i, test[i], is_palindrome(test[i]));
    }
    
    char testString[] = "A palindrome is a word,\n"
        "phrase, number, or other sequence of\n"
        "symbols or elements, whose meaning\n"
        "may be interpreted the same way in\n"
        "either forward or reverse direction\n";
    printf("1: %s", testString);
    reversed_words(testString);  
    printf("2: %s\n", testString);
    
    const char *testcount = "one two one three two four two";
    char result[20];
    result[0] = 0;
    int c = most_common(testcount, result, 20);
    printf("most common: %s (%d)\n", result, c);
}
