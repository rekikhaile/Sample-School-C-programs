#include "students.h"

/* Exercise 3.4.a: Add student
 * Parameters:
 * s: pointer to the Students main structure (allocated by caller)
 * name: name of student
 * id: Student ID
 * age: age
 * course: course code
 * Returns: pointer to the student element in the array
 */
Student *create_student(Students *s, const char *name, const char *id,
        unsigned char age, const char *course)
{
	s->array = (Student *)realloc(s->array, sizeof(Student) * (++s->count));
	
	if(s->array == NULL)
	{
		return NULL;
	}
	
	s->array[s->count - 1].name = (char *)malloc(sizeof(char) * (strlen(name) + 1));
	memset(s->array[s->count - 1].name, 0, sizeof(char) * (strlen(name) + 1));
	strcpy(s->array[s->count - 1].name, name);
	memset(s->array[s->count - 1].id, 0, sizeof(char) * 8);
	strcpy(s->array[s->count - 1].id, id);
	s->array[s->count - 1].age = age;
	strcpy(s->array[s->count - 1].course, course);
	s->array[s->count - 1].points = NULL;
	s->array[s->count - 1].numPoints = 0;
	
    return &s->array[s->count - 1];
}

/* Exercise 3.4.b: Find Student
 * Parameters:
 * s: pointer to the Students main structure 
 * id: Student ID to be looked for
 * course: Course code to be looked for
 * Returns: pointer to the student element in array, if found. NULL if not found
 */
Student *find_student(Students *s, const char *id, const char *course)
{
	for(int i = 0; i < s->count; i++)
	{
		if(strcmp(s->array[i].course, course) == 0 && strcmp(s->array[i].id, id) == 0)
		{
			return &s->array[i];
		}
	}
	
    return NULL;
}

/* Exercise 3.4.c: Delete student
 * Parameters:
 * s: pointer to the Students main structure 
 * id: Student ID to be deleted
 * course: Course from which student is deleted
 * Returns: 1 if deletion was succesful, 0 if not (e.g. student not found)
 */
int delete_student(Students *s, const char *id, const char *course)
{
	int index = -1;
	Student * student = NULL;
	
	for(int i = 0; i < s->count; i++)
	{
		if(strcmp(s->array[i].course, course) == 0 && strcmp(s->array[i].id, id) == 0)
		{
			index = i;
			student = &s->array[i];
		}
	}
	
	if(student == NULL)
	{
		return 0;
	}
	
	free(student->name);
	free(student->points);
	
	memmove(student, student+1, sizeof(Student) * (s->count - index - 1));
	
	s->count--;
	
	return 1;
}

/* Exercise 3.4.d: Set points
 * Parameters:
 * s: pointer to the Students main structure
 * id: student ID to set the points
 * course: course ID to set the points
 * points: array of points to be set to the student (will replace previous entry)
 * len: length of the points array
 * Returns: 1 if setting points was succesful, 0 if not (e.g. student not found)
 */
int set_points(Students *s, const char *id, const char *course, const float *points, int len)
{
	Student * student = find_student(s, id, course);
	
	if(student == NULL)
	{
		return 0;
	}
	
	student->points = (float *)realloc(student->points, sizeof(float) * len);
	
	if(student->points == NULL)
	{
		return 0;
	}
	
	memcpy(student->points, points, sizeof(float) * len);
	
    return 1;
}
