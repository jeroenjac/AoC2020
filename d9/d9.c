#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <limits.h>

#define	MAXBUF 200
 
typedef struct	ds
{
	long 	num;
}			dat;

void	getdata(char *file, int lines, struct ds *groups);
int		getnumberoflines(char *file, int *ptparts);
int		numissum(dat *numss, int pos, int pream);
long	findmin(long *range, int n);
long	findmax(long *range, int n);

/*
** Part 1 functions
*/

long	findoddnum(dat *numss, int lines, int pream)
{
	int	pos = pream;

	while (pos < lines)
	{
		if (numissum(numss, pos, pream) == 0)
			return (numss[pos].num);
	pos++;
	}
	return (-42);
}

int		numissum(dat *numss, int pos, int pream)
{
	int i = pos - pream;
	int j;
	int num1, num2;

	while (i < pos)
	{
		j = i + 1;
		while (j < pos)
		{
			num1 = numss[i].num;
			num2 = numss[j].num;
			if (num1 + num2 == numss[pos].num)
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}

/*
** Part 2 Function - this could be split up into smaller pieces much better..
*/

long	rangesum(dat *numss, int begin, int end)
{
	long	sum = 0;
	int		i = begin;

	while (i <= end)
	{
		sum = sum + numss[i].num;
		i++;
	}
	return (sum);
}

long	part2(dat *numss, int oddnum, int lines)
{
	int	begin = 0;
	int	end;
	int	found = 0;

	while (begin < lines && found == 0)
	{
		end = begin + 1;
		while (end < lines && found == 0)
		{
			if (rangesum(numss, begin, end) == oddnum)
				found = 1;
			end++;
		}
		begin++;
	}
	end--;
	begin--;
	printf("range (pos): %i - %i\n", begin, end);

	long	*range;
	range = calloc(end - begin + 1, sizeof(long));
	if (range == NULL)
		printf ("Malloc for num range failed\n");
	
	int		io = begin;
	int		in = 0;
	while (io <= end)
	{
		range[in] = numss[io].num;
		io++;
		in++;
	}
	
	//return min + max in range
	long	min = findmin(range, end - begin + 1);
	long	max = findmax(range, end - begin + 1);
	free(range);
	
	return (min + max);
}

/*
** Util functions - normal programming languages might have these standard ;-)
*/

long	findmin(long *range, int n)
{
	int		i = 0;
	long	min = range[i];

	while (i < n)
	{
		if (range[i] < min)
			min = range[i];
		i++;
	}
	return (min);
}

long	findmax(long *range, int n)
{
	int		i = 0;
	long	max = range[i];

	while (i < n)
	{
		if (range[i] > max)
			max = range[i];
		i++;
	}
	return (max);
}

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
	int		parts;
	ptparts = &parts;
	
	printf("INT MAX = %i\n", INT_MAX);
	printf("LONG MAX = %li\n", LONG_MAX);
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
		printf("number %-3i = %li\n", i, numss[i].num);
		i++;
	}

	printf("= PT1 ANALYSIS ===============================================\n");
	int		pream;
	long	firstoddnum = -2003;

	if (ft == 0)
		pream = 25;
	if (ft == 1)
		pream = 5;
	
	firstoddnum = findoddnum(numss, lines, pream);
	printf("Answer part 1 = %li\n", firstoddnum);
	// 18272118		
	
	printf("= PT2 ANALYSIS ===============================================\n");
	long	sumof2 = 0;

	sumof2 = part2(numss, firstoddnum, lines);
	printf("Answer part 2 = %li\n", sumof2);
	// 2186361
	
	free(numss);
	printf("= END =======================================================\n");
	return (0);
}

/*
** Functions 'test reads' the data and returns number of lines
** + stores number of 'parts/files' (for example sep. by newline) to the pointers
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
	char	*pt;
	int		i;

	lines = 666;
	in_file = fopen(file, "r");
	if (in_file == NULL)
		printf("File read failed\n");
	
	i = 0;
	while (fgets(line, MAXBUF, in_file) != NULL)
	{
		//printf("reading: %s", line);
		pt = line;
		dat[i].num = atoi(line);
		i++;
	}
	fclose(in_file);
}
