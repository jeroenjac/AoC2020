#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define	MAXBUF 20
 
typedef struct	ds
{
	char	str[MAXBUF];		//Need to make this variable, useful for many applications?
	char	ins[4];
	int		arg;
	int		ct;
}			dat;

void	getdata(char *file, int lines, struct ds *groups);
int		getnumberoflines(char *file, int *ptparts);
int		step(dat *data, int *ptpos, int lines);

long	g_acm;

void	init_cts(dat *data, int lines)
{
	while (lines > 0)
	{
		lines--;
		data[lines].ct = 0;
	}
}

int		run(dat *data, int start, int lines, int *stp)
{
	g_acm = 0;
	int	pos = start;		//turned out it was not needed
	int	*ptpos = &pos;
	int	steps = 0;
	int	nextstep = 1;

	init_cts(data, lines);
	//For part 1 "debug mode".
	while (nextstep == 1)
	{
		nextstep = step(data, ptpos, lines);
		steps++;
	}
	if (nextstep == 3)
		printf("last instruction was invalid, pos over program length\n");
	*stp = steps;
	return (nextstep);	
}

int		step(dat *data, int *ptpos, int lines)
{
	//returns next step

	int		pos = *ptpos;
	char	*instruction = data[pos].ins;
	
	if (pos == lines)
		return (2);			//program is 1 past last line (DONE).
	if (pos > lines)
		return (3);			//program is >1 past last line (NOK).
	if (data[pos].ct == 1)
		return (0);			//prorgram is inf loop / circular
	data[pos].ct = 1;
	if (strcmp(instruction, "acc") == 0)
	{
		g_acm = g_acm + data[pos].arg;
		pos++;
	}
	else if (strcmp(instruction, "jmp") == 0)
		pos = pos + data[pos].arg;
	else if (strcmp(instruction, "nop") == 0)
		pos++;
	else
		printf("for pos %i, %s is an unknown instruction\n", pos, instruction);
	*ptpos = pos;
	return (1);
}

void	swap_nop_jmp(dat *data, int line)
{
	char	*instruction = data[line].ins;
	
	if (strcmp(instruction, "jmp") == 0)
		strcpy(instruction, "nop");
	else if (strcmp(instruction, "nop") == 0)
		strcpy(instruction, "jmp");
	else
		printf("can not swap the instruction for line %i\n", line);
}

long	checkswaps(dat *data, int lines)
{
	int	i = 0, cases = 0;
	int	terminates = 1;
	int steps;
	int	*stp = &steps;
	
	//get amount of cases: nop or jmp
	while (i < lines)
	{
		if (strcmp(data[i].ins, "acc"))
			cases++;
		i++;
	}
	printf("How many nop or jmp? %i\n", cases);
	i = 0;
	//find a nop or jmp -> change it -> run (check succes) -> swap back
	while (cases > 0 && i < lines)
	{	
		while (strcmp(data[i].ins, "acc") == 0)
			i++;
		swap_nop_jmp(data, i);
		terminates = run(data, 0, lines, stp);
		if (terminates == 2)
			return (terminates);
		swap_nop_jmp(data, i);		//swap back
		cases--;
		i++;
	}
	return (terminates);
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
		//printf("line %i = %s\n", i, data[i].str);
		printf("LINE %-3i: %s[%i]\n", i, data[i].ins, data[i].arg);
		i++;
	}

	printf("= PT1 ANALYSIS ===============================================\n");
	int		a1 = 0;
	int		dummy;
	int		*ptd = &dummy;
	int		steps;
	int		*stp = &steps;

	a1 = run(data, 0, lines, stp);
	printf("Answer part 1 = %li (%i steps, terminated code %i)\n", g_acm, steps, a1);
	// 1489

	printf("= PT2 ANALYSIS ===============================================\n");
	int		a2 = 0;
	//Step 1: what is the last instruction (e.g. jmp + 3) would be not defined? It is 'jmp +1' [OK]
	//Step 2: How many x nop or jmp? 280, we can 'brute force'.. . [OK]
	//Step 3: Adjust program on all possible places and let it run

	a2 = checkswaps(data, lines);
	printf("Answer part 2 = %li (%i steps, terminated code %i)\n", g_acm, steps, a2);
	// 1539

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
		strncpy(dat[line].ins, pt, 3);
		dat[line].arg = atoi(pt + 4);
		line++;
	}
	fclose(in_file);
}
