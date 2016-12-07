#include <stdio.h>
#include "calculator.h"

/* Exercise 1.7: Calculator */



/* Exercise 1.7.a */
char get_character()
{
	char c;
	
	if(scanf("%c", &c) == 0)
		return 0;
	else
	{
		while(c == ' ')
			scanf("%c", &c);

		return c;
	}
}

/* Exercise 1.7.b */
double parse_operation(double num)
{
	char c,n, result;
	float resultd;
	c = get_character();
	if(c == '\n')
	{
		return -10000;
	}
	else if(c == '(')
	{
		return -15000;
	}
	else if(c == ')')
	{
		return -16000;
	}
	else
	{
	    scanf(" %c", &n);

	    if(n > 47 && n < 58) 
	    {
		n = (double)n - 48;
		switch(c) {
		    case '+':
		    //printf("%lf\n", num + n);
		    return result = num + n;
				    
		    case '-':
		    return result = num - n;
				    
		    case '*':
		    return result = num * n;
				    
		    case '/':
		    return resultd = (float)num / n;
		    
		    case '(':
		      return -15000;
				    
		    default:
		    return -20000;
		}
	    }
	    else if(n == '(')
	    {
	      switch(c) {
		case '+':
		  return -15000;
				
		case '-':
		  return -15001;
				
		case '*':
		  return -15002;
				
		case '/':
		  return -15003;
		  
		default:
		  return -20000;
	      }
	    }
	    else
		return -20000;
	}
}


/* Exercise 1.7.c and 1.7.d */
double calculator()
{
    char c;
    double n, result;
    if(scanf("%c", &c) != 1)
    {
	return -20000;
    }
    else
    {
      n = (double)c - 48;
      
      if(c == '(')
      {
	if((result = calculator()) == -20000)
	{
	  return -20000;
	}
	else
	{
	  n = result;
	}
      }
      
      while((result = parse_operation(n)) != -10000)
      {
	  if(result == -20000)
	  {
	      return -20000;
	  }
	  else if(result == -15000)	// (
	  {
	      result = n + calculator();
	  }
	  else if(result == -15001)	// (
	  {
	      result = n - calculator();
	  }
	  else if(result == -15002)	// (
	  {
	      result = n * calculator();
	  }
	  else if(result == -15003)	// (
	  {
	      result = n / calculator();
	  }
	  else if(result == -16000)	// )
	  {
	      break;
	  }

	  n = result;
      }
      
      return n;
    }
}


 