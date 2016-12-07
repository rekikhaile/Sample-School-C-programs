#include <check.h>
#include <assert.h>
#include "tmc-check.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../src/students.h"
#include "../src/students.h"

void print_student(Student *st)
{
    printf("%s (%s), Course: %s, Age: %d\n", st->name, st->id, st->course, st->age);
    if (st->numPoints) {
        printf(" -- Points: ");
        for (unsigned int j = 0; j < st->numPoints; j++)
            printf("%f  ", st->points[j]);
        printf("\n");
    } 
}

void print_students(Students *s)
{
    Student *st = s->array;
    for (unsigned int i = 0; i < s->count; i++) {
        print_student(st);
        st++;
    }
}

void initialize_reg(Students *reg) {
    assert(reg != NULL);
    reg->count = 0;
    reg->array = NULL;
    create_student(reg, "Teemu", "00000A", 20, "ELEC-A1100");
    create_student(reg, "Matti", "12345B", 28, "ELEC-A1100");
    create_student(reg, "Wow", "33333C", 28, "ELEC-A1100");
    create_student(reg, "Much", "98765H", 28, "ELEC-A1100");
    create_student(reg, "Suchcourse", "12121R", 28, "ELEC-A1111");
    create_student(reg, "Amaze", "11111T", 28, "ELEC-A1111");
}

void printPoints(char *buffer, Student *s)
{
    buffer[0] = 0;
    char buf[16];
    for (unsigned int i = 0; i < s->numPoints; i++) {
        sprintf(buf, "%f", s->points[i]);
        strcat(buffer, buf);
        if (i < s->numPoints - 1)
            strcat(buffer, ",");
    }
}

int compare_regs(Students *model, Students *student, char *info)
{
    if (model->count != student->count) {
        sprintf(info, "Student count doesn't match. Your count: %d, model: %d", student->count, model->count);
        return 0;
    }
    unsigned int i;
    Student mstud;
    Student sstud;
    for (i = 0; i < model->count; i++) {
        mstud = model->array[i];
        sstud = student->array[i];
        if (strcmp(mstud.name, sstud.name)) {
            sprintf(info, "Name of student %d loaded incorrectly. You: '%s', model: '%s'",
                    i, sstud.name, mstud.name);          
            return 0;
        }
        if (strcmp(mstud.course, sstud.course)) {
            sprintf(info, "Course of student %d loaded incorrectly. You: '%s', model: '%s'",
                    i, sstud.course, mstud.course);            
            return 0;            
        }
        if (strcmp(mstud.id, sstud.id)) {
            sprintf(info, "ID of student %d loaded incorrectly. You: '%s', model: '%s'",
                    i, sstud.id, mstud.id);            
            return 0;            
        }
        if (mstud.age != sstud.age) {
            sprintf(info, "Age of student %d loaded incorrectly. You: %d, model: %d",
                    i, sstud.age, mstud.age);            
            return 0;            
        }
        if (mstud.numPoints != sstud.numPoints) {
            sprintf(info, "numPoints of student %d loaded incorrectly. You: %d, model: %d",
                    i, sstud.numPoints, mstud.numPoints);        
            return 0;            
        }
        char mpoints[80];
        char spoints[80];
        printPoints(mpoints, &mstud);
        printPoints(spoints, &sstud);
        
        unsigned int j;
        for (j = 0; j < mstud.numPoints; j++) {
            if (mstud.points[j] != sstud.points[j]) {
                sprintf(info, "Points of student %d loaded incorrectly. You: {%s}, model: {%s}",
                        i, spoints, mpoints);
                return 0;
            }
        }
    }
    return 1;
}

int compare_files(FILE *model, FILE *student, char *info)
{
    char m, s;
    int i = 1;
    while ((m = fgetc(model)) != EOF) {
        s = fgetc(student);
        if(s != m) {
            if (s == EOF) {
                sprintf(info, "Your file is shorter than expected.");
            } else {
                sprintf(info, "Your file differs from the model at byte number %d.", i);
            }
            return 0;
        }
        i++;
    }
    if (fgetc(student) != EOF) {
        sprintf(info, "Your file is longer than expected. %d", i);
        return 0;
    }
    return 1;

}

void release_memory(Students *s) {
    unsigned int i;
    if (s->array) {
        for (i = 0; i < s->count; i++) {
            Student *st = &(s->array[i]);
            if (st->name)
                free(st->name);
            if (st->points)
                free(st->points);
        }
        free(s->array);
    }
}

