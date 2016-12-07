#include "filestats.h"


/* Returns the line count in given file
 * 
 * Parameters:
 * filename: name of the file to be investigated.
 * 
 * Returns:
 * The number of lines in file. */
int line_count(const char *filename)
{
	FILE * f = fopen(filename, "r");
	
	if(f == NULL)
	{
		return -1;
	}
	
	int c = 0;
	int count = 0;
	int charCount = 0;
	int empty = 1;
	
	while((c = fgetc(f)) != EOF)
	{
		if(c == '\n')
		{
			count++;
			empty = 1;
		}
		else
		{
			charCount++;
			empty = 0;
		}
	}
	
	if(charCount > 0 && !empty)
	{
		count++;
	}
	
	fclose(f);
	
	return count;
}

/* Count the number of words in the file. Word has to include at least one
 * alphabetic character, and words are separated by whitespace.
 * 
 * Parameters:
 * filename: name of the file to be investigated.
 * 
 * Returns:
 * number of words in the file */
int word_count(const char *filename)
{	
	FILE * f = fopen(filename, "r");
	
	if(f == NULL)
	{
		return -1;
	}
	
	int c = 0;
	int count = 0;
	int alphaFound = 0;
	
	while((c = fgetc(f)) != EOF)
	{
		if(isspace(c))
		{
			if(alphaFound)
			{
				count++;
			}
			
			alphaFound = 0;
		}
		else if(isalpha(c))
		{
			alphaFound = 1;
		}
	}
	
	if(alphaFound)
	{
		count++;
	}
	
	fclose(f);
	
	return count;
}

/* Returns the most common character in the file. Only alphabetic characters
 * are counted.
 * 
 * Parameters:
 * filename: name of the file to be investigated.
 * 
 * Returns: the most common character.*/
int most_common(const char *filename)
{
	int counts[128];
	memset(counts, 0, sizeof(int) * 128);
	
	FILE * f = fopen(filename, "r");
	
	if(f == NULL)
	{
		return -1;
	}
	
	int c = 0;
	
	while((c = fgetc(f)) != EOF)
	{
		if(isalpha(c) && c < 128)
		{
			counts[c]++;
		}
	}
	
	fclose(f);
	
	int maxValue = -1;
	int maxIndex = -1;
	
	for(int i = 0; i < 128; i++)
	{
		if(counts[i] > maxValue)
		{
			maxValue = counts[i];
			maxIndex = i;
		}
	}
	
	return maxIndex;
}
