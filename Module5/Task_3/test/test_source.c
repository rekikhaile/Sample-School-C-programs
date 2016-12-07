#include <check.h>
#include "tmc-check.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../src/findstr.h"

START_TEST(test_find_string) 
{
    char *files[] = {"nonexistent.txt", "empty.txt", "text.txt"};
    char *strings[] = {"Wow!", "Such\nnewline", "much not there", "amaze"};
    long offsets[] = {0, 100};
    long results[][3][4] = {{{-1, -1, -1, -1}, {-1, -1, -1, -1}, {93, 104, -1, 97}}, {{-1, -1, -1, -1}, {-1, -1, -1, -1}, {-1, 104, -1, -1}}};
    int i, j, k;
    long ret;
    for (k = 0; k < 2; k++) {
        for (j = 0; j < 3; j++) {
            for (i = 0; i < 4; i++) {
                ret = find_string(strings[i], files[j], offsets[k]);
                fail_unless(ret == results[k][j][i], "[Task 5.3] Should have returned %ld for file string %s, file %s and offset %ld. You returned %ld.", results[k][j][i], strings[i], files[j], offsets[k], ret);
            }
        }
    }
    
    ret = find_string("Catenet", "rfc791.txt", 0);
    int ans = 93687;
    fail_unless (ret == ans, "[Task 5.3] Should have returned %ld for file string %s, file %s and offset %ld. You returned %ld.", ans, "Catenet", "rfc791.txt", 0, ret);
}
END_TEST

int main(int argc, const char *argv[])
{
    	Suite *s = suite_create("Test-5.3");

        /* TODO: define tests */
	tmc_register_test(s, test_find_string, "5.3");
        
	return tmc_run_tests(argc, argv, s);
}
