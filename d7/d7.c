#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define	MAXBUF 200
 
typedef struct	ds
{
	char	col[40];
	char	cont[200];		//'Colors' seperated by |
	int		bagIDs[10];		//'ColorIDs' (lines) as int
	int		bagIDcts[10];	//'ColorIDs' (lines) as int
}			color;

void	storedata(char *file, int lines, struct ds *groups);
int		getnumberoflines(char *file, int *ptparts);
int		getcolid(char *colname, color *dat, int lines);
void	filldata(int lines, struct ds *dat);
int		hasshinygoldbag(color *dat, int line, int goldID);
long long	countGbags(color *dat, int line);

int		getcolid(char *colname, color *dat, int lines)
{
	int	i = 0;

	while (i < lines)
	{
		if (strcmp(colname, dat[i].col) == 0)
			return (i);
		i++;
	}
	return (-1);
}

//Assumption: a gold bag inside a gold bag does count, though makes it infinite loop?
long long	countGbags(color *dat, int line)
{
	int			bagN = 0;
	int			check_bagID = dat[line].bagIDs[bagN];
	long long	bags = 0;

	while (check_bagID != 707)
	{
		bags = bags + dat[line].bagIDcts[bagN] * (1 +  countGbags(dat, check_bagID));
		bagN++;
		check_bagID = dat[line].bagIDs[bagN];
	}
	return (bags);
}

int		hasshinygoldbag(color *dat, int line, int goldID)
{
	int		bagN = 0;
	int		check_bagID = dat[line].bagIDs[bagN];
	int		foundgold = 0;

	while (check_bagID != 707)
	{
		foundgold = hasshinygoldbag(dat, check_bagID, goldID);
		if (check_bagID == goldID || foundgold == 1)
			return (1);
		bagN++;
		check_bagID = dat[line].bagIDs[bagN];
	}
	return (0);
}

