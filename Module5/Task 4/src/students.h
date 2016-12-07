#ifndef  STUDENTS_H
#define STUDENTS_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct student Student;

struct student {
    char *name; // name of the student
    char id[8]; // null-terminated student ID
    unsigned char age;
    char course[16]; // null-terminated course code;
    float *points; // pointer to dynamic array of exercise points
    unsigned int numPoints; // length of the above array
    Student *next; // For implementing linked list
};

typedef struct {
    unsigned int count; // size if the students array
    Student *array; // pointer to the first element in the array
} Students;

struct commands {
    /* Define this structure so that it compiles with the command list in main.c */
	char * cmd;
	int (*func)(int, char **, Students *);
};

/* defined in students.c -- from Exercise 3.4 */
Student *create_student(Students *s, const char *name, const char *id, unsigned char age, const char *course);
Student *find_student(Students *s, const char *id, const char *course);
int delete_student(Students *s, const char *id, const char *course);
int set_points(Students *s, const char *id, const char *course, const float *points, int len);

/* defined in loader.c -- from Exercise 5.1 */
int load_register(Students *reg, const char *filename);
int save_register(const Students *reg, const char *filename);

/* defined in main.c -- from Exercise 3.1 */
void print_student(Student *st);
void print_students(Students *s);

/* defined in cli.c -- this exercise */
int parse_cmdline(char ***argv, char *input);
void free_cmdline(char ***argv);
int process(struct commands *cl, int argc, char **argv, Students *s);

int process_create(int argc, char **argv, Students *s);
int process_find(int argc, char **argv, Students *s);
int process_delete(int argc, char **argv, Students *s);
int process_points(int argc, char **argv, Students *s);
int process_list(int argc, char **argv, Students *s);
int process_save(int argc, char **argv, Students *s);
int process_load(int argc, char **argv, Students *s);
int process_exit(int argc, char **argv, Students *s);

#endif