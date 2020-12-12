#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

#define	MAXBUF 10

int		find2nums(int *arr, int size, int sum, int *coords);

int		main(void)
{
	char	*file = "input.txt";
	FILE	*in_file;
	char	line[10];

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

	//Step 2: Allocating int array
	int		*exrep;

	exrep = calloc(n_lines, sizeof(int));
	if (exrep == NULL)
		printf("memory could not be allocated\n");
	
	int		i = 0;
	char	str[MAXBUF];
	char	*pt;

	//Step 3: Reading file per MAXBUF chars
	in_file = fopen(file, "r");
	if (in_file == NULL)
		printf("File read failed\n");
	while (fgets(str, MAXBUF, in_file) != NULL)
	{
		pt = str;
		while (*pt != '\n')
		{
			exrep[i] = ((char)*pt - '0') + 10 * exrep[i];
			pt++;
		}
		i++;
	}
	fclose(in_file);
	i = 0;
	//Step 4: Validate if reading numbers into array is succes
	while (i < 5)
	{
		printf("number %i = %i\n", i + 1, exrep[i]);
		i++;
	}	
	i = n_lines - 1;
	printf("number %i = %i\n", i + 1, exrep[i]);

	//Step 5: Finding tne nums
	int		p1c[2];
	
	printf("==== SOLUTION PART 1 ====\n");
	printf("Solution part 1 = %i\n", find2nums(exrep, n_lines, 2020, p1c));
	printf("(num %i  = \t%i)\n",\
		p1c[0],\
		exrep[p1c[0]]);
	printf("(num %i  = \t%i)\n",\
		p1c[1],\
		exrep[p1c[1]]);

	//Step 6: Finding tne nums for 2nd part
	printf("==== SOLUTION PART 2 ====\n");
	int		num1 = 0;
	int		num2 = 0;
	int		num3 = 0;

	while (num1 < n_lines)
	{
		num2 = 0;
		while (num2 < n_lines)
		{
			num3 = 0;
			while (num3 < n_lines)
			{
				if (exrep[num1] + exrep[num2] + exrep[num3] == 2020)
					printf("Solution (%i, %i, %i) = %i\n", num1, num2, num3, \
				exrep[num1] * exrep[num2] * exrep[num3]);
				num3++;
			}	
			num2++;
		}
		num1++;
	}
	
	//Step 6: Alternative method
	printf("==== SOLUTION PART 2 (alternative) ====\n");
	num3 = 0;
	int	sol2sum = -10000;
	int	sol2prod = -10000;
	find2nums(exrep, n_lines, 2020 - exrep[num3], p1c);
	int	sum3nums = exrep[num3] + exrep[p1c[0]] + exrep[p1c[1]];
	while (num3 < n_lines - 1 && sum3nums != 2020)
	{
		num3++;
		sol2prod = find2nums(exrep, n_lines, 2020 - exrep[num3], p1c);
		sum3nums = exrep[p1c[0]] + exrep[p1c[1]] + exrep[num3];
	}
	printf("Solution = %i\n", sol2prod * exrep[num3]);
	printf("(num %i  = \t%i)\n",\
		p1c[0],\
		exrep[p1c[0]]);
	printf("(num %i  = \t%i)\n",\
		p1c[1],\
		exrep[p1c[1]]);
	printf("(num %i  = \t%i)\n",\
		num3,\
		exrep[num3]);
	return (0);
}

int		find2nums(int *arr, int size, int sum, int *coords)
{
	int		num1 = 0;
	int		num2 = 0;
	while (num1 < size)
	{
		coords[0] = num1;
		num2 = 0;
		while (num2 < size)
		{
			coords[1] = num2;
			if (arr[num1] + arr[num2] == sum)
				return (arr[num1] * arr[num2]);
			num2++;
		}
		num1++;
	}
	return (-1);
}
