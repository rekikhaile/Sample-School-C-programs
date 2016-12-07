#include <check.h>
#include "tmc-check.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../src/source.h"


int mycompare(char *student, char *model, char *infostr)
{
    int i = 0, line = 1;
    while(*student) {
        if (!(*model)) {
            sprintf(infostr, "your output is longer than expected: character: '%c' (%d), position: %d, line: %d",
                    *student, *student, i, line);
            return -1;
        }
        if (*student != *model) {
            sprintf(infostr, "position: %d, line: %d, your output: '%c' (%d) , expected: '%c' (%d)", 
                    i, line, *student, *student, *model, *model);
            return -1;
        }
        if (*student == '\n') line++;
        student++; model++; i++;
    }
    if (*model) {
        sprintf(infostr, "output correct until position: %d, but shorter than expected",
                i);
        return -1;
    }
    return 0;
}


void pr_array(char *buf, int *arr, int n)
{
    char b[40];
    sprintf(buf, "{");
    while (n--) {
        sprintf(b, "%d", *arr++);
        if (n)
            strcat(b, ", ");
        strcat(buf, b);
    }
    strcat(buf, "}");
}

START_TEST(test_sort)
{
#if defined(__CYGWIN__)
    fail("Local tests not working on Cygwin. Send code to server for checking.\n");
#endif     
	int ar1[] = {65, 234, 676, 3, 98, 35, 297, 34, 12};
	int ar1_sorted[] = {3, 12, 34, 35, 65, 98, 234, 297, 676};
	int ar2[] = {5, 93, 4, 54, 2, 876, 11, 123};
	int ar2_sorted[] = {2, 4, 5, 11, 54, 93, 123, 876};
	int ok = 1, i;
        char obuf1[100], obuf2[100];

	sort(ar1, 9);
	sort(ar2, 8);

	for (i = 0; i < 9; i++) {
		if (ar1[i] != ar1_sorted[i]) {
			ok = 0;
			break;
		}
	}
        if (!ok) {
            pr_array(obuf1, ar1, 9);
            pr_array(obuf2, ar1_sorted, 9);
            fail("[Task 2.5] Array {65, 234, 676, 3, 98, 35, 297, 34, 12} not ordered correctly.\nYour array: %s.\nShould be: %s\n", obuf1, obuf2);
        }
	for (i = 0; i < 8; i++) {
		if (ar2[i] != ar2_sorted[i]) {
			ok = 0;
			break;
		}
	}
        if (!ok) {
            pr_array(obuf1, ar2, 8);
            pr_array(obuf2, ar2_sorted, 8);
            fail("[Task 2.5] Array {5, 93, 4, 54, 2, 876, 11, 123} not ordered correctly.\nYour array: %s.\nShould be: %s\n", obuf1, obuf2);
        }
        
}
END_TEST

START_TEST(test_count_vowels)
{
#if defined(__CYGWIN__)
    fail("Local tests not working on Cygwin. Send code to server for checking.\n");
#endif         
	char str1[] = "This is a test string with vowels. Also Uppercase.";
	char str2[] = "Thś s tst strng wtht vwls. ls pprcs.";
	int ret;

	ret = count_vowels(str1);
	fail_unless(ret == 14, "[Task 2.6] String \"%s\" has 14 vowels, not %d\n",
                str1, ret);

	ret = count_vowels(str2);
	fail_unless(ret == 0, "[Task 2.6] String \"%s\" has 0 vowels, not %d\n",
                str2, ret);
}
END_TEST

int open_and_read(const char *fname, char *buffer, size_t size)
{
    int n = 0;
    char fnamebuf[80];
    sprintf(fnamebuf, "test/%s", fname);
    FILE *f = fopen(fnamebuf, "r");
    if (!f) {
        f = fopen(fname, "r");
    }
    if (f) {
        n = fread(buffer, 1, size, f);
        buffer[n] = 0;
    }    
    return n;
}

START_TEST(test_remove_comments)
{
#if defined(__CYGWIN__)
    fail("Local tests not working on Cygwin. Send code to server for checking.\n");
#endif         
    char outbuf[2000] = "", inbuf[2000] = "", refbuf[2000] = "";
    int n = 0;
    n = open_and_read("testfile.conf", inbuf, sizeof(inbuf));
    fail_unless(n, "[Task 2.7] Reading testfile failed.\n");
    
    n = open_and_read("test-ref.txt", refbuf, sizeof(refbuf));
    fail_unless(n, "[Task 2.7] Reading reference file failed.\n");
    
    remove_comments(outbuf, inbuf);
    char infobuf[100];
    n = mycompare(outbuf, refbuf, infobuf);
    fail_unless(!n, "[Task 2.7] Output from test/testfile.conf differs from reference output (%s). Check test/test-ref.txt\n",
            infobuf);
    
    n = open_and_read("hidden_test.txt", inbuf, sizeof(inbuf));
    fail_unless(n, "[Task 2.7] Reading hidden testfile failed.\n");
    
    n = open_and_read("hidden_ref.txt", refbuf, sizeof(refbuf));
    fail_unless(n, "[Task 2.7] Reading hidden reference file failed.\n");
    
    remove_comments(outbuf, inbuf);
    n = mycompare(outbuf, refbuf, infobuf);
    fail_unless(!n, "[Task 2.7] Output from test/hidden_test.txt differs from reference output (%s). Check test/hidden-ref.txt\n",
            infobuf);
}
END_TEST

int main(int argc, const char *argv[])
{
    srand((unsigned)time(NULL));
    Suite *s = suite_create("Test-2.5-7");

    tmc_register_test(s, test_sort, "2.5");
    tmc_register_test(s, test_count_vowels, "2.6");
    tmc_register_test(s, test_remove_comments, "2.7");    
    return tmc_run_tests(argc, argv, s);
}
