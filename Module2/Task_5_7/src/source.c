#include "source.h"
#include <string.h>

/* Exercise 2.5: Selection sort */
/* Parameters:
 * start: start of an array
 * size: length of an array
 */

void sort(int *start, int size)
{
  int i;
  int temp = 0;

  while(size > 0)
  {
    for(i = 1; i < size; i++)
    {
      if(*start > *(start + i))
      {
	temp = *start;
	*start = *(start + i);
	*(start+i) = temp;
      }
    }
         
    start++;
    size--;
  }
}

/* Exercise 2.6: Count Vowels */
/* Parameters:
 * str: string to be evaluated
 * returns: number of vowels */
 int count_vowels(char *str)
 {
    size_t count = 0, i = 0;
    
    for(i=0; i < strlen(str); i++)
    {
      if(str[i] == 'a' || str[i] == 'e' || str[i] == 'i' || str[i] =='o' || str[i] =='u' || str[i] =='y'
	|| str[i] =='A'|| str[i] =='E'|| str[i] =='I'|| str[i] =='O'|| str[i] =='U'|| str[i] =='Y')
	count++;
    }
    return count;
}
 
/* Exercise 2.7: Remove Comments */
/* Parameters:
 * out: place to write the resulting string
 * in: the original string */
void remove_comments(char * out, const char * in)
{
  int isComment = 0, newLine = 1;
  size_t i,j=0;
  
  for(i = 0; i < strlen(in); i++)
  {
    if(in[i] == '\n')
    {
      newLine = 1;
      if(isComment == 0)
      {
	out[j++]= in[i];
      }
      continue;
    }
    
    if(newLine == 1 && in[i] == '#')
    {
      isComment = 1;
      newLine = 0;
    }
    else if(newLine == 1 && in[i] != '#')
    {
      isComment = 0;
      newLine = 0;
    }
    
    if(isComment == 0)
    {
      out[j++]= in[i];
    }
  }
  
  out[j] = '\0';
}