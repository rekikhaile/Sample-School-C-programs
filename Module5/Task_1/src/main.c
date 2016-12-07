#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "students.h"


void initialize_reg(Students *reg) {
    assert(reg != NULL);
    reg->count = 0;
    reg->array = NULL;
    create_student(reg, "Teemu Teekkari", "00000A", 20, "ELEC-A1100");
    create_student(reg, "Matti Meikäläinen", "12345B", 28, "ELEC-A1100");
    create_student(reg, "Wow", "33333C", 28, "ELEC-A1100");
    create_student(reg, "Much Student", "98765H", 28, "ELEC-A1100");
    create_student(reg, "Such course", "12121R", 28, "ELEC-A1111");
    create_student(reg, "Amaze", "11111T", 28, "ELEC-A1111");
}

void free_array(Students *reg)
{
    unsigned int i;
    if (reg->array) {
        for (i = 0; i < reg->count; i++) {
            Student *st = &(reg->array[i]);
            if (st->name)
                free(st->name);
            if (st->points)
                free(st->points);
        }
        free(reg->array);
    }
}

// Print students currently in register
void print_students(Students *s)
{
    Student *st = s->array;
    for (unsigned int i = 0; i < s->count; i++) {
        printf("%s (%s), Course: %s, Age: %d\n", st->name, st->id, st->course, st->age);
        if (st->numPoints) {
            printf(" -- Points: ");
            for (unsigned int j = 0; j < st->numPoints; j++)
                printf("%f  ", st->points[j]);
            printf("\n");
        } 
        st++;
    }
}


int main(void)
{
    Students s;
    
    // load a register from file
    int ret = load_register(&s, "register.bin");
    printf("load_register: %d\n", ret);
    print_students(&s);
    
    if (ret > 0)
        free_array(&s);
    
    // create a register and save it to a file
    initialize_reg(&s);

    float p[] = {1.0, 2.2, 5.4};
    set_points(&s, "00000A", "ELEC-A1100", p, 3);
    print_students(&s);
    
    ret = save_register(&s, "save.bin");
    printf("save_register: %d\n", ret);
}
