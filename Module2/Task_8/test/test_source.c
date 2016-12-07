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
            sprintf(infostr, "your output is longer than expected: character: '%c', position: %d, line: %d",
                    *student, i, line);
            return -1;
        }
        if (*student != *model) {
            sprintf(infostr, "position: %d, line: %d, your output: '%c' , expected: '%c'", 
                    i, line, *student, *model);
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

int myNcompare(char *student, char *model, char *infostr, int n)
{
    int i = 0, line = 1;
    while(*student && n--) {
        if (!(*model)) {
            sprintf(infostr, "your output is longer than expected: character: '%c', position: %d, line: %d",
                    *student, i, line);
            return -1;
        }
        if (*student != *model) {
            sprintf(infostr, "position: %d, line: %d, your output: '%c' , expected: '%c'", 
                    i, line, *student, *model);
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


START_TEST(test_es_print)
{
#if defined(__CYGWIN__)
    fail("Local tests not working on Cygwin. Send code to server for checking.\n");
#endif         
    freopen("mockoutput", "w", stdout);
    char *test1 = "Foobar123nelja!viisibaz";
    es_print(test1);
    fflush(stdout);
    FILE *fp = fopen("mockoutput", "r");
    char str [100];
    fgets(str, 100, fp);

    char infostr[100] = "";
    int ret = mycompare(str, "Foobar123nelja", infostr);
    fail_unless(!ret, "[Task 2.8.a] Incorrect output with input \"%s\": %s\n",
            test1, infostr);
    freopen("mockoutput", "w", stdout);
    char *test2 = "v!iisibazfoo";
    es_print(test2);
    fflush(stdout);
    fp = fopen("mockoutput", "r");
    char str2 [100];
    fgets(str2, 100, fp);
    
    infostr[0] = 0;
    ret = mycompare(str2, "v", infostr);
    fail_unless(!ret, "[Task 2.8.a] Incorrect output with input \"%s\": %s\n",
            test2, infostr);
}
END_TEST

START_TEST(test_es_length)
{
#if defined(__CYGWIN__)
    fail("Local tests not working on Cygwin. Send code to server for checking.\n");
#endif         
	unsigned int ret;

        char *test1 = "Foobar123nelja!viisibaz";
	ret = es_length(test1);
	fail_unless(ret == 14, "[Task 2.8.b] Wrong length with string \"%s\". Got %d, expected 14\n",
                test1, ret);

        char *test2 = "!viisibazfoo";
	ret = es_length(test2);
	fail_unless(ret == 0, "[Task 2.8.b] Wrong length with string \"%s\". Got %d, expected 0\n",
                test2, ret);
}
END_TEST

START_TEST(test_es_copy)
{
#if defined(__CYGWIN__)
    fail("Local tests not working on Cygwin. Send code to server for checking.\n");
#endif     
	char buf[100];
	int ret;

        char *test1 = "Foobar123neljaa!viisibaz";
        char *ref1 = "Foobar123neljaa!";
	ret = es_copy(buf, test1);
	fail_unless(ret == 15, "[Task 2.8.c] Wrong return value with string \"%s\". Got %d, expected 15\n",
                test1, ret);
        char infostr[100] = "";
        ret = myNcompare(buf, ref1, infostr, strlen(ref1));
	fail_unless(!ret, "[Task 2.8.c] Copied string is incorrect with input \"%s\": %s\n",
                test1, infostr);

        infostr[0] = 0;
        char *test2 = "!viisibazfoo";
	ret = es_copy(buf, test2);
	fail_unless(ret == 0, "[Task 2.8.c] Wrong return value with string \"%s\". Got %d, expected 15\n",
                test2, ret);
        ret = myNcompare(buf, "!", infostr, 1);
	fail_unless(!ret, "[Task 2.8.c] Copied string is incorrect with input \"%s\": %s\n",
                test1, infostr);
}
END_TEST

START_TEST(test_es_token)
{
 #if defined(__CYGWIN__)
    fail("Local tests not working on Cygwin. Send code to server for checking.\n");
#endif        
	char str1[] = "Foobarbaz!asd,w";
        char *ref1 = "Foobarbaz!";
	char str2[] = "Foobar,baz!asd,w";
        char *ref2 = "Foobar!baz!";
        char orig1[80], orig2[80];
	char *ret;
        int ret2;

        strcpy(orig1, str1);
        strcpy(orig2, str2);
	ret = es_token(str1, ',');
	fail_unless(ret == NULL, "[Task 2.8.d] With string \"%s\" should have returned NULL\n",
                orig1);
        char infostr[100] = "";
        ret2 = myNcompare(str1, ref1, infostr, strlen(ref1));
	fail_unless(!ret2, "[Task 2.8.d] Processed string is incorrect with input (\"%s\", ','): %s\n",
                orig1, infostr);

	ret = es_token(str2, ',');
	fail_unless(ret == &(str2[7]), "[Task 2.8.d] Wrong return value with string \"%s\": Got %p, expected %p\n",
                orig2, ret, &(str2[7]));
        infostr[0] = 0;
        ret2 = myNcompare(str2, ref2, infostr, strlen(ref2));
	fail_unless(!ret2, "[Task 2.8.d] Processed string is incorrect with input (\"%s\", ','): %s\n",
                orig1, infostr);
}
END_TEST

START_TEST(test_es_demo)
{
#if defined(__CYGWIN__)
    fail("Local tests not working on Cygwin. Send code to server for checking.\n");
#endif         
	char str[] = "yksi,kaksi,kymmenen!nelja";
	char sol[] = "yksi 4\nkaksi 5\nkymmenen 8\n";
	int ret, n = 0;

	freopen("mockoutput", "w", stdout);
	ret = es_demo(str, ',');
    fflush(stdout);
    FILE *fp = fopen("mockoutput", "r");
    char buf [100];
    while (fgets(buf + n, 100 - n, fp) != NULL) n = strlen(buf);
    fclose(fp);
    fail_unless(ret == 3, "[Task 2.8.e] Incorrect return value: %d\n", ret);
    char infostr[100] = "";
    ret = mycompare(buf, sol, infostr);
    fail_unless(!ret, "[Task 2.8.e] Incorrect output, expected\n%s: %s",
            sol, infostr);

}
END_TEST

int main(int argc, const char *argv[])
{
	Suite *s = suite_create("Test-2.8");

        /* TODO: define tests */
	tmc_register_test(s, test_es_print, "2.8.a");
	tmc_register_test(s, test_es_length, "2.8.b");
	tmc_register_test(s, test_es_copy, "2.8.c");
	tmc_register_test(s, test_es_token, "2.8.d");
	tmc_register_test(s, test_es_demo, "2.8.e");
        
	return tmc_run_tests(argc, argv, s);
}
