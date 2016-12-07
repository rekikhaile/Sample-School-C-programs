#include <stdio.h>
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


int main()
{
    /* Testing 2.5 Selection Sort. Implement a function to print
     * the resulting array to see that it really works */ 
    int arr[] = {3, 4, 7, 2, 8};
    int i;
    
    sort(arr, 5);
    
    for(i = 0; i < 5; i++)
    {
      printf("%d ", arr[i]);
    }
    
    printf("\n");
 
    /* Testing 2.6 Count Vowels */
    char *str = "How many vowels in this string?";
    printf("%d\n", count_vowels(str));
    
        /* Testing 2.7: Remove Comments */
    /* fopen will be introduced later. It opens a file, used in testing this task
     * Just trust (or hope) that the line works */
    FILE *f;
   // f = fopen("../test/hidden_test.txt", "r");
    f = fopen("../test/testfile.conf", "r");
    if (!f) {
        f = fopen("test/hidden_test.txt", "r");
    }

    if (f) {
        char outbuf[2000], inbuf[2000];
        int n = fread(inbuf, 1, sizeof(inbuf), f);
        if (n > 0) {
            inbuf[n] = 0;
            remove_comments(outbuf, inbuf);
            printf("Output:\n%s\n", outbuf);
        }
        
    } else {
        printf("opening file failed\n");
    }
    
    
    return 0;
}