START_TEST(test_parse_cmdline)
{
    int i;
    char **args = NULL;
    char cmdline[] = "cmdline -s 20 -r -t parameter -p 20 filename";
    char orig_cmd[60];
    strcpy(orig_cmd, cmdline);
    const char *argsol[] = { "cmdline", "-s", "20", "-r", "-t", "parameter", "-p",
        "20", "filename" };
    int count = parse_cmdline(&args, cmdline);
    if (count != 9) {
        free_cmdline(&args);
        fail("[Task 5.4.a] Incorrect argument count returned (%d instead of %d) for string '%s'.\n",
                count, 9, orig_cmd);

    }
    for (i = 0; i < 9; i++) {
        if (strcmp(args[i], argsol[i])) {
            char outbuf[160];
            snprintf(outbuf, 160, "[Task 5.4.a] Incorrect argument at index %d ('%s' instead of '%s') for string '%s'.\n",
                    i, args[i], argsol[i], orig_cmd);
            free_cmdline(&args);
            fail("%s", outbuf);
        }
    }
    free_cmdline(&args);
    args = NULL;
    //char cmdline2[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec sed bibendum sem, sed molestie nulla. Pellentesque dapibus id leo quis malesuada. Mauris ultricies felis purus, ac condimentum mauris auctor at. Duis pharetra enim arcu. Vivamus vel elit sed nunc malesuada posuere non sit amet urna. Ut pulvinar pharetra eleifend. Nullam id orci mollis, euismod tortor at, dapibus nisl. Sed tempor molestie mauris, sit amet faucibus lectus convallis eu. Duis eu purus at sem dapibus vestibulum eu nec mauris. Nullam in volutpat sapien. Curabitur luctus est quis lobortis tristique. Pellentesque convallis felis ut lorem cursus, rutrum viverra ante egestas. Sed sagittis.";
    char cmdline2[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec sed bibendum sem";
    //const char *argsol2[] = {"Lorem", "ipsum", "dolor", "sit", "amet,", "consectetur", "adipiscing", "elit.", "Donec", "sed", "bibendum", "sem,", "sed", "molestie", "nulla.", "Pellentesque", "dapibus", "id", "leo", "quis", "malesuada.", "Mauris", "ultricies", "felis", "purus,", "ac", "condimentum", "mauris", "auctor", "at.", "Duis", "pharetra", "enim", "arcu.", "Vivamus", "vel", "elit", "sed", "nunc", "malesuada", "posuere", "non", "sit", "amet", "urna.", "Ut", "pulvinar", "pharetra", "eleifend.", "Nullam", "id", "orci", "mollis,", "euismod", "tortor", "at,", "dapibus", "nisl.", "Sed", "tempor", "molestie", "mauris,", "sit", "amet", "faucibus", "lectus", "convallis", "eu.", "Duis", "eu", "purus", "at", "sem", "dapibus", "vestibulum", "eu", "nec", "mauris.", "Nullam", "in", "volutpat", "sapien.", "Curabitur", "luctus", "est", "quis", "lobortis", "tristique.", "Pellentesque", "convallis", "felis", "ut", "lorem", "cursus,", "rutrum", "viverra", "ante", "egestas.", "Sed", "sagittis."};
    const char *argsol2[] = {"Lorem", "ipsum", "dolor", "sit", "amet,", "consectetur", "adipiscing", "elit.", "Donec", "sed", "bibendum", "sem" };
    char orig_cmd2[120];
    strcpy(orig_cmd2, cmdline2);
    count = parse_cmdline(&args, cmdline2);
    if (count != 12) {
        free_cmdline(&args);
        fail("[Task 5.4.a] Incorrect argument count returned (%d instead of %d) for string '%s'",
                count, 100, orig_cmd2);        
    }
    for (i = 0; i < 12; i++) {
        if (strcmp(args[i], argsol2[i])){
            char outbuf[160];
            snprintf(outbuf, 160, "[Task 5.4.a] Incorrect argument at index %d ('%s' instead of '%s') for string '%s'",
                   i, args[i], argsol2[i], orig_cmd2);
            free_cmdline(&args);
            fail("%s", outbuf);
        }
    }
    free_cmdline(&args);
}
END_TEST


int testfuna(int argc, char **argv, Students *s)
{
    if (!s) return 0;
    if (strcmp(argv[1], "a1") || strcmp(argv[2], "a2")) return 0;
    if (argc != 3) return 0;

    return 1;
}

int testfunb(int argc, char **argv, Students *s)
{
    if (!s) return 0;
    if (strcmp(argv[1], "b1") || strcmp(argv[2], "b2") || strcmp(argv[3], "b3")) return 0;
    if (argc != 4) return 0;

    return 2;
}


struct commands cmdlist1[] = {
    { "testfuna", testfuna },
    { "testfunb", testfunb },
    { NULL, NULL }
};

START_TEST (test_process)
{
    Students s;
    char *argv[] = {"nonexistent"};
    char *argva[] = {"testfuna", "a1", "a2"};
    char *argvb[] = {"testfunb", "b1", "b2", "b3"};
    int ret;
    ret = process(cmdlist1, 1, argv, &s);
    fail_unless(ret == -1,
        "[Task 5.4.b] process() should have returned -1 with when given unknown command.");
    ret = process(cmdlist1, 3, argva, &s);
    fail_unless(ret == 1,
        "[Task 5.4.b] process() did not call the function given in command list (invalid return value from function).");
    ret = process(cmdlist1, 4, argvb, &s);
    fail_unless(ret == 2,
        "[Task 5.4.b] process() did not call the function given in command list (invalid return value from function).");

}
END_TEST

void showInput(char *buffer, char **input, int n)
{
    buffer[0] = 0;
    for (int i = 0; i < n; i++) {
        strcat(buffer, input[i]);
        if (i < n - 1)
            strcat(buffer, " ");
    }
}

START_TEST(test_create_find_delete)
{
    Students s, m;
    char infostr[1024];
    char **cmds = malloc(5*sizeof(char*));
    char agebuf[4];
    
    initialize_reg(&m);
    s.array = NULL;
    s.count = 0;
    char *invalid1[] = {"create", "invalid", "invalid", "invalid"};
    char *invalid2[] = {"create", "Name", "ID", "invalidage", "Course"};
    
    //Failing
    char buffer[80];
    buffer[0] = 0;
    int ret;

    if ((ret = process_create(4, invalid1, &s)) != -1) {
        showInput(buffer, invalid1, 4);
        release_memory(&m);
        free(cmds);
        fail("[Task 5.4.c] Invalid command line '%s' should return -1. You returned %d",
                buffer, ret);
    }
    if ((ret =process_create(5, invalid2, &s)) != -1) {
        showInput(buffer, invalid2, 5);
        release_memory(&m);
        free(cmds);
        fail("[Task 5.4.c] Invalid command line '%s' should return -1. You returned %d",
                buffer, ret);
    }
    // Correct
    //print_students(&s);
    unsigned int i;

    for (i = 0; i < m.count; i++) {
        Student ms = m.array[i];
        sprintf(agebuf, "%d", ms.age);
        cmds[0] = "create";
        cmds[1] = ms.name;
        cmds[2] = ms.id;
        cmds[3] = agebuf;
        cmds[4] = ms.course;
 //       printf("%s %s %s %s %s\n", cmds[0], cmds[1], cmds[2], cmds[3], cmds[4]);
        ret = process_create(5, cmds, &s);
        if (ret) {
            release_memory(&m);
            release_memory(&s);
            showInput(buffer, cmds, 5);
            free(cmds);
            fail("[Task 5.4.c] Valid command line '%s' should return 0. You returned %d",
                buffer, ret);
        }
    }
    int test = compare_regs(&m, &s, infostr);
    if (!test) {
        release_memory(&m);
        release_memory(&s);
        free(cmds); 
        fail("[Task 5.4.c] Invalid registry contents after multiple creates: %s", infostr);        
    }

    char *invalid3[] = {"find", "invalid"};
    char *valid1[] = {"find", "33333C", "ELEC-A1100"};
    if (!(ret = process_find(2, invalid3, &s))) {
        release_memory(&m);
        release_memory(&s);
        free(cmds); 
        showInput(buffer, invalid3, 2);
        fail("[Task 5.4.c] Invalid command line '%s' should return -1. You returned %d",
                buffer, ret);
    }
    if ((ret = process_find(3, valid1, &s))) {
        release_memory(&m);
        release_memory(&s);
        free(cmds); 
        showInput(buffer, valid1, 3);
        fail("[Task 5.4.c] Valid command line '%s' should return 0. You returned %d",
                buffer, ret);
    }

    for (i = 0; i < m.count; i++) {
        Student ms = m.array[i];
        sprintf(agebuf, "%d", ms.age);
        cmds[0] = "delete";
        cmds[1] = ms.id;
        cmds[2] = ms.course;
        //printf("%s %s %s %s %s\n", cmds[0], cmds[1], cmds[2]);
        ret = process_delete(3, cmds, &s);
        if (ret) {
            release_memory(&m);
            release_memory(&s);
            showInput(buffer, cmds, 3);
            free(cmds);
            fail("[Task 5.4.c] Valid command line '%s' should return 0. You returned %d",
                buffer, ret);
        }
        delete_student(&m, cmds[1], cmds[2]);
        test = compare_regs(&m, &s, infostr);
        if (!test) {
            release_memory(&m);
            release_memory(&s);
            free(cmds);
            fail("[Task 5.4.c] Invalid registry contents after multiple deletes: %s", infostr);        
        }
    }
    release_memory(&m);
    release_memory(&s);
    free(cmds);
}
END_TEST

START_TEST(test_points)
{
    Students s, m;
    char infostr[1024];
    int test;
    initialize_reg(&m);
    initialize_reg(&s);

    float points1[] = {3.0, 2.5, 1.0};
    char *pointscmd1[] = {"points", "98765H", "ELEC-A1100", "3.0", "2.5", "1.0"};

    process_points(6, pointscmd1, &s);
    set_points(&m, pointscmd1[1], pointscmd1[2], points1, 3);
    test = compare_regs(&m, &s, infostr);
    if (!test) {
        release_memory(&m);
        release_memory(&s);
        char buffer[80];
        showInput(buffer, pointscmd1, 6);
        fail("[Task 5.4.d] Invalid registry contents after adding points with command '%s': %s",
                buffer, infostr);
    }
    release_memory(&m);
    release_memory(&s);
}   
END_TEST

START_TEST(test_load_save)
{
    Students s, m;
    char infostr[1024];
    int test;
    FILE *mfile, *sfile;
    initialize_reg(&m);
    s.array = NULL;
    s.count = 0;

    char *loadcmd[] = {"load", "modelsave.reg"};
    char *savecmd[] = {"save", "save.reg"};

    char buffer[80];
    save_register(&m, "modelsave.reg");
    process_load(2, loadcmd, &s);
    print_students(&s);
    test = compare_regs(&m, &s, infostr);
    if (!test) {
        release_memory(&m);
        release_memory(&s);
        showInput(buffer, loadcmd, 2);
        fail("[Task 5.4.e] Invalid registry contents after command '%s': %s", buffer, infostr);
    }
    process_save(2, savecmd, &s);

    mfile = fopen("modelsave.reg", "r");
    if (!mfile) {
	    release_memory(&m);
	    release_memory(&s);
	    fail("[Task 5.4.e] Created file using save_register(), but opening it failed. Have you implemented function save_register()?");
    }
    sfile = fopen("save.reg", "r");
    if (!sfile) {
	    release_memory(&m);
	    release_memory(&s);
	    showInput(buffer, savecmd, 2);
	    fail("[Task 5.4.e] Opening file failed after command '%s': have you implemented process_save()?", buffer);
    }

    test = compare_files(mfile, sfile, infostr);
    if (!test) {
        release_memory(&m);
        release_memory(&s);
	fclose(mfile);
	fclose(sfile);
        showInput(buffer, savecmd, 2);
        fail("[Task 5.4.e] Invalid file contents after '%s': %s", buffer, infostr);
    }
    fclose(mfile);
    fclose(sfile);
    release_memory(&m);
    release_memory(&s);
}
END_TEST

int main(int argc, const char *argv[])
{
    	Suite *s = suite_create("Test-5.4");

        tmc_register_test(s, test_parse_cmdline, "5.4.a");
        tmc_register_test(s, test_process, "5.4.b");
        tmc_register_test(s, test_create_find_delete, "5.4.c");
        tmc_register_test(s, test_points, "5.4.d");
        tmc_register_test(s, test_load_save, "5.4.e");
        
	return tmc_run_tests(argc, argv, s);
}
