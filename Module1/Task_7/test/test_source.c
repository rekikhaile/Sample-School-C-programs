#include <check.h>
#include "tmc-check.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../src/calculator.h"


START_TEST(test_get_character)
{
#if defined(__CYGWIN__)
    fail("Local tests not working on Cygwin. Send code to server for checking.\n");
#endif     
    char inp [100];
    
    sprintf(inp, "2");
    FILE *mock_input = freopen("mockinput", "w+", stdin);
    fputs(inp, mock_input);
    fseek(mock_input, 0, SEEK_SET);
    char a = get_character();
    fail_unless(a == '2', "[Task 1.7.a] get_character returned wrong character\n");

    fseek(mock_input, 0, SEEK_SET);
    sprintf(inp, " 3 ");
    fputs(inp, mock_input);
    fseek(mock_input, 0, SEEK_SET);
    a = get_character();
    fail_unless(a == '3', "[Task 1.7.a] get_character returned wrong character -- remember to ignore space\n");
    fclose(mock_input);
}
END_TEST

double parse_helper(FILE *f, char *input, double par, double res)
{
    fseek(f, 0, SEEK_SET);
    fputs(input, f);
    fseek(f, 0, SEEK_SET);
    double r = parse_operation(par);
    return r;
}

START_TEST(test_parse_operation)
{
#if defined(__CYGWIN__)
    fail("Local tests not working on Cygwin. Send code to server for checking.\n");
#endif     
    FILE *mif = freopen("mockinput", "w+", stdin);

    fseek(mif, 0, SEEK_SET);
    fputs("+ 3\n", mif);
    fseek(mif, 0, SEEK_SET);
    double r = parse_operation(0);
    if (r == '3') {
        fail("[Task 1.7.b] parse_operation(0) and user input\"+ 3\" returned %d which is same as '3'.\n"
                "Did you remember to convert character to number?\n", '3');
    }
    
    double ret = parse_helper(mif, "+ 4\n", 3, 7);
    fail_unless(ret == 7,
            "[Task 1.7.b] parse_operation(3) and user input \"+ 4\" should return %f, but it returned %f\n",
            7.0, ret);
    ret = parse_helper(mif, "*3\n", 5, 15);
    fail_unless(ret == 15,
            "[Task 1.7.b] parse_operation(5) and user input \"*3\" should return %f, but it returned %f\n",
            15.0, ret);
    ret = parse_helper(mif, "/   4\n", 2, 0.5);
    fail_unless(ret == 0.5,
            "[Task 1.7.b] parse_operation(2) and user input \"/   4\" should return %f, but it returned %f\n",
            0.5, ret);
    ret = parse_helper(mif, "+ +\n", 2, -20000);
    /*    fail_unless(ret <= -20000,
            "[Task 1.7.b] parse_operation(2) and user input \"+ +\" should return <= %f, but it returned %f\n",
            -20000.0, ret);*/
    fail_unless(ret <= -20000, "user input '+ +' should return error\n");
    fclose(mif);
}
END_TEST


int calc_helper(FILE *f, char *input, double res)
{
    //fseek(f, 0, SEEK_SET);
    fputs(input, f);
    fseek(f, 0, SEEK_SET);
    double r = calculator();
    return res == r;
}

START_TEST(test_calculator)
{
#if defined(__CYGWIN__)
    fail("Local tests not working on Cygwin. Send code to server for checking.\n");
#endif     
    FILE *mif = freopen("mockinput", "w+", stdin);

    if (!mif)
        fail("File not opened!\n");
    
    int a = rand() % 10, b = rand() % 10;
    fprintf(mif, "%d + %d\n", a, b);
    fseek(mif, 0, SEEK_SET);
    double ret = calculator();
    fail_unless(ret == (a + b), "[Task 1.7.c] \"%d + %d\\n\" should return %f, but it returned %f\n",
            a, b, (double) a + b, ret);

    fseek(mif, 0, SEEK_SET);
    a = rand() % 10;
    b = rand() % 10;
    int c = rand() % 10;
    int d = rand() % 10;
    fprintf(mif, "%d+%d-%d*%d\n", a, b, c, d);
    fseek(mif, 0, SEEK_SET);
    ret = calculator();
    fail_unless(ret == ((a + b - c) * d), "[Task 1.7.c] \"%d+%d-%d*%d\\n\" should return %f, but it returned %f\n",
            a, b, c, d, (double)(a + b -c) * d, ret);
    
    fclose(mif);
}
END_TEST


START_TEST(test_calc_prec)
{
#if defined(__CYGWIN__)
    fail("Local tests not working on Cygwin. Send code to server for checking.\n");
#endif 
    FILE *mif = freopen("mockinput", "w+", stdin);

    int a,b,c,d;
    a = rand() % 10;
    b = rand() % 10;
    c = rand() % 10;
    d = rand() % 10;
    fprintf(mif, "%d+%d-(%d*%d)\n", a, b, c, d);
    fseek(mif, 0, SEEK_SET);
    double ret = calculator();
    fail_unless(ret == (a + b - (c * d)),
            "[Task 1.7.d] \"%d+%d-(%d*%d)\\n\" should return %f, but it returned %f\n",
            a, b, c, d, (double) a + b - (c *d), ret);
    fclose(mif);
}
END_TEST


int main(int argc, const char *argv[])
{
        srand((unsigned)time(NULL));
	Suite *s = suite_create("Test-1.7");

	tmc_register_test(s, test_get_character, "1.7.a");
        tmc_register_test(s, test_parse_operation, "1.7.b");
        tmc_register_test(s, test_calculator, "1.7.c");
        tmc_register_test(s, test_calc_prec, "1.7.d");        
        
	return tmc_run_tests(argc, argv, s);
}
