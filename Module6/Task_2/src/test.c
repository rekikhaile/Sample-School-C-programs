#include "stringman.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>


int main(void)
{
    char test[][10] = { "testi", "sokos", "toimii", "12345678" };
    for (int i = 0; i < 4; i++) {
        reverse_string(test[i]);
        printf("%d: %s -- palindrome: %d\n", i, test[i], is_palindrome(test[i]));
		printf("%s\n", test[i]);
    }
    
        char testString[] = "A palindrome is a word,\n"
        "phrase, number, or other sequence of\n"
        "symbols or elements, whose meaning\n"
        "may be interpreted the same way in\n"
        "either forward or reverse direction\n";
    printf("1: %s", testString);
    reversed_words(testString);  
    printf("2: %s\n", testString);
	
	const char *testcount = "one two one three two four two";
    char result[20];
    result[0] = 0;
    int c = most_common(testcount, result, 20);
    printf("most common: %s (%d)\n", result, c);
    
} 
/* Exercise 6.2.a: reverse the string pointed by parameter <str>.
 * The string is modified in place: after function exits, <str> contains
 * the reversed string.
 */
void reverse_string(char *str)
{
	char * temp;
	temp = malloc(sizeof(char)*(strlen(str)+1));
	
	char * src;
	char * dest;
	
	dest = temp;
	src = str + strlen(str) - 1;
	
	for(size_t i = 0; i < strlen(str); i++)
	{
		*(dest++) = *(src--);
	}
	
	*dest = '\0';
	
	strcpy(str, temp);
	
	free(temp);
}

/* Exercise 6.2.b: Returns 1 if given string is palindrome, 0 if it is not
 */
int is_palindrome(const char *str)
{
	char * revStr = malloc(sizeof(char) * (strlen(str)+ 1));
	
	strcpy(revStr, str);
	reverse_string(revStr);
	
	if(strcmp(revStr, str) == 0)
		return 1;
	else
		return 0;
}

/* Exercise 6.2.c: Reverse all words in given string <str>. "Word" consists of
 * alphanumeric characters, and any non-alphanumeric character splits the
 * word.
 */
void reversed_words(char * str)
{
	int i = 0, count = 0, start = 0;
	
	char * newStr = malloc(sizeof(char)* (strlen(str) + 1));
	newStr[strlen(str)] = '\0';
	
	while(str[i] != '\0')
	{
		if(isalnum(str[i]))
		{
			while(isalnum(str[i]))
			{
				count++;
				i++;
			}
			
			char * oneWord = malloc(sizeof(char)*(count+1));
			strncpy(oneWord, &str[start], count);
			oneWord[count] = '\0';
			
			reverse_string(oneWord);
			
			for(int j = 0; j < count; j++)
			{
				newStr[start + j] = oneWord[j];
			}
			
			newStr[i] = str[i];
			
			start = i + 1;
			count = 0;
			
			free(oneWord);
		}
		else
		{
			// Non-word since does not begin with an alphanumeric character
			
			newStr[i] = str[i];
			start++;
		}
		
		i++;
	}
	
	strcpy(str, newStr);
	free(newStr);
}

/* Exercise 6.2.d: return the most common word in given string <str>. That word
 * is written to the buffer pointed by <result>. Size of the <result> buffer is
 * indicated in <size>. "Word" consists of alphanumeric characters, and any
 * non-alphanumeric character ends the word.
 * 
 * Returns: number of times the most common word was present in the string
 */
int most_common(const char *str, char *result, int size)
{
  unsigned int wordCount = 0;
  unsigned int letterCount = 0;
  unsigned int index = 0;
  unsigned int start = 0;
  char ** dict = NULL;
  unsigned int * counts = NULL;
  
  while(str[index] != '\0')
  {
    if(isalnum(str[index]))
    {
      // Picks up words
      while(isalnum(str[index]))
      {
	letterCount++;
	index++;
      }
      
      char * word = malloc(sizeof(char) * (letterCount + 1));
      strncpy(word, &str[start], letterCount);
      word[letterCount] = '\0';
      
      int found = 0;
      
      for(unsigned int i = 0; i < wordCount; i++)
      {
	if(strcmp(dict[i], word) == 0)
	{
	  counts[i]++;
	  found = 1;
	  break;
	}
      }
      
      if(!found)
      {
	wordCount++;
	dict = (char**)realloc(dict, sizeof(char *) * wordCount);
	counts = (unsigned int *)realloc(counts, sizeof(unsigned int *) * wordCount);
	
	dict[wordCount - 1] = (char *)malloc(sizeof(char) * (strlen(word) + 1));
	strcpy(dict[wordCount - 1], word);
	dict[wordCount - 1][strlen(word)] = '\0';
	
	counts[wordCount - 1] = 1;
      }
      
      free(word);
      
      if(str[index] == '\0')
      {
	break;
      }
      
      letterCount = 0;
      start = index + 1;
      index++;
    }
    else
    {
      if(str[index] == '\0')
      {
	break;
      }
      
      start++;
      index++;
    }
  }
  
  index = 0;
  
  unsigned int max = 0;
  
  for(unsigned int i = 0; i < wordCount; i++)
  {
    if(counts[i] > max)
    {
      max = counts[i];
      index = i;
    }
  }
  
  if(strlen(dict[index]) + 1 <= (unsigned int)size)
  {
    memset(result, 0, sizeof(char) * (strlen(dict[index]) + 1));
    strncpy(result, dict[index], size);
  }
  
  return counts[index];
}