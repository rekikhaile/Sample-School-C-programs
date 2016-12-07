#include <stdio.h>
#include "diagram.h"
#include <string.h>

/* Exercise 6.1: Draw a diagram based on given arrays
 * 
 * Parameters:
 * labels: array of strings, ends with NULL as last element
 * values: array of float numbers. Same length as the labels array
 */
void diagram(const char **labels, float *values)
{
	int i = 0, j = 0;
	float maxValue = 0;
	
	while(values[i]!= 0)
	{
		if(values[i] > maxValue)
			 maxValue = values[i];
		i++;
	}
	
	while(labels[j] != NULL)
	{
		int barLength = (int)(values[j]/maxValue * 20);
		printf("%s", labels[j]);
		for(size_t k = 0; k < 20 - strlen(labels[j]); k++)
			printf(" ");
		printf(" ");
		for(int k = 0; k < barLength; k++)
			printf("#");
		for(int k = 0; k < 20 - barLength; k++)
			printf(" ");
		printf(" %lf", values[j]);
		printf("\n");
		j++;
	}
}
