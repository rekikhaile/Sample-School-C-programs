#include <stdio.h>
#include "source.h"


/* Exercise 1.5 */
void multi_table()
{
  int a, b, i, j;
  scanf("%d %d", &a, &b);
  if(a <= 20 && b <= 20)
  {
  for(i = 1; i <= b; i++)
  {
    for(j = 1; j <= a; j++)
    {
      if(j == 1 && i != 1)
	printf("%4d", i);
      if (i == 1)
      printf("%4d", j);
      if(i != 1 && j != 1)
      {
      printf("%4d",i* j);
      }

    }
    printf("\n");
  }
  }
  
  else
        printf("ERROR\n");
  
}


/* Exercise 1.6 */
void hexdump(FILE *f)
{
  char c;
  int count=0;
  while((c=fgetc(f)) != -1)
    {
     count++;
      if(count % 16 != 0)
	printf("%02x ",c);
      else
      {
	printf("%02x ",c);
	printf("\n");
      }
	
      }
    
}
