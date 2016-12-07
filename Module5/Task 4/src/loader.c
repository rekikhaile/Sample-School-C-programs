#include "students.h"


int load_register(Students *reg, const char *filename)
{
	FILE * fp = fopen(filename, "r");
	
	if(fp == NULL)
	{
		return -1;
	}
	
	reg->count = 0;
	reg->array = NULL;
	
	int success = 1;
	int nameLen = 0;
	int age = 0;
	int pointCount = 0;
	int count = 0;
	char * name = NULL;
	char studentID[8];
	char courseCode[16];
	float * points = NULL;
	Student * s = NULL;
	
	while(!feof(fp))
	{
		if((nameLen = fgetc(fp)) == EOF)
		{
			if(ferror(fp))
			{
				success = 0;
			}
			
			break;
		}
		
		name = (char*)malloc(sizeof(char)*(nameLen + 1));
		name[nameLen] = '\0';
		
		if(fread(name, sizeof(char), nameLen, fp) != nameLen)
		{
			success = 0;
			break;
		}
		
		memset(studentID, 0, sizeof(char) * 8);
		
		if(fread(studentID, sizeof(char), 8, fp) != 8)
		{
			success = 0;
			break;
		}
		
		if((age = fgetc(fp)) == EOF)
		{
			success = 0;
			break;
		}
		
		memset(courseCode, 0, sizeof(char) * 16);
		
		if(fread(courseCode, sizeof(char), 16, fp) != 16)
		{
			success = 0;
			break;
		}
		
		if((pointCount = fgetc(fp)) == EOF)
		{
			success = 0;
			break;
		}
		
		points = (float *)malloc(sizeof(float) * pointCount);
		
		if(fread(points, sizeof(float), pointCount, fp) != pointCount)
		{
			success = 0;
			break;
		}
		
		s = create_student(reg, name, studentID, age, courseCode);
		
		if(set_points(reg, studentID, courseCode, points, pointCount) != 1)
		{
			success = 0;
			break;
		}
		
		s->numPoints = pointCount;
		
		count++;
		
		free(name);
		free(points);
	}
	
	fclose(fp);
	
	if(!success)
	{
		return -1;
	}
	else
	{
		return count;
	}
}

int save_register(const Students *reg, const char *filename)
{
	FILE * f = fopen(filename, "w");
	
	if(f == NULL)
	{
		return -1;
	}
	
	int nameLen = 0;
	int success = 1;
	int count = 0;
	
	for(int i = 0; i < reg->count; i++)
	{
		nameLen = strlen(reg->array[i].name);
		
		if(fputc(nameLen, f) == EOF)
		{
			success = 0;
			break;
		}
		
		if(fwrite(reg->array[i].name, sizeof(char), nameLen, f) != nameLen)
		{
			success = 0;
			break;
		}
		
		if(fwrite(reg->array[i].id, sizeof(char), strlen(reg->array[i].id), f) != strlen(reg->array[i].id))
		{
			success = 0;
			break;
		}
		
		for(int j = 0; j < 8 - strlen(reg->array[i].id); j++)
		{
			if(fputc(0, f) == EOF)
			{
				success = 0;
				break;
			}
		}
		
		if(success == 0)
		{
			break;
		}
		
		if(fputc(reg->array[i].age, f) == EOF)
		{
			success = 0;
			break;
		}
		
		if(fwrite(reg->array[i].course, sizeof(char), strlen(reg->array[i].course), f) != strlen(reg->array[i].course))
		{
			success = 0;
			break;
		}
		
		for(int j = 0; j < 16 - strlen(reg->array[i].course); j++)
		{
			if(fputc(0, f) == EOF)
			{
				success = 0;
				break;
			}
		}
		
		if(success == 0)
		{
			break;
		}
		
		if(fputc(reg->array[i].numPoints, f) == EOF)
		{
			success = 0;
			break;
		}
		
		if(fwrite(reg->array[i].points, sizeof(float), reg->array[i].numPoints, f) != reg->array[i].numPoints)
		{
			success = 0;
			break;
		}
		
		count++;
	}
	
	fclose(f);
	
	if(!success)
	{
		return -1;
	}
	else
	{
		return count;
	}
}