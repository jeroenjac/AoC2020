#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAXL 30
#define	MAXBUF 200

typedef struct	ds
{
	int		q_any[26]; // 1 when answered yes 1x, otherwise 0;
	int		q_all[26]; // 1 when answered by all group mems, otherwise 0;
}			group;

void	storedata(char *file, int lines, struct ds *groups);
int		getnumberoflines(char *file, int *ptparts);
void	checkqs(char *ptl, group *gr);
void	initqall(group *gr);

int		main(void)
{
	char	*file = "input.txt";
	FILE	*in_file;

	printf("= CHECKS AND BALANCES ========================================\n");
	//Step 1: Getting number of lines
	int		lines;
	int		parts;
	int		*ptparts;
	ptparts = &parts;
	
	lines = getnumberoflines(file, ptparts);

	printf("Number of lines: %i\n", lines);
	printf("Number of parts: %i\n", parts);

	printf("= ALLOC MEMORY & READING DATA ================================\n");
	//Step 2: Allocating arrays
	group	*groups;

	printf("Allocating data array...\n");
	groups = calloc(parts, sizeof(group));
	if (groups == NULL)
		printf("Allocation for data array failed");

	//Step 3: Reading file per MAXBUF chars
	printf("Start reading input...\n");
	storedata(file, lines, groups);


	printf("= PRINT TEST DATA ============================================\n");
	int	i = 0, q = 0;
	int	last = 4;
	//Validate if reading data is succes

	while (i <= last)
	{
		printf("READ TEST (a-z by any): group %-3i = ", i);
		q = 0;
		while (q < 26)
		{
			printf("%i", groups[i].q_any[q]);
			q++;
		}
		printf("\n");
		printf("READ TEST (a-z by all): group %-3i = ", i);
		q = 0;
		while (q < 26)
		{
			printf("%i", groups[i].q_all[q]);
			q++;
		}
		printf("\n");
		i++;
	}

	printf("= PT1 ANALYSIS ===============================================\n");
	//Calc number of questions with any person "y", per group
	long	num_q_min1x_yes = 0;
	i = 0;
	while (i < parts)
	{
		q = 0;
		while (q < 26)
		{
			num_q_min1x_yes += groups[i].q_any[q];
			q++;
		}
		i++;
	}
	
	printf("Answer part 1 = %li\n", num_q_min1x_yes);
	
	printf("= PT2 ANALYSIS ===============================================\n");
	//Calc number of questions all "y", per group
	long	num_q_all_yes = 0;
	i = 0;
	while (i < parts)
	{
		q = 0;
		while (q < 26)
		{
			num_q_all_yes += groups[i].q_all[q];
			q++;
		}
		i++;
	}
	
	printf("Answer part 2 = %li\n", num_q_all_yes);

	printf("= END =======================================================\n");
	free(groups);
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
	int		lines = 0;
	int		ppfiles = 1;
	int		i;
	while (fgets(line, sizeof(line), in_file) != NULL)
	{
		lines++;
		if (*line == '\n')
			ppfiles++;
	}
	*ptparts = ppfiles;
	fclose(in_file);

	return (lines);
}

void		storedata(char *file, int lines, struct ds *dat)
{
	FILE	*in_file;
	char	line[MAXBUF];
	char	*pt;
	int		i, qnum;

	in_file = fopen(file, "r");
	if (in_file == NULL)
		printf("File read failed\n");
	
	i = 0;
	initqall(&dat[i]);
	qnum = -1;
	while (fgets(line, MAXBUF, in_file) != NULL)
	{
		//printf("reading: %s", line);
		if (*line == '\n')
		{
			i++;
			initqall(&dat[i]);
		}
		else
		{
			pt = line;
			checkqs(pt, &dat[i]);
			while (*pt != '\n')
			{
				qnum = (int)(*pt - 'a');
				dat[i].q_any[qnum] = 1;
				pt++;
			}
		}
	}
	fclose(in_file);
}

void		checkqs(char *ptl, group *gr)
{
	char	q = 'a';
	
	while (*ptl != '\n')
	{
		while (q <= 'z')
		{
			if (gr->q_all[q - 'a'] == 1)
				gr->q_all[q - 'a'] = strchr(ptl, q) != NULL;
			q++;
		}
		ptl++;
	}
}

void		initqall(group *gr)
{
	char	q = 'a';
	
	while (q <= 'z')
	{
		gr->q_all[q - 'a'] = 1;
		q++;
	}
}
