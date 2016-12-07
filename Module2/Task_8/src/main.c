#include <stdio.h>
#include "source.h"

int main()
{
    /* Implement your own tests here. Below, you can find an example test
     * for es_demo */

    char str[] = "yksi,kaksi,kymmenen!neljä";

    int n = es_demo(str, ',');
    if (n == 3)
        printf("Success!");
    else
        printf("Fail...");
    return 0;
}
