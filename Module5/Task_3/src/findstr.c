#include "findstr.h"

/* Searches for given string inside a file, starting from given offset
 * 
 * Parameters:
 * str: The string to be looked for
 * filename: name of the file to be searched
 * offset: starting point for the search, in bytes
 * 
 * Parameter:
 * The offset from which the string was first found */
long find_string(const char *str, const char *filename, long offset)
{
	FILE * f = fopen(filename, "r");
	
	if(f == NULL)
	{
		return -1;
	}
	
	int success = 1;
	long retVal = -1;
	size_t targetLength = strlen(str);
	
	for(long i = 0; i < offset; i++)
	{
		if(fgetc(f) == EOF)
		{
			success = 0;
			break;
		}
	}
	
	int c = 0;
	long current = 0;
	long start = -1;
	long relativeIndex = -1;
	
	while((c = fgetc(f)) != EOF)
	{
		if(c == str[0] && relativeIndex == -1)
		{
			relativeIndex = 1;
			start = current;
		}
		else if(relativeIndex == targetLength)
		{
			retVal = start;
			break;
		}
		else if(relativeIndex != -1 && c == str[relativeIndex])
		{
			relativeIndex++;
		}
		else
		{
			relativeIndex = -1;
		}
		
		current++;
	}
	
	fclose(f);
	
	if(!success)
	{
		return -1;
	}
	else if(retVal != -1)
	{
		return retVal + offset;
	}
	else
	{
		return -1;
	}
}
