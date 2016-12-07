#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tmc-check.h"
#include "../src/diagram.h"


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
        if (*student == '\n') {
            line++;
            i = -1;
        }
        student++; model++; i++;
    }
    if (*model) {
        sprintf(infostr, "output correct until position: %d, but shorter than expected\n",
                i);
        return -1;
    }
    return 0;
}

// Sometimes Netbeans TMC plugin pukes when it gets non-ASCII characters
// in test output. Trying to avoid that
void remove_nonascii(char *str)
{
    while (*str) {
        if (*str & 0x80)
            *str = '?';
        str++;
    }
}


START_TEST(test_diagram) {
    char infostr[100];
    const char *pop_l[] = { "China", "India", "Russia", "Germany", "Finland", NULL};
    float pop_v[] = { 1363840000, 1242700000, 146019512, 80716000, 5453784, 0 };
    char *rightstr = "China                #################### 1363840000.000000\n"
        "India                ##################   1242700032.000000\n"
        "Russia               ##                   146019520.000000\n"
        "Germany              #                    80716000.000000\n"
        "Finland                                   5453784.000000\n";
    
    freopen("mockoutput", "w", stdout);
    diagram(pop_l, pop_v);
    fflush(stdout);
    FILE *fp = fopen("mockoutput", "r"); 
    char str [400]; 
    int n = fread(str, 1, sizeof(str)-1, fp); 
    str[n] = 0; 
    fclose(fp);
    
    int res = mycompare(str, rightstr, infostr);
    if (res) {
        remove_nonascii(str);
        fail("[Task 6.1] Your output:\n%s\nReference output:\n%s\nReason: %s\n",
                str, rightstr, infostr);
    }
    
    const char *weight_l[] = { "Hamster", "Rabbit", "Cat", "Guinea pig", NULL };
    float weight_v[] = { 0.12, 2.5, 3.3, 1.04, 0 };
    char *rightstr2 = "Hamster                                   0.120000\n"
        "Rabbit               ###############      2.500000\n"
        "Cat                  #################### 3.300000\n"
        "Guinea pig           ######               1.040000\n";
    freopen("mockoutput2", "w", stdout);
    diagram(weight_l, weight_v);
    fflush(stdout);
    fp = fopen("mockoutput2", "r");
    n = fread(str, 1, sizeof(str)-1, fp); 
    str[n] = 0; 
    fclose(fp);
    
    res = mycompare(str, rightstr2, infostr);
    if (res) {
        remove_nonascii(str);
        fail("[Task 6.1] Your output:\n%s\nReference output:\n%s\nReason: %s\n",
                str, rightstr2, infostr);
    }
    
}
END_TEST


int main(int argc, const char *argv[]) {
    srand((unsigned)time(NULL));
    Suite *s = suite_create("Test-6.1");

    /* TODO: define tests */
    tmc_register_test(s, test_diagram, "6.1");

    return tmc_run_tests(argc, argv, s);
}
