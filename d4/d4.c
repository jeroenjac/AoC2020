#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXL 20
#define	MAXBUF 200

typedef struct	passport
{
	char	byr[MAXL];
	char	iyr[MAXL];
	char	eyr[MAXL];
	char	hgt[MAXL];
	char	hcl[MAXL];
	char	ecl[MAXL];
	char	pid[MAXL];
	char	cid[MAXL];
}			pp;

void	storedata(char *file, int lines, pp *allpp);
int		getnumberoflines(char *file, int *ppnumpt);
int		ppval(pp *file);
int		ppval2(pp *file);
int		valbyr(pp *file);
int		valiyr(pp *file);
int		valeyr(pp *file);
int		valhgt(pp *file);
int		valhcl(pp *file);
int		valecl(pp *file);
int		valpid(pp *file);

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
	printf("Number of passport files: %i\n", ppfiles);


	printf("= ALLOC MEMORY & READING DATA ================================\n");
	//Step 2: Allocating arrays
	pp		*allpp;

	printf("allocating pp data array\n");
	allpp = calloc(ppfiles, sizeof(pp));
	if (allpp == NULL)
		printf("Allocation for pp data array failed");
	
	//Step 3: Reading file per MAXBUF chars
	printf("start reading input\n");
	storedata(file, lines, allpp);	
	
	printf("= PRINT TEST DATA ============================================\n");
	int	i = 31;
	int	ppe = 31;
	//Validate if reading data is succes
	while (i <= ppe)
	{
		printf("READ TEST: byr of pp %i = %s\n", i, allpp[i].byr);
		printf("READ TEST: iyr of pp %i = %s\n", i, allpp[i].iyr);
		printf("READ TEST: eyr of pp %i = %s\n", i, allpp[i].eyr);
		printf("READ TEST: hgt of pp %i = %s\n", i, allpp[i].hgt);
		printf("READ TEST: hcl of pp %i = %s\n", i, allpp[i].hcl);
		printf("READ TEST: ecl of pp %i = %s\n", i, allpp[i].ecl);
		printf("READ TEST: pid of pp %i = %s\n", i, allpp[i].pid);
		printf("READ TEST: cid of pp %i = %s\n", i, allpp[i].cid);
		if (ppval(&allpp[i]) == 0)
			printf("pp %i is invalid\n", i);
		i++;
	}

	printf("= ANALYSIS ===================================================\n");
	i = 0;
	int		totvalid = 0;
	int		totvalid2 = 0;
	int		valid = 0;
	while (i < ppfiles)
	{
		valid = ppval2(&allpp[i]);
		totvalid = totvalid + ppval(&allpp[i]);
		totvalid2 = totvalid2 + ppval2(&allpp[i]);
		//if (valid == 0)
		//	printf("PP %i is invalid\n", i);
		i++;
	}
	printf("Number of valid passports: %i\n", totvalid);
	//251 is too high
	
	printf("Number of valid passports (pt2): %i\n", totvalid2);
	
	free(allpp);
	return (0);
}

int		ppval(pp *file)
{
	if (strlen(file->byr) == 0)
		return (0);
	if (strlen(file->iyr) == 0)
		return (0);
	if (strlen(file->eyr) == 0)
		return (0);
	if (strlen(file->hgt) == 0)
		return (0);
	if (strlen(file->hcl) == 0)
		return (0);
	if (strlen(file->ecl) == 0)
		return (0);
	if (strlen(file->pid) == 0)
		return (0);
	return (1);
}

int		ppval2(pp *file)
{
	/*
	int	valid;

	valid = valbyr(file) * valiyr(file) * valeyr(file) * valhgt(file)\
			*valhcl(file) * valecl(file) * valpid(file);;
	return (valid);
	*/
	if (valbyr(file) == 0)
		return 0;
	if (valiyr(file) == 0)
		return 0;
	if (valeyr(file) == 0)
		return 0;
	if (valhgt(file) == 0)
		return 0;
	if (valhcl(file) == 0)
		return 0;
	if (valecl(file) == 0)
		return 0;
	if (valpid(file) == 0)
		return 0;
	return 1;
}

int		valbyr(pp *file)
{
	int	num;
	
	num = atoi(file->byr);
	if (num >= 1920 && num <= 2002)
		return 1;
	return 0;
}
	
int		valiyr(pp *file)
{
	int	num;
	
	num = atoi(file->iyr);
	if (num >= 2010 && num <= 2020)
		return 1;
	return 0;
}

int		valeyr(pp *file)
{
	int	num;
	
	num = atoi(file->eyr);
	if (num >= 2020 && num <= 2030)
		return 1;
	return 0;
}

