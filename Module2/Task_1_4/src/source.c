#include <stdio.h>
#include "source.h"

/* Exercise 2.1: Pointer Sum */
/* Parameters:
 * a: modified by this function to contain sum of values behind a and b
 * b: not modified by this function
 */
void pointer_sum(int *a, int *b)
{
  *a = *a + *b;
}

/* Exercise 2.2: Extended Pointer Sum */
/* Parameters:
 * a: modified by this function to contain sum of values behind a and b
 * b: array of numbers that are each added to 'a'
 * count: how many elements are in array
 */
void pointer_sum2(int *a, int *b, int count)
{
  while(count > 0)
  {
    *a = *a + *b;
    b++;
    count--;
  }
}

/* Exercise 2.3: Array Reader */
/* Parameters:
 * array: array to be filled 
 * size: maximum size of array
 * returns: number of values read */
int array_reader(int *array, int size)
{
  int count = 0;
  
  while(size > 0)
  {
    if(scanf("%d", array) != 1)
    {
      break;
    }
    
    array++;
    size--;
    count++;
  }
  
  return count;
}

/* Exercise 2.4: Magic Number */
/* Parameters: 
 * start: start of an array
 * magic: number to be searched
 * returns: number of elements before magic number */
int magic_number(int *start, int magic)
{
  int count = 0;
  
  for(; *start!=magic; start++)
  {
    count++;
  }
  
  return count;  // placeholder, replace with actual code
}