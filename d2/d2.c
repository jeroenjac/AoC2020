#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

#define	MAXBUF 200

int		main(void)
{
	char	*file = "input.txt";
	FILE	*in_file;
	char	line[MAXBUF];

	printf("==== CHECKS AND BALANCES ====\n");
	//Step 1: Getting number of lines
	in_file = fopen(file, "r");
	if (in_file == NULL)
		printf("File read failed\n");
	
	int		n_lines = 0;
	while (fgets(line, sizeof(line), in_file) != NULL)
		n_lines++;
	fclose(in_file);

	printf("Number of lines: %i\n", n_lines);

	//Step 2: Allocating arrays (this should be struct)
	int		*minc;
	int		*maxc;
	char	c[n_lines];
	char	**pw;


	printf("allocating int arrays\n");
	minc = calloc(n_lines, sizeof(int));
	maxc = calloc(n_lines, sizeof(int));
	if (minc == NULL || maxc == NULL)
		printf("memory could not be allocated\n");
	
	printf("allocating char arrays\n");
	pw = (char **)calloc(n_lines, sizeof(char *));
	if (pw == NULL)
		printf("mallof pw failedi\n");
	int		i = 0;
	printf("start reading input\n");
	while (i < n_lines)
	{
		pw[i] = (char *)calloc(50, sizeof(char));
		if (pw[i] == NULL)
			printf("malloc of line %i failed\n", i);
		i++;
	}

	char	str[MAXBUF];
	char	*pt;
	int		j = 0;

	//Step 3: Reading file per MAXBUF chars
	in_file = fopen(file, "r");
	if (in_file == NULL)
		printf("File read failed\n");
	i = 0;
	while (fgets(str, MAXBUF, in_file) != NULL)
	{
		pt = str;
		//printf("reading: %s\n", str);
		while (*pt != '-')
		{
			minc[i] = ((char)*pt - '0') + 10 * minc[i];
			pt++;
		}
		pt++; //The '-'
		while (*pt != ' ')
		{
			maxc[i] = ((char)*pt - '0') + 10 * maxc[i];
			pt++;
		}
		pt++; //The space
		c[i] = *pt;
		pt = pt + 3; //Skip the char, : and space
		while (*pt != '\n')
		{
			*(*(pw + i) + j) = *pt;
			j++;
			pt++;
		}
		*(*(pw + i) + j) = '\0';
		i++;
		j = 0;
	}
	fclose(in_file);

	//Step 4: Validate if reading data is succesfull
	i = 0;
	while (i < 5)
	{
		printf("%-5i: minc = %-3i\t", i + 1, minc[i]);
		printf("maxc = %-3i\t", maxc[i]);
		printf("char = %c\t", c[i]);
		printf("pw = %s\n", pw[i]);
		i++;
	}
	i = n_lines - 1;
	printf("%-5i: minc = %-3i\t", i + 1, minc[i]);
	printf("maxc = %-3i\t", maxc[i]);
	printf("char = %c\t", c[i]);
	printf("pw = %s\n", pw[i]);

	//Step 5: Count and Analyse - pt1

	int		cntc = 0;
	int		valid = 0;

	i = 0;
	while (i < n_lines)
	{
		j = 0;
		cntc = 0;
		while (*(*(pw + i) + j) != '\0')
		{
			if (*(*(pw + i) + j) == c[i])
				cntc++;
			j++;
		}
		if (cntc >= minc[i] && cntc <= maxc[i])
		{
			valid++;
			//if (i % 5 == 0)
				//printf("pw %i (%s) is valid\n", i, *(pw + i));
		}
		i++;
	}
	printf("Number of valid pw (pt1) = %i\n", valid);
	
	//Step 5b: Count and Analyse - pt2

	cntc = 0;
	valid = 0;

	i = 0;
	while (i < n_lines)
	{
		j = 0;
		cntc = 0;
		if ((pw[i][minc[i] - 1] == c[i]) + (pw[i][maxc[i] - 1] == c[i]) == 1)
		{
			valid++;
			//if (i % 5 == 0)
			//	printf("pw %i (%s) is valid: %i\n", i, *(pw + i),\
				(pw[i][minc[i] - 1] == c[i]) + (pw[i][maxc[i] - 1] == c[i]) == 1);
		}
		i++;
	}
	printf("Number of valid pw (pt2)= %i\n", valid);
}