int		valhgt(pp *file)
{
	int	h;

	if (strstr(file->hgt, "cm") != NULL)
	{
		h = atoi(file->hgt);
		if (h >= 150 && h <= 193)
			return 1;
	}
	if (strstr(file->hgt, "in") != NULL)
	{
		h = atoi(file->hgt);
		if (h >= 59 && h <= 76)
			return 1;
	}
	return 0;
}

int		valhcl(pp *file)
{
	char	*pt = file->hcl;
	int		i = 0;
	
	if (*pt != '#')
		return 0;
	pt++;
	if (strlen(pt) != 6)
		return 0;
	while (i < 6)
	{
		if (!(isdigit(*pt) || (*pt >= 'a' && *pt <= 'f')))
			return 0;
		i++;
		pt++;
	}
	return 1;

}

int		valecl(pp *file)
{
	char	*code = file->ecl;
	if (strlen(code) != 3)
		return 0;
	if (	strcmp(code, "amb") == 0 ||\
			strcmp(code, "blu") == 0 ||\
			strcmp(code, "brn") == 0 ||\
			strcmp(code, "gry") == 0 ||\
			strcmp(code, "grn") == 0 ||\
			strcmp(code, "hzl") == 0 ||\
			strcmp(code, "oth") == 0		)
		return 1;
	return 0;
}

int		valpid(pp *file)
{
	char	*pid = file->pid;
	
	if (strlen(pid) != 9)
		return 0;
	while (*pid != '\0')
	{
		if (!isdigit(*pid))
			return 0;
		pid++;
	}
	return 1;
}

void	readdatafield(char *pt, char *field, pp *curpp)
{
	char	*eod;

	if ((pt = strstr(pt, field)) != NULL)
	{
		eod = strchr(pt, ' ');
		if (eod == NULL)
			eod = strchr(pt, '\n');
		strncpy(curpp->byr, pt, eod - pt); //CAN NOT MAKE ->XXX dynamic :(.
		//Not null term, but not needed bc calloc = 00000...0?
	}
}

void		storedata(char *file, int lines, pp *allpp)
{
	FILE	*in_file;
	char	line[MAXBUF];
	int		i;
	
	char	*pt; 
	char	*eod;
	int		kl = 4;

	in_file = fopen(file, "r");
	if (in_file == NULL)
		printf("File read failed\n");
	
	i = 0;
	while (fgets(line, MAXBUF, in_file) != NULL)
	{
		//printf("reading: %s", line);
		if (*line != '\n')
		{
			pt = line;
			//Here sth smart to get pp fields
			if ((pt = strstr(line, "byr:")) != NULL)
			{
				eod = strchr(pt, ' ');
				if (eod == NULL)
					eod = strchr(pt, '\n');
				strncpy(allpp[i].byr, pt + kl, eod - (pt + kl));
			}
			if ((pt = strstr(line, "iyr:")) != NULL)
			{
				eod = strchr(pt, ' ');
				if (eod == NULL)
					eod = strchr(pt, '\n');
				strncpy(allpp[i].iyr, pt + kl, eod - (pt + kl));
			}
			if ((pt = strstr(line, "eyr:")) != NULL)
			{
				eod = strchr(pt, ' ');
				if (eod == NULL)
					eod = strchr(pt, '\n');
				strncpy(allpp[i].eyr, pt + kl, eod - (pt + kl));
			}
			if ((pt = strstr(line, "hgt:")) != NULL)
			{
				eod = strchr(pt, ' ');
				if (eod == NULL)
					eod = strchr(pt, '\n');
				strncpy(allpp[i].hgt, pt + kl, eod - (pt + kl));
			}
			if ((pt = strstr(line, "hcl:")) != NULL)
			{
				eod = strchr(pt, ' ');
				if (eod == NULL)
					eod = strchr(pt, '\n');
				strncpy(allpp[i].hcl, pt + kl, eod - (pt + kl));
			}
			if ((pt = strstr(line, "ecl:")) != NULL)
			{
				eod = strchr(pt, ' ');
				if (eod == NULL)
					eod = strchr(pt, '\n');
				strncpy(allpp[i].ecl, pt + kl, eod - (pt + kl));
			}
			if ((pt = strstr(line, "pid:")) != NULL)
			{
				eod = strchr(pt, ' ');
				if (eod == NULL)
					eod = strchr(pt, '\n');
				strncpy(allpp[i].pid, pt + kl, eod - (pt + kl));
			}
			if ((pt = strstr(line, "cid:")) != NULL)
			{
				eod = strchr(pt, ' ');
				if (eod == NULL)
					eod = strchr(pt, '\n');
				strncpy(allpp[i].cid, pt + kl, eod - (pt + kl));
			}
		}
		else
			i++;
	}
	fclose(in_file);
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
