#include <check.h>
#include "tmc-check.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <ctype.h>
#include "../src/students.h"

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

int initialize_reg(Students *reg) {
    assert(reg != NULL);
    int ret = 6;
    reg->count = 0;
    reg->array = NULL;
    create_student(reg, "Teemu Teekkari", "00000A", 20, "ELEC-A1100");
    create_student(reg, "Matti Meikäläinen", "12345B", 28, "ELEC-A1100");
    create_student(reg, "Wow", "33333C", 28, "ELEC-A1100");
    create_student(reg, "Much Student", "98765H", 28, "ELEC-A1100");
    create_student(reg, "Such course", "12121R", 28, "ELEC-A1111");
    create_student(reg, "Amaze", "11111T", 28, "ELEC-A1111");
    float p[] = {1.0, 2.2, 5.4};
    set_points(reg, "00000A", "ELEC-A1100", p, 3);
    Student *s = &reg->array[0];
    s->numPoints = 3;  // original 3.4 test had forgot to check this
    return ret;
}

int initialize_reg_easy(Students *reg) {
    assert(reg != NULL);
    int ret = 1;
    reg->count = 0;
    reg->array = NULL;
    create_student(reg, "Teemu Teekkari", "00000A", 20, "ELEC-A1100");
    return ret;
}



void printPoints(char *buffer, Student *s) {
    buffer[0] = 0;
    char buf[16];
    for (unsigned int i = 0; i < s->numPoints; i++) {
        sprintf(buf, "%f", s->points[i]);
        strcat(buffer, buf);
        if (i < s->numPoints - 1)
            strcat(buffer, ",");
    }
}

