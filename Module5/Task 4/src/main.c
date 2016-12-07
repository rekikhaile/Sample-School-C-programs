#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "students.h"

struct commands cmdlist[] = {
    { "create", process_create },
    { "find", process_find },
    { "delete", process_delete },
    { "points", process_points },
    { "list", process_list },
    { "load", process_load },
    { "exit", process_exit },
    { NULL, NULL }
};


void initialize_reg(Students *reg) {
    assert(reg != NULL);
    reg->count = 0;
    reg->array = NULL;
    create_student(reg, "TeemuTeekkari", "00000A", 20, "ELEC-A1100");
    create_student(reg, "MattiMeikäläinen", "12345B", 28, "ELEC-A1100");
    create_student(reg, "Wow", "33333C", 28, "ELEC-A1100");
    create_student(reg, "MuchStudent", "98765H", 28, "ELEC-A1100");
    create_student(reg, "SuchCourse", "12121R", 28, "ELEC-A1111");
    create_student(reg, "Amaze", "11111T", 28, "ELEC-A1111");
}

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

int main()
{
    Students s;
    s.count = 0;
    s.array = NULL;
    char buffer[80];
    char **argv = NULL;
    
    while (1) {
        printf("> ");
        fgets(buffer, sizeof(buffer), stdin);
        int argc = parse_cmdline(&argv, buffer);
        process(cmdlist, argc, argv, &s);
        free_cmdline(&argv);
    }
}
