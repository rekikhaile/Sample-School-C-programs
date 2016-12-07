#include <check.h>
#include "tmc-check.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../src/filestats.h"

// Bad bad.. global variable, but will let it be this time.
struct stats {
    int lines;
    int words;
    int common;
} generated;


START_TEST(test_line_count)
{   
    char *files[] = {"nonexistent.txt", "empty.txt", "oneline.txt", "lines.txt", "rfc791.txt", "generated.txt" };
    int results[] = {-1, 0, 1, 7, 2887, 0};
    int i, ret;
    
    results[5] = generated.lines;

    for (i = 0; i < 6; i++) {
        ret = line_count(files[i]);
        fail_unless(ret == results[i], "[Task 5.2.a] Should have returned %d for file %s. You returned %d.", results[i], files[i], ret);
    }
}
END_TEST

START_TEST(test_word_count)
{
    char *files[] = {"nonexistent.txt", "empty.txt", "oneline.txt", "lines.txt", "rfc791.txt", "generated.txt" };
    int results[] = {-1, 0, 9, 9, 9843, 0};
    int i, ret;

    results[5] = generated.words;
    
    for (i = 0; i < 6; i++) {
        ret = word_count(files[i]);
        fail_unless(ret == results[i], "[Task 5.2.b] Should have returned %d for file %s. You returned %d.", results[i], files[i], ret);
    }
}
END_TEST

void printChar(char *buffer, int c)
{
    if (c == -1)
        sprintf(buffer, "EOF (-1)");
    else if (c < 32)
        sprintf(buffer, "%d", c);
    else if (c < 128)
        sprintf(buffer, "'%c' (%d)", c, c);
    else
	    sprintf(buffer, "%d", c);
}

START_TEST(test_most_common)
{
    char *files[] = {"nonexistent.txt", "empty.txt", "oneline.txt", "lines.txt", "rfc791.txt", "generated.txt" };
    int results[] = {-1, 0, 'e', 'f', 'e', 0};
    int i;
    int ret;

    results[5] = generated.common;
    
    char buf[16];
    char buf2[16];
    for (i = 0; i < 6; i++) {
        ret = most_common(files[i]);
        printChar(buf, results[i]);
        printChar(buf2, ret);
        fail_unless(ret == results[i],
                "[Task 5.2.c] Should have returned %s for file %s. You returned %s.",
                buf, files[i], buf2);
    }
}
END_TEST

void generate_file(void)
{
    FILE *f = fopen("generated.txt", "w");
    if (!f) {
        fprintf(stderr, "Generating file failed!\n");
        return;
    }
    generated.common = rand() % 20 + 'a';
    generated.lines = rand() % 10 + 2;
    int words = rand() % 10 + 2;
    generated.words = words * generated.lines;
    
    char one_wd[6];
    sprintf(one_wd, "%c%c%c ", generated.common, generated.common, generated.common);
    for (int i = 0; i < generated.lines; i++) {
        for (int j = 0; j < words; j++) {
            if (fputs(one_wd, f) == EOF) {
                fprintf(stderr, "Writing to file failed!\n");
                fclose(f);
                return;
            }
        }
        fputs("\n", f);
    }
    fclose(f);
}


int main(int argc, const char *argv[])
{
    Suite *s = suite_create("Test-5.2");

    srand((unsigned)time(NULL));    
    generate_file();

    tmc_register_test(s, test_line_count, "5.2.a");
    tmc_register_test(s, test_word_count, "5.2.b");
    tmc_register_test(s, test_most_common, "5.2.c");
    
    return tmc_run_tests(argc, argv, s);
}