int		main(int argc, char **argv)
{
	int		ft = 0;
	char	*file0 = "input.txt";
	char	*file1 = "example.txt";
	char	*file2 = "example2.txt";
	char	*file3 = "example_adj.txt";
	char	*file4 = "example_simple.txt";
	char	*file;

	if (argc > 1)
		ft = atoi(argv[1]);
	if (ft == 0)
		file = file0;
	else if (ft == 1)
		file = file1;
	else if (ft == 2)
		file = file2;
	else if (ft == 3)
		file = file3;
	else if (ft == 4)
		file = file4;

	printf("= CHECKS AND BALANCES ========================================\n");
	//Step 1: Getting number of lines
	int		lines;
	int		parts;
	int		*ptparts;
	ptparts = &parts;
	
	lines = getnumberoflines(file, ptparts);

	//Checks TBD / DONE:
	// 1 - does every line has one 'rule', e.g. not "a contain b contain c"? YES
	// 2 - does every line represent a unique color, e.g. not two lines "a contain DEF" 
	//		"a contain IJK"? YES
	// 3 - are there similar colors, like "blue" and "pale blue"?
	//		NOT APPLICABLE - EXTENSIVE CHECK 2.
	printf("Number of lines: %i\n", lines);
	printf("Number of 'contains': %i\n", parts);

	printf("= ALLOC MEMORY & READING DATA ================================\n");
	//Step 2: Allocating arrays

	printf("Allocating data array...\n");
	color	*colorlines;
	colorlines = calloc(lines, sizeof(color));
	if (colorlines == NULL)
		printf("Allocation for data array failed");
		
	//Step 3: Reading file per MAXBUF chars
	printf("Start reading input...\n");
	storedata(file, lines, colorlines);
	printf("Enriching data...\n");
	filldata(lines, colorlines);
	
	printf("= CHECKS EXTRA ===============================================\n");
	int	i = 0, j = 0;
	int	db = 0;

	while (i < lines && db == 0)
	{
		j = i + 1;
		while (j < lines)
		{
			if (!strcmp(colorlines[i].col, colorlines[j].col))
				db++;
			j++;
		}
		i++;
	}
	printf("Are there double colours (db == 0 -> no)\t");
	printf("db = %i\n", db);

	printf("= PRINT TEST DATA ============================================\n");
	int	lastprint = 5;

	if (argc > 2)
		lastprint = atoi(argv[2]);
	
	int		goldID = getcolid("shiny gold", colorlines, lines);
	
	//Validate if reading data is succes
	i = 0;
	while (i < lastprint)
	{
		printf("line %-3i ", i);
		printf("col=%-20s", colorlines[i].col);
		printf("cts=");
		j = 0;
		while (j < 10)
		{
			printf("%i,", colorlines[i].bagIDcts[j]);
			j++;
		}
		//printf("\toth=%-60s", colorlines[i].cont);

		printf("\tbagIDs=");
		j = - 1;

		while (colorlines[i].bagIDs[j] != 707)
		{
			printf("%i-", colorlines[i].bagIDs[j + 1]);
			j++;
		}
		//printf("G:%i", hasshinygoldbag(colorlines, i, goldIDt));
		//printf("T:%lli", countGbags(colorlines, i));
		printf("\n");
		i++;
	}

	printf("= PT1 ANALYSIS ===============================================\n");

	int		bagswithgold = 0;
	int		goldIDt = getcolid("shiny gold", colorlines, lines);
	
	i = 0;
	while (i < lines)
	{
		bagswithgold = bagswithgold + hasshinygoldbag(colorlines, i, goldID);
		i++;
	}
	printf("Answer part 1 = %i\n", bagswithgold);
	//Not 6;
	//But 335.
	
	printf("= PT2 ANALYSIS ===============================================\n");
	
	long long		bagsingold = 0;

	bagsingold = countGbags(colorlines, goldID);

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

void		storedata(char *file, int lines, struct ds *dat)
{
	FILE	*in_file;
	char	line[MAXBUF];
	char	*pt, *end, end2;;
	char	newcolor[40];
	int		i, j;

	in_file = fopen(file, "r");
	if (in_file == NULL)
		printf("File read failed\n");
	
	i = 0;
	while (fgets(line, MAXBUF, in_file) != NULL)
	{
		//printf("reading: %s", line);
		pt = line;
		end = strchr(pt, ' ') + 1;
		end = strchr(end, ' ');
		strncpy(dat[i].col, line, end - line);
		pt = strstr(pt, "contain") + 7;
		j = 0;
		if (strstr(pt, "no other") == NULL)
		{
			pt = pt + 1;			//pt at begin of new col (number)
			while (pt != NULL)
			{
				dat[i].bagIDcts[j] = (int)(*pt - '0');
				pt = pt + 2;
				bzero(newcolor, 40);
				end = strchr(pt, ' ') + 1;
				end = strchr(end, ' ');
				strncpy(newcolor, pt, end - pt);
				strncat(dat[i].cont, newcolor, strlen(newcolor));
				strcat(dat[i].cont, "|");
				pt = strchr(pt, ',');
				if (pt != NULL)
				{
					pt = pt + 2;
					j++;
				}
			}
		}
		
		i++;
	}
	fclose(in_file);
}

//This is data enrichment

void	filldata(int lines, struct ds *dat)
{
	char	*pt, *end;
	char	newcolor[40];
	int		i, colID, bagN;
		
	i = 0;
	//Replace with get from dat
	while (i < lines)
	{
		colID = 707;
		bagN = 0;
		dat[i].bagIDs[bagN] = colID;		//Init to ColID (707 - PLANE :) ) 
		pt = dat[i].cont;
		//dat[i].csg = 0;		//Initializing CSG - not needed?
		while (*pt != '\0')
		{
			bzero(newcolor, 40);
			end = strchr(pt, '|');
			strncpy(newcolor, pt, end - pt);
			// ADD vertices
			colID = getcolid(newcolor, dat, lines);
			if (colID >= 0)
			{
				dat[i].bagIDs[bagN] = colID;
				bagN++;
			}
			else
				printf("Bag %s NOT FOUND in index (%i)\n", newcolor, colID);
			pt = end + 1;
		}
		dat[i].bagIDs[bagN] = 707;		//Init to ColID (707 - PLANE :) ) 
		i++;
	}
}
