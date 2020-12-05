#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAXL 20
#define	MAXBUF 200

typedef struct	boardingpass
{
	char	row[MAXL];
	char	col[MAXL];
	int		rownum;
	int		colnum;
	long	seatID;
}			pass;

void	storedata(char *file, int lines, pass *bp);
int		getnumberoflines(char *file, int *ppnumpt);
void	strtobin(char *str, char one);
long	bintoseat(pass *ap);
int		ppval(pass *file);
int		ppval2(pass *file);
void	fillplane(char plane[128][8]);
void	printplane(char plane[128][8]);

int		main(void)
{
	char	*file = "input.txt";
	FILE	*in_file;

	printf("= CHECKS AND BALANCES ========================================\n");
	//Step 1: Getting number of lines
	int		lines;
	int		ppfiles;
	int		*ppnumpt;
	ppnumpt = &ppfiles;
	
	lines = getnumberoflines(file, ppnumpt);

	printf("Number of lines: %i\n", lines);
	printf("Number of boarding passes: %i\n", ppfiles);


	printf("= ALLOC MEMORY & READING DATA ================================\n");
	//Step 2: Allocating arrays
	pass	*ap;

	printf("allocating pp data array\n");
	ap = calloc(lines, sizeof(pass));
	if (ap == NULL)
		printf("Allocation for data array failed");
	
	//Step 3: Reading file per MAXBUF chars
	printf("start reading input\n");
	storedata(file, lines, ap);


	printf("= PRINT TEST DATA ============================================\n");
	int	i = 0;
	int	ppe = 5;
	//Validate if reading data is succes
	int	seatnum = 0;

	while (i <= ppe)
	{
		printf("READ TEST: rowcode %i = %s\t", i, ap[i].row);
		printf("colcode %i = %s\t", i, ap[i].col);
		printf("SeatID = %li\n", ap[i].seatID);
		i++;
	}

	printf("= PT1 ANALYSIS ===============================================\n");
	//Find max seat id
	long maxID = 0;
	i = 0;
	while (i < lines)
	{
		if (maxID < ap[i].seatID)
			maxID = ap[i].seatID;
		i++;
	}
	printf("Max id = %li\n", maxID);

	printf("= PT2 VISUAL INSP / GREP =====================================\n");
	//Print Plane & check which seats are free

	//Fina my seat, multiply row num * 8 and add col pos.
	char	plane[128][8];
	
	fillplane(plane);
	//Mark all occ seats with "X".
	i = 0;
	while (i < lines)
	{
		plane[ap[i].rownum][ap[i].colnum] = 'X';
		i++;
	}
	//Visual inspection to find my seat, or use grep "XOX"
	printplane(plane);

	free(ap);
	return (0);
}

int		getnumberoflines(char *file, int *ppnumpt)
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
	*ppnumpt = ppfiles;
	fclose(in_file);

	return (lines);
}

void		storedata(char *file, int lines, pass *ap)
{
	FILE	*in_file;
	char	line[MAXBUF];
	int		i, j;
	
	char	*pt;

	in_file = fopen(file, "r");
	if (in_file == NULL)
		printf("File read failed\n");
	
	i = 0;
	j = 0;
	while (fgets(line, MAXBUF, in_file) != NULL)
	{
		//printf("reading: %s", line);
		if (*line != '\n')
		{
			pt = line;
			strncpy(ap[i].row, pt, 7);
			strtobin(ap[i].row, 'B');
			pt = pt + 7;
			strncpy(ap[i].col, pt, 3);
			strtobin(ap[i].col, 'R');
			ap[i].seatID = bintoseat(&ap[i]);
		}

		i++;
	}
	fclose(in_file);
}

void	strtobin(char *str, char one)
{
	while (*str != '\0')
	{
		if (*str == one)
			*str = '1';
		else
			*str = '0';
	str++;
	}
}

long	bintoseat(pass *ap)
{
	long	seat = -1;
	int		rownum;
	int		colnum;

	rownum = strtol(ap->row, NULL, 2);
	colnum = strtol(ap->col, NULL, 2);
	ap->rownum = rownum;
	ap->colnum = colnum;

	seat = rownum * 8 + colnum;
	return (seat);
}

void	fillplane(char plane[128][8])
{
	int		i = 0;
	int		j = 0;

	while (i < 128)
	{
		j = 0;
		while (j < 8)
		{
			plane[i][j] = 'O';
			j++;
		}
		i++;
	}
}

void	printplane(char plane[128][8])
{
	int		i = 0;
	int		j = 0;

	while (i < 128)
	{
		j = 0;
		printf("row %-3i: ", i);
		while (j < 8)
		{
			printf("%c", plane[i][j]);
			j++;
		}
		printf("\n");
		i++;
	}
}
