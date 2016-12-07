#include "source.h"

int main()
{
    /* testing exercise 1.5 */
    printf("\n--- Exercise 1.5 ---\n");
    multi_table(); 
    
    /* testing exercise 1.6 */
    printf("\n--- Exercise 1.6 ---\n");

    /* fopen will be introduced later. Just trust (or hope) that the line works */
    FILE *f;
    f = fopen("../test/test-file.txt", "r");
    if (!f) {
        f = fopen("test/test-file.txt", "r");
    }

    if (f) {
        hexdump(f);
        fclose(f);
    } else {
        printf("Opening test file failed\n");
    }
    
    return 0;
}
