#include <string.h>
#include <stdlib.h>

typedef struct student Student;

struct student {
    char *name; // name of the student
    char id[8]; // null-terminated student ID
    unsigned char age;
    char course[16]; // null-terminated course code;
    float *points; // pointer to dynamic array of exercise points
    unsigned int numPoints; // length of the above array
};

typedef struct {
    unsigned int count; // size if the students array
    Student *array; // pointer to the first element in the array
} Students;

/* defined in students.c -- from Exercise 3.4 */
Student *create_student(Students *s, const char *name, const char *id,
        unsigned char age, const char *course);
Student *find_student(Students *s, const char *id, const char *course);
int delete_student(Students *s, const char *id, const char *course);
int set_points(Students *s, const char *id, const char *course, const float *points, int len);

/* defined in loader.c */

/* Load student database from given file. The function should allocate all
 * needed memory for loading the student data
 * 
 * Parameters:
 * reg: the student register to be created. The function fill modify the
 *      the structure pointer by this parameter
 * filename: Name of the file form which the data is loaded
 * 
 * Returns:
 * Number of students loaded from file */
int load_register(Students *reg, const char *filename);

/* Save student database to the given file
 * 
 * Parameters:
 * reg: the student register to be written to file
 * filename: name of file to be created
 * 
 * Return:
 * Number of students saved. */
int save_register(const Students *reg, const char *filename);
