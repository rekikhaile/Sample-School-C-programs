#include <stdio.h>
#include "source.h"

int main()
{
    /* Testing 2.5 Selection Sort. Implement a function to print
     * the resulting array to see that it really works */ 
    int arr[] = {3, 4, 7, 2, 8};
    sort(arr, 5);

    /* Testing 2.6 Count Vowels */
    char *str = "How many vowels in this string?";
    printf("%d\n", count_vowels(str));
    
    /* Testing 2.7: Remove Comments */
    /* fopen will be introduced later. It opens a file, used in testing this task
     * Just trust (or hope) that the line works */
    FILE *f;
    //f = fopen("../test/hidden_test.txt", "r");
    f = fopen("../test/testfile.conf", "r");
    if (!f) {
        f = fopen("test/hidden_test.txt", "r");
    }

    if (f) {
        char outbuf[2000], inbuf[2000];
        int n = fread(inbuf, 1, sizeof(inbuf), f);
        if (n > 0) {
            inbuf[n] = 0;
            remove_comments(outbuf, inbuf);
            printf("Output:\n%s\n", outbuf);
        }
        
    } else {
        printf("opening file failed\n");
    }
    
    return 0;
}
