#include <check.h>
#include "tmc-check.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../src/source.h"

// SEE man dup

int mycompare(char *student, char *model, char *infostr)
{
    int i = 0, line = 1;
    while(*student) {
        if (!(*model)) {
            sprintf(infostr, "your output is longer than expected: character: '%c', position: %d, line: %d\n",
                    *student, i, line);
            return -1;
        }
        if (*student != *model) {
            sprintf(infostr, "position: %d, line: %d, your output: '%c' , expected: '%c'\n", 
                    i, line, *student, *model);
            return -1;
        }
        if (*student == '\n') line++;
        student++; model++; i++;
    }
    if (*model) {
        sprintf(infostr, "output correct until position: %d, but shorter than expected\n",
                i);
        return -1;
    }
    return 0;
}


START_TEST(test_multi_table)
{
#if defined(__CYGWIN__)
fail("Local tests not working on Cygwin. Send code to server for checking.\n");
#endif
FILE *mock_input = freopen("mockinput", "w+", stdin);
fputs("5 8", mock_input);
fseek(mock_input, 0, SEEK_SET);
freopen("mockoutput", "w", stdout);
multi_table();
fflush(stdout);
FILE *fp = fopen("mockoutput", "r");
char str [200];
char infostr[100];
int n = fread(str, 1, sizeof(str), fp);
str[n] = 0;
char *rightstr = "   1   2   3   4   5\n   2   4   6   8  10\n   3   6   9  12  15\n   4   8  12  16  20\n   5  10  15  20  25\n   6  12  18  24  30\n   7  14  21  28  35\n   8  16  24  32  40\n";
int res = mycompare(str, rightstr, infostr);
fail_unless(!res, "[Task 1.5] Invalid output for input \"5 8\": %s\n", infostr);
fclose(fp);
}
END_TEST



int test_file_pair(char *outbuf, const char *testfile, const char *hexfile)
{
    FILE *f, *fop, *fref;
    f = fopen(testfile, "r");
    if (!f) {
        char fbuf[80];
        sprintf(fbuf, "test/%s", testfile);
        f = fopen(fbuf, "r");
        if (!f) {
            sprintf(outbuf, "[Task 1.6] Opening \"%s\" failed. Contact teacher.\n",
                    testfile);
            return -1;
        }
    }
    freopen("mockoutput", "w", stdout);
    hexdump(f);
    fflush(stdout);
    fclose(f);

    fop = fopen("mockoutput", "r");
    fref = fopen(hexfile, "r");
    if (!fref) {
        char fbuf[80];
        sprintf(fbuf, "test/%s", hexfile);
        fref = fopen(fbuf, "r");
        if (!fref) {
            sprintf(outbuf, "[Task 1.6] Opening \"%s\" failed. Contact teacher.\n",
                    hexfile);
            return -1;
        }
    }
    
    int i = 0;
    char str[1024], str_ref[1024];
    while((fgets(str, 1024, fop)) != NULL) {
        i++;
        if ((fgets(str_ref, 1024, fref)) == NULL) {
            sprintf(outbuf, "[Task 1.6] Your hexdump for file \"%s\" has too many lines\n",
                    testfile);
            return -1;
        }
        if(strcmp(str, str_ref)) {
            sprintf(outbuf,
                    "[Task 1.6] Hexdump for file \"%s\": line %d has invalid output\n",
                    testfile, i);
            return -1;
        }
    }
    if (i == 0) {
        sprintf(outbuf, "[Task 1.6] There was no output\n");
        return -1;
    }
    fclose(fref);
    fclose(fop);
    return 0;
}

START_TEST(test_hexdump) 
{
#if defined(__CYGWIN__)
    fail("Local tests not working on Cygwin. Send code to server for checking.\n");
#endif    
    char outbuf[200];
    if (test_file_pair(outbuf, "test-file.txt", "test-dump.hex") == -1) {
        fail(outbuf);
    }
    
}
END_TEST


int main(int argc, const char *argv[])
{
    srand((unsigned)time(NULL));
	Suite *s = suite_create("Test-1.5-6");

        tmc_register_test(s, test_multi_table, "1.5");
        tmc_register_test(s, test_hexdump, "1.6");
        
	return tmc_run_tests(argc, argv, s);
}