int compare_regs(Students *model, Students *student, char *info) {
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

void hexcmp(unsigned char *data, unsigned char *ref, size_t ndata,
        char *outbuf) {
    int diffoff = 0;
    unsigned char *origdata = data;
    unsigned char *origref = ref;
    while (*data == *ref) {
        diffoff++;
        data++;
        ref++;
    }

    data = origdata;
    ref = origref;

    int outoff = diffoff - 16;
    if (outoff < 0)
        outoff = 0;

    sprintf(outbuf, "(Starting output at byte offset %d, first difference at byte %d)\n",
            outoff, diffoff);
    int lastline = outoff;
    int i = outoff;
    char buf[8];
    do {
        if (data[i] != ref[i])
            strcat(outbuf, ">");
        else
            strcat(outbuf, " ");
        sprintf(buf, "%02x", data[i]);
        strcat(outbuf, buf);
#if 1
        if (((i - outoff) % 16) == 15 || i >= diffoff + 16 || i >= (int) ndata) {
            strcat(outbuf, "  |");
            for (int j = lastline; j <= i; j++) {
                    if (isgraph(data[j]))
                        sprintf(buf, "%c", data[j]);
                    else
                        sprintf(buf, ".");
                    strcat(outbuf, buf);
            }
            strcat(outbuf, "|\n");
            lastline = i+1;
        }
#endif
        i++;
    } while (i < diffoff + 16 && i < (int) ndata);

#if 1
    if (lastline != i) {
        strcat(outbuf, "  |");
        for (int j = lastline; j < i; j++) {
            if (isgraph(data[j]))
                sprintf(buf, "%c", data[j]);
            else
                sprintf(buf, ".");
            strcat(outbuf, buf);
        }
        strcat(outbuf, "|\n");
    }
#endif

    if (diffoff + 16 < (int) ndata) {
        strcat(outbuf, "(File continues...)\n");
    }
}

int compare_files(FILE *model, FILE *student, char *info) {
    unsigned char buf_m[400];
    unsigned char buf_s[400];
    memset(buf_m, 0, sizeof (buf_m));
    memset(buf_s, 0, sizeof (buf_s));

    info[0] = 0;

    size_t read_m = fread(buf_m, 1, 400, model);
    size_t read_s = fread(buf_s, 1, 400, student);
    if (read_s != read_m || memcmp(buf_m, buf_s, read_s)) {
        sprintf(info, "Your file (length: %lu, should be: %lu):\n", read_s, read_m);
        hexcmp(buf_s, buf_m, read_s, info + strlen(info));
        sprintf(info + strlen(info), "Reference file:\n");
        hexcmp(buf_m, buf_s, read_m, info + strlen(info));
        return 0;
    }
    return 1;
}

START_TEST(test_load) {
    Students model;
    Students student;
    student.array = NULL;
    student.count = 0;
    char info[100];
    int modelret, ret, test;
    if (load_register(&student, "nonexistent.bin") != -1) {
        fail("[Task 5.1.a] Tried to load nonexistent file, should have returned -1.");
    }
    modelret = initialize_reg(&model);
    ret = load_register(&student, "register.bin");
    if (ret != modelret) {
        release_memory(&student);
        release_memory(&model);
        fail("[Task 5.1.a] Load returned %d, should have returned %d",
                ret, modelret);
    }
    test = compare_regs(&model, &student, info);
    if (!test) {
        release_memory(&model);
        release_memory(&student);
        fail("[Task 5.1.a] %s", info);
    }
    release_memory(&model);
    release_memory(&student);

}

END_TEST

START_TEST(test_load_easy) {
    Students model;
    Students student;
    student.array = NULL;
    student.count = 0;
    char info[100];
    int modelret, ret, test;

    modelret = initialize_reg_easy(&model);
    ret = load_register(&student, "simple.bin");
    if (ret != modelret) {
        release_memory(&student);
        release_memory(&model);
        fail("[Task 5.1.a] Load returned %d, should have returned %d",
                ret, modelret);
    }
    test = compare_regs(&model, &student, info);
    if (!test) {
        release_memory(&model);
        release_memory(&student);
        fail("[Task 5.1.a] %s", info);
    }
    release_memory(&model);
    release_memory(&student);

}

END_TEST

START_TEST(test_save) {
    Students s;
    char info[800];
    int ret, modelret, test;
    modelret = initialize_reg(&s);
    ret = save_register(&s, "student.bin");
    release_memory(&s);
    if (ret != modelret) {
        fail("[Task 5.1.b] Save returned %d, should have returned number of records saved (%d)",
                ret, modelret);
    }
    FILE *sfile = fopen("student.bin", "r");
    if (!sfile) {
        fail("[Task 5.1.b] Failed to open student.bin");
    }
    FILE *mfile = fopen("register.bin", "r");
    test = compare_files(mfile, sfile, info);
    if (!test) {
        fclose(sfile);
        fclose(mfile);
        fail("[Task 5.1.b] %s", info);
    }
    fclose(sfile);
    fclose(mfile);

}

END_TEST

START_TEST(test_save_easy) {
    Students s;
    char info[800];
    int ret, modelret, test;
    modelret = initialize_reg_easy(&s);
    ret = save_register(&s, "student-simple.bin");
    release_memory(&s);

    if (ret != modelret) {
        fail("[Task 5.1.b] Save returned %d should have returned number of records saved (%d)",
                ret, modelret);
    }
    FILE *sfile = fopen("student-simple.bin", "r");
    if (!sfile) {
        fail("[Task 5.1.b] Failed to open student.bin");
    }
    FILE *mfile = fopen("simple.bin", "r");
    test = compare_files(mfile, sfile, info);
    if (!test) {
        fclose(sfile);
        fclose(mfile);
        fail("[Task 5.1.b] %s", info);
    }
    fclose(sfile);
    fclose(mfile);

}

END_TEST

int main(int argc, const char *argv[]) {
    Suite *s = suite_create("Test-5.1");

    /* TODO: define tests */
    tmc_register_test(s, test_load_easy, "5.1.a1");
    tmc_register_test(s, test_load, "5.1.a2");
    tmc_register_test(s, test_save_easy, "5.1.b1");
    tmc_register_test(s, test_save, "5.1.b2");

    return tmc_run_tests(argc, argv, s);
}
