#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define	MAXBUF 200
 
typedef struct	ds
{
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
	if (ft == 0)
		file = file0;
	else if (ft == 1)
		file = file1;

	printf("= INPUT CHECK ======= ========================================\n");
	int		lines;
	int		*ptparts;
	//int		parts;
	//ptparts = &parts;
	
	lines = getnumberoflines(file, ptparts);

	printf("Number of lines: %i\n", lines);

	printf("= ALLOC MEMORY & READING DATA ================================\n");
	//Step 2: Allocating arrays

	printf("Allocating data array...\n");
	dat	*numss;
	numss = calloc(lines, sizeof(dat));
	if (numss== NULL)
		printf("Allocation for data array failed");
		
	//Step 3: Reading file per MAXBUF chars
	printf("Start reading input...\n");
	getdata(file, lines, numss);
	
	printf("= CHECKS EXTRA ===============================================\n");
	printf("= PRINT TEST DATA ============================================\n");
	int	lastprint = 5;

	if (argc > 2)
		lastprint = atoi(argv[2]);
	
	int i = 0;
	while (i < lastprint)
	{
		printf("number %-3i = ", i);
		i++;
	}

	printf("= PT1 ANALYSIS ===============================================\n");

	int		bagswithgold = 0;
	printf("Answer part 1 = %i\n", bagswithgold);
		
	printf("= PT2 ANALYSIS ===============================================\n");
	
	long long		bagsingold = 0;

	printf("Answer part 2 = %lli\n", bagsingold);

	printf("= END =======================================================\n");
	free(colorlines);
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
		while (strstr(lpt, "contain"))
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
	char	line[MAXBUF];
	int		i

	in_file = fopen(file, "r");
	if (in_file == NULL)
		printf("File read failed\n");
	
	i = 0;
	while (fgets(line, MAXBUF, in_file) != NULL)
	{
		//printf("reading: %s", line);
		pt = line;
		numss[i].num = atoi(line);
		i++;
	}
	fclose(in_file);
}
