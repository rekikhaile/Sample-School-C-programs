#include <stdio.h>
#include "diagram.h"


int main(void)
{
    const char *test_l[] = { "A", "B1", "B2", "C", NULL };
    float test_v[] = { 100, 80, 81, 50 };
    diagram(test_l, test_v);
    printf("----\n");

    const char *pop_l[] = { "China", "India", "Russia", "Germany", "Finland", NULL };
    float pop_v[] = { 1363840000, 1242700000, 146019512, 80716000, 5453784, 0 };
    diagram(pop_l, pop_v);
    
    printf("----\n");
    
    const char *weight_l[] = { "Hamster", "Rabbit", "Cat", "Guinea pig", NULL };
    float weight_v[] = { 0.12, 2.5, 3.3, 1.04, 0 };
    diagram(weight_l, weight_v);
}
