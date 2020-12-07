#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "libft.h"

#define	MAXBUF 200

typedef struct	ds
{
	char	base[15];
	char	adj[15];
	int		csgd;		//Contains Shiny Gold (directly) 0 or 1.
	int		empty;		//Equal to 1 for empty bag
	int		numbags;	//Number of bags in total per color
	struct	color	*nextcolors;
}			color;

void	storedata(char *file, int lines, struct ds *groups);
int		getnumberoflines(char *file, int *ptparts);

int		main(void)
{
	//char	*file = "input.txt";
	char	*file = "example.txt";
	FILE	*in_file;

	printf("= CHECKS AND BALANCES ========================================\n");
	//Step 1: Getting number of lines
	int		lines;
	int		parts;
	int		*ptparts;
	ptparts = &parts;
	
	lines = getnumberoflines(file, ptparts);

	//Checks tbd
	// 1 - does every line has one 'rule', e.g. not "a contain b contain c"? YES
	// 2 - does every line represent a unique color, e.g. not two lines "a contain DEF" & "a contain IJK"? YES
	// 3 - are there similar colors, like "blue" and "pale blue"? NOT APPLICABLE SINCE EXTENSIVE CHECK 2.
	printf("Number of lines: %i\n", lines);
	printf("Number of 'contains': %i\n", parts);

	printf("= ALLOC MEMORY & READING DATA ================================\n");
	//Step 2: Allocating arrays
	color	*colorlines;

	printf("Allocating data array...\n");
	colorlines = calloc(lines, sizeof(color));
	if (colorlines == NULL)
		printf("Allocation for data array failed");

	//Step 3: Reading file per MAXBUF chars
	printf("Start reading input...\n");
	storedata(file, lines, colorlines);
	
	printf("= CHECKS EXTRA ===============================================\n");
	int	i = 0, j = 0;
	int	db = 0;

	while (i < lines && db == 0)
	{
		j = i + 1;
		while (j < lines && db == 0)
		{
			if (	!strcmp(colorlines[i].adj, colorlines[j].adj) &&\
					!strcmp(colorlines[i].base, colorlines[j].base)	)
				db = 1;
			j++;
		}
		i++;
	}
	printf("db = %i\n", db);

	printf("= PRINT TEST DATA ============================================\n");
	int	lastprint = 9;
	
	//Validate if reading data is succes
	i = 0;
	while (i <= lastprint)
	{
		printf("READ TEST colorline %-3i\t ", i);
		printf("adj=%s\t", colorlines[i].adj);
		printf("base=%s\t", colorlines[i].base);
		printf("csgd=%i ", colorlines[i].csgd);
		printf("empty=%i\n", colorlines[i].empty);
		i++;
	}

	printf("= PT1 ANALYSIS ===============================================\n");
	//Calc number of questions with any person "y", per group
	i = 0;
	while (i < lines)
	{
		//CheckGold colorline(i)
		//	- this fun can see "empty" - DONE / return 0;
		//  - this fun can see "csg" - DONE / return 1;
		//	- this fun can see sth else - CALL AGAIN
		i++;
	}
/*
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
*/
	printf("= END =======================================================\n");
	free(colorlines);
	return (0);
}

int		containshinygold(color *checkcolor)
{
	if (checkcolor->empty == 1)
		return 0;
	else if (checkcolor->csgd == 1)
		return 1;
	return 2; //Check next color, but this is a list of different length :(
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

void		storedata(char *file, int lines, struct ds *dat)
{
	FILE	*in_file;
	char	line[MAXBUF];
	char	*pt, *end;
	int		i;

	in_file = fopen(file, "r");
	if (in_file == NULL)
		printf("File read failed\n");
	
	i = 0;
	while (fgets(line, MAXBUF, in_file) != NULL)
	{
		//printf("reading: %s", line);
		pt = line;
		end = strchr(pt, ' ');
		strncpy(dat[i].adj, pt, end - pt);
		pt = end + 1;
		end = strchr(pt, ' ');
		strncpy(dat[i].base, pt, end - pt);
		pt = strstr(pt, "contain") + 7;
		if (strstr(pt, "no other"))
			dat[i].empty = 1;
		else if (strstr(pt, "shiny gold"))
			dat[i].csgd = 1;
		//else // != no other
		//while - iterate colors

		//ft_lst_add_back(dat[i].nextcolors, ft_lstnew(
		i++;
	}
	fclose(in_file);
}
