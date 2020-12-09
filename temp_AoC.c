#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define	MAXBUF 200
 
typedef struct	ds
{
	char	str[MAXBUF];		//Need to make this variable, useful for many applications?
	int		num;
}			dat;

void	getdata(char *file, int lines, struct ds *groups);
int		getnumberoflines(char *file, int *ptparts);

int		main(int argc, char **argv)
{
	int		ft = 0;
	char	*file0 = "input.txt";
	char	*file1 = "example.txt";
	char	*file;

	if (argc > 1)
		ft = atoi(argv[1]);
	if (ft == 1)
		file = file1;
	else
		file = file0;

	printf("= INPUT CHECK ================================================\n");
	int		lines;
	int		*ptparts;
	int		parts;			//Parts can be different groups of lines separated by \n or sth.
	
	ptparts = &parts;
	lines = getnumberoflines(file, ptparts);

	printf("Number of lines: %i\n", lines);
	printf("Number of 'parts': %i\n", parts);

	printf("= ALLOC MEMORY & READING DATA ================================\n");

	printf("Allocating data array...\n");
	dat	*data;
	data = calloc(lines, sizeof(dat));
	if (data == NULL)
		printf("Allocation for data array failed");
		
	printf("Start reading input...\n");
	getdata(file, lines, data);
	
	printf("= CHECKS EXTRA ===============================================\n");
	printf("= PRINT TEST DATA ============================================\n");
	int	lastprint = 5;
	int i = 0;

	if (argc > 2)
		lastprint = atoi(argv[2]);
	
	while (i < lastprint)
	{
		printf("line %i = %s\n", i, data[i].str);
		i++;
	}

	printf("= PT1 ANALYSIS ===============================================\n");
	int		a1 = 0;
	
	printf("Answer part 1 = %i\n", a1);
		
	printf("= PT2 ANALYSIS ===============================================\n");
	long long		a2 = 0;

	printf("Answer part 2 = %lli\n", a2);
	
	//Freeing data
	free(data);
	printf("= END =======================================================\n");
	return (0);
}

/*
** Functions 'test reads' the data and returns number of lines
** + stores number of 'partsi/files' (for example sep. by newline) to the pointers
*/

int		getnumberoflines(char *file, int *ptparts)
{
	FILE	*in_file = fopen(file, "r");
	
	if (in_file == NULL)
		printf("File read failed\n");
	
	char	line[MAXBUF];
	char	*lpt;
	int		lines = 0;
	int		ppfiles = 0;
	
	while (fgets(line, sizeof(line), in_file) != NULL)
	{
		lines++;
		lpt = line;
		while (strstr(lpt, "contain"))			//adjust this condition to count 'parts'
		{
			ppfiles++;
			lpt = strstr(lpt, "contain") + 1;
		}
	}
	*ptparts = ppfiles;
	fclose(in_file);

	return (lines);
}

void		getdata(char *file, int lines, struct ds *dat)
{
	FILE	*in_file;
	char	str[MAXBUF];
	int		line;
	char	*pt;

	in_file = fopen(file, "r");
	if (in_file == NULL)
		printf("File read failed\n");
	
	line = 0;
	while (line < lines && fgets(str, MAXBUF, in_file) != NULL)
	{
		//printf("reading: %s", str);
		pt = str;
		strncpy(dat[line].str, str, strchr(str, '\n') - str);
		dat[line].num = atoi(str);
		line++;
	}
	fclose(in_file);
}
