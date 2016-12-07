#include <stdio.h>
#include "source.h"

/* Exercise 1.1 */
void three_lines()
{
  printf("One\nTwo\nThree\n");
}


/* Exercise 1.2 */
void data_types()
{
   /* char val_a = 200 + 200;  // should be 400, right?
    unsigned int sign = 1 - 2;  // Hmm... I'd assume this to be -1
    float val_b = 5 * sign; // so this must be -5, right?
    int val_c = 2.5;
    long val_d = 100 / val_c;  // 100 / 2.5 = 40, I suppose */
    
    short val_a = 200 + 200;  // should be 400, right?
    short sign = 1 - 2;  // Hmm... I'd assume this to be -1
    float val_b = 5 * sign; // so this must be -5, right?
    float val_c = 2.5;
    long val_d = 100 / val_c;  // 100 / 2.5 = 40, I suppose
    
    /* you can assume that the following line is correct */
    printf("%d %.1f %ld\n", val_a, val_b, val_d);
}

/* Exercise 1.3 */
void simple_sum()
{
  int a, b ;
  /*printf("enter two integers\n")*/
  if(scanf("%d %d", &a, &b)==2)
    printf("%d\n", a + b);
  else
    printf("ERROR\n");
}

/* Exercise 1.4 */
void simple_math()
{
  int a,b;
  char c;
  if(scanf("%d %c %d", &a, &c, &b) == 3)
  {
    switch(c) {
    case '+':
      printf("%d\n", a + b);
      break;
    case '-':
      printf("%d\n", a - b);
      break;
    case '*':
      printf("%d\n", a * b);
      break;
    case '/':
      printf("%d\n", a / b);
      break;
    default:
      printf("ERROR\n");
      break;
    }
  }
  else
    printf("ERROR\n");
}
