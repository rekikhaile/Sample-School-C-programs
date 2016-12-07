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

START_TEST(test_three_lines)
{
#if defined(__CYGWIN__)
    fail("Local tests not working on Cygwin. Send code to server for checking.\n");
#endif
    freopen("mockoutput", "w", stdout);
    three_lines();
    fclose(stdout);
    FILE *fp = fopen("mockoutput", "r");
    char str [100];
    memset(str, 0, sizeof(str));
    fread(str, 99, 1, fp);
    char infostr[100] = "";
    int ret = mycompare(str, "One\nTwo\nThree\n", infostr);
    fail_unless(!ret, "[Task 1.1] Incorrect output: %s\n", infostr);
    fclose(fp);
}
END_TEST

START_TEST(test_data_types)
{
#if defined(__CYGWIN__)
    fail("Local tests not working on Cygwin. Send code to server for checking.\n");
#endif    
    freopen("mockoutput", "w", stdout);
    data_types();
    fclose(stdout);
    FILE *fp = fopen("mockoutput", "r");
    char str [100];
    memset(str, 0, sizeof(str));
    fgets(str, 100, fp);
    char infostr[100] = "";
    int ret = mycompare(str, "400 -5.0 40\n", infostr);
    fail_unless(!ret,"[Task 1.2] Incorrect output: %s\n", infostr);
    fclose(fp);
}
END_TEST

START_TEST(test_simple_sum)
{
#if defined(__CYGWIN__)
fail("Local tests not working on Cygwin. Send code to server for checking.\n");
#endif
FILE *mock_input = freopen("mockinput", "w+", stdin);
fputs("6 -8", mock_input);
fseek(mock_input, 0, SEEK_SET);
freopen("mockoutput", "w", stdout);
simple_sum();
fflush(stdout);
FILE *fp = fopen("mockoutput", "r");
char str [100];
memset(str, 0, sizeof(str));
fgets(str, 100, fp);
fail_unless(!strcmp(str, "-2\n"), "[Task 1.3] When giving input 6 -8, \"-2\\n\" should be printed");
fclose(fp);
mock_input = freopen("mockinput", "w+", stdin);
fputs("68 a", mock_input);
fseek(mock_input, 0, SEEK_SET);
freopen("mockoutput", "w", stdout);
simple_sum();
fflush(stdout);
fp = fopen("mockoutput", "r");
fgets(str, 100, fp);
fail_unless(!strcmp(str, "ERROR\n"), "[Task 1.3] When giving input '68 a', \"ERROR\\n\" should be printed");
fclose(fp);
}
END_TEST


START_TEST(test_simple_math)
{
#if defined(__CYGWIN__)
fail("Local tests not working on Cygwin. Send code to server for checking.\n");
#endif
FILE *mock_input = freopen("mockinput", "w+", stdin);
fputs("11 * 3", mock_input);
fseek(mock_input, 0, SEEK_SET);
freopen("mockoutput", "w", stdout);
simple_math();
fflush(stdout);
FILE *fp = fopen("mockoutput", "r");
char str [100];
fgets(str, 100, fp);
fail_unless(!strcmp(str, "33\n"), "[Task 1.4] When giving input 11 * 3, \"33\\n\" should be printed");
fclose(fp);
mock_input = freopen("mockinput", "w+", stdin);
fputs("6 a 8", mock_input);
fseek(mock_input, 0, SEEK_SET);
freopen("mockoutput", "w", stdout);
simple_math();
fflush(stdout);
fp = fopen("mockoutput", "r");
fgets(str, 100, fp);
fail_unless(!strcmp(str, "ERROR\n"), "[Task 1.4] When giving input 6 a 8, \"ERROR\\n\" should be printed");
fclose(fp);
}
END_TEST



int main(int argc, const char *argv[])
{
    srand((unsigned)time(NULL));
	Suite *s = suite_create("Test-1.1-4");

	tmc_register_test(s, test_three_lines, "1.1");
        tmc_register_test(s, test_data_types, "1.2");
        tmc_register_test(s, test_simple_sum, "1.3");
        tmc_register_test(s, test_simple_math, "1.4");
        
	return tmc_run_tests(argc, argv, s);
}
