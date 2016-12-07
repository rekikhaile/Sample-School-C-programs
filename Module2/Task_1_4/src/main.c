#include <stdio.h>
#include "source.h"

int main()
{
	/* This is an example for testing your functions.
	 * feel free to modify this code. TMC will not test this. */
    /* Lets first test 2.1 Pointer Sum with two input parameters */
    int val1 = 4; int val2 = 5;
    pointer_sum(&val1, &val2);
    if (val1 == 9) {
        printf("Great, 2.1 worked!\n");
    } else {
        printf("Invalid result for 2.1\n");
    }

    /* Testing 2.2 Extending Pointer sum */
    val1 = 2;
    int valarray[] = { 10, 100, 1000 };
    pointer_sum2(&val1, valarray, 3);
    if (val1 == 1112) {
        printf("2.2 worked\n");
    } else {
        printf("Invalid result for 2.2");
    }

    /* Test 2.2 Array Reader */
    int ints[10];
    printf("Read %d integers!\n", array_reader(ints, 10));

    /* Test 2.3 Magic Number */
    /* Try this: modify the below code such that it uses the above array
       from array reader */
    int arr[] = {3, 4, 7, 2, 8};
    int res = magic_number(arr, 2);
    printf("magic: %d\n", res);

    return 0;
}
