#include <check.h>
#include "tmc-check.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../src/source.h"


START_TEST(test_pointer_sum)
{
#if defined(__CYGWIN__)
    fail("Local tests not working on Cygwin. Send code to server for checking.\n");
#endif     
	int a, b;
	
	a = rand() % 100;
	b = rand() % 100;
	int a_temp = a;
	int b_temp = b;
	pointer_sum(&a, &b);
        fail_unless(a == a_temp + b_temp, "[Task 2.1] pointer_sum(a, b) called with *a = %d and *b = %d. Value of *a should change to %d, but it is %d\n",
                a_temp, b_temp, a_temp + b_temp, a);
}
END_TEST


START_TEST(test_pointer_sum2)
{
#if defined(__CYGWIN__)
    fail("Local tests not working on Cygwin. Send code to server for checking.\n");
#endif     
    int a, i, size, mysum;
    int orig_a;
    int array[6];
    for (i = 0; i < 3; i++) {
        orig_a = a = rand() % 100;
        mysum = a;
        size = rand() % 6;
        char buf[80];
        buf[0] = 0;
        for (int j = 0; j < size; j++) {
            char buf2[8];
            array[j] = rand() % 100;
            mysum += array[j];
            sprintf(buf2, "%d ", array[j]);
            strcat(buf, buf2);
        }
        pointer_sum2(&a, array, size);
        fail_unless(a == mysum, "[Task 2.2] Invalid sum for value %d and array: %s, expected: %d, got: %d\n",
                orig_a, buf, mysum, a);
    }
}
END_TEST


START_TEST(test_array_reader)
{
#if defined(__CYGWIN__)
    fail("Local tests not working on Cygwin. Send code to server for checking.\n");
#endif     
    char inp[200];
    int size = rand() % 10 + 5;
    inp[0] = 0;
    int s_arr[20];
    int ar1[20];
    int i, ret;
    for (i = 0; i < size; i++) {
        s_arr[i] = rand() % 100;
        char buf[10];
        sprintf(buf, "%d ", s_arr[i]);
        strcat(inp, buf);
    }
    strcat(inp, "f");
    
    FILE *mock_input = freopen("mockinput", "w+", stdin);
    fputs(inp, mock_input);
    fseek(mock_input, 0, SEEK_SET);

    memset(ar1, 0, sizeof(ar1));
    ret = array_reader(ar1, 20);
    
    fail_unless(ret == size, "[Task 2.3] Wrong return value with input \"%s\". You returned %d, expected %d\n",
            inp, ret, size);
    
    for (i = 0; i < size; i++) {
        fail_unless(ar1[i] == s_arr[i], "[Task 2.3] Wrong array member with input \"%s\". You had %d as %dth item, expected %d\n",
                inp, ar1[i], i+1, s_arr[i]);
    }
    
    fail_unless(ar1[size] == 0, "[Task 2.3] You have written more than %d values to array, which was the length of user input\n",
            size);
}
END_TEST

START_TEST(test_magic_number)
{
#if defined(__CYGWIN__)
    fail("Local tests not working on Cygwin. Send code to server for checking.\n");
#endif     
	int ar1[] = {3, 2435, 1234, 123, 34, 776, 1, -123, 235, -7, 2};
	int ar2[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0};
	int ret1, ret2;

	ret1 = magic_number(ar1, -123);
	ret2 = magic_number(ar2, 0);

	fail_unless(ret1 == 7, "[Task 2.4] Magic number should be at index 7, not at %d\n", ret1);
	fail_unless(ret2 == 44, "[Task 2.4] Magic number should be at index 44, not at %d\n", ret2);
}
END_TEST


int main(int argc, const char *argv[])
{
	srand((unsigned)time(NULL));
	Suite *s = suite_create("Test-2.1-3");

        /* TODO: define tests */
	tmc_register_test(s, test_pointer_sum, "2.1");
        tmc_register_test(s, test_pointer_sum2, "2.2");
	tmc_register_test(s, test_array_reader, "2.3");
	tmc_register_test(s, test_magic_number, "2.4");
	return tmc_run_tests(argc, argv, s);
}
