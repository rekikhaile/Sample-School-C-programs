#include "students.h"
#include "students.h"


/* Parses a string into tokens (command line parameters) separated by space
 * Builds a dynamically allocated array of strings. Pointer to the array is
 * stored in variable pointed by argv.
 * 
 * Parameters:
 * argv: pointer to the variable that will store the string array
 * input: the string to be parsed (the original string can be modified, if needed)
 * 
 * Returns:
 * number of space-separated tokens in the string */
int parse_cmdline(char ***argv, char *input)
{
	size_t count = 0;
	*argv = NULL;
	
	char * temp = strtok(input, " ");
	
	if(temp != NULL)
	{
		*argv = (char **)malloc(sizeof(char*));
		(*argv)[0] = temp;
		count = 1;
	}
	
	while(temp != NULL)
	{
		count++;
		temp = strtok(NULL, " ");
		*argv = (char **)realloc(*argv, sizeof(char *) * count);
		(*argv)[count - 1] = temp;
	}
	
 	return --count;
}

/* Release the memory allocated by parse_cmdline.
 * 
 * Parameters:
 * argv: pointer to the string array with command line parameters. After
 * releasing memory, the string array should be set to NULL. */
void free_cmdline(char ***argv)
{


	free(*argv);

	*argv = NULL;
}

/* Process the command in string array produced by parse_cmdline, using
 * the command list given. When a matching command is found, the function
 * given in the list is called to process the command.
 * 
 * Parameters:
 * cl: array of commands and functions to process the command
 * argc: Number of arguments (length of string array)
 * argv: Arguments as an array of strings
 * s: The Students database. The calling function allocates Students structure
 * 
 * Returns:
 * -1 if parsing command failed, 0 if parsing was succesful */
int process(struct commands *cl, int argc, char **argv, Students *s)
{
	int index = 0;
	int found = 0;
	
	while(cl[index].cmd != NULL)
	{
		if(strcmp(argv[0], cl[index].cmd) == 0)
		{
			found = 1;
			break;
		}
	
		index++;
	}
	
	if(!found)
	{
		return -1;
	}
	else
	{
		return cl[index].func(argc, argv, s);
	}
}

int process_create(int argc, char **argv, Students *s)
{
	if(argc != 5)
	{
		return -1;
	}
	
	int age = -1;
	
	if(isdigit(argv[3][0]))
	{
		age = (unsigned char)atoi(argv[3]);
	}
	else
	{
		return -1;
	}
	
	create_student(s, argv[1], argv[2], (unsigned char)age, argv[4]);
	
	return 0;
}

int process_find(int argc, char **argv, Students *s)
{
	if(argc != 3)
	{
		return -1;
	}
	
	Student * student = find_student(s, argv[1], argv[2]);
	
	if(student != NULL)
	{
		print_student(student);
	}
	
	return 0;
}

int process_delete(int argc, char **argv, Students *s)
{
	if(argc != 3)
	{
		return -1;
	}
	
	int result = delete_student(s, argv[1], argv[2]);
	
	printf("%d\n", result);
	
	return 0;
}

int process_points(int argc, char **argv, Students *s)
{
	if(argc < 4)
	{
		return -1;
	}
	
	float points[argc - 3];
	
	for(int i = 3; i < argc; i++)
	{
		points[i - 3] = (float)atof(argv[i]);
	}
	
	if(set_points(s, argv[1], argv[2], points, argc - 3) == -1)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

int process_list(int argc, char **argv, Students *s)
{
	print_students(s);
	
	return 0;
}

int process_load(int argc, char **argv, Students *s)
{
	if(argc != 2)
	{
		return -1;
	}
	
	if(load_register(s, argv[1]) != -1)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int process_save(int argc, char **argv, Students *s)
{
	if(argc != 2)
	{
		return -1;
	}
	
	if(save_register(s, argv[1]) != -1)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int process_exit(int argc, char **argv, Students *s)
{
}
