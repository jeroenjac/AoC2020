#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

typedef struct ds
{
	char	type[5];
	char	*mask;
	int		addr;
	long	newval;
}			inst;

int		*get_bits(long n, int bitswanted);
//int		*makemask36(char *mask, int x_bit);
int		*applymasktobits(char *mask, int *bits);
void	print_bits(int	*bits);
long	bits_to_int(int *bits);

int	main(int argc, char **argv)
{
	int		memslots = 100000;
	long	*memory;

	memory = calloc(memslots, sizeof(*memory));
	if (memory == NULL)
		printf("Calloc for memory failed\n");
	if (argc < 2)
		printf("No input\n");
	
	inst	*test;
	int		testsize = 5;
	test = calloc(testsize, sizeof(inst));
	if (test == NULL)
		printf("Calloc for test failed\n");
	
	//test data
	test[0].mask = strdup("XXXXXXXXXXXXXXXXXXXXXXXXXXXXX1XXXX0X");
	test[1].newval = 0;
	
	//first input data - manual
	strcpy(test[2].type, "mask");
	test[2].mask = strdup("0X11XX1X010X01101000X01X011101100000");
	strcpy(test[3].type, "mem");
	test[3].addr = 4634;
	test[3].newval = 907;
	strcpy(test[2].type, "mem");
	test[4].addr = 54949;
	test[4].newval = 444591415;

	printf("Write %li on addr %i\n", test[1].newval, test[3].addr);
	int	*bits = get_bits(test[1].newval, 36);
	printf("Test print the bits:\n");
	print_bits(bits);
	printf("(Test) print the mask:\n%s\n", test[0].mask);
	printf("Apply the mask, print the result:\n");
	int	*aftermask = applymasktobits(test[0].mask, bits);
	print_bits(aftermask);
	printf("Convert new bits back to int\n");
	printf("2^36	 = %li\n", (long int)pow(2, 36));
	printf("long_max = %li\n", LONG_MAX);
	printf("Int before = %li\n", bits_to_int(bits));
	printf("Int after =  %li\n", bits_to_int(aftermask));
	free(bits);
	free(aftermask);

	//Part 1
	char	*filename = strdup(argv[1]);;
	FILE	*file = fopen(filename, "r");
	char	line[50];
	char	*pt;
	char	*mask;
	int		mempos = 0;
	int		num;
	int		*num_in_bits;
	int		*mask_result;
	long	mask_result_to_int;
	
	mask = strndup("aaaaa", 2);
	while (fgets(line, sizeof(line), file) != NULL)
	{
		pt = line;
		printf("Reading: %s", pt);
		if (pt[1] == 'a')
		{
			free(mask);
			mask = strndup(pt + 7, 36);
		}
		else if (pt[1] == 'e')
		{
			mempos = atoi(pt + 4);
			pt = strchr(pt, '=') + 2; 
			num = atoi(pt);
			printf("Num = %i\n", num);
			num_in_bits = get_bits(num, 36);
			print_bits(num_in_bits);
			mask_result = applymasktobits(mask, num_in_bits);
			print_bits(mask_result);
			mask_result_to_int = bits_to_int(mask_result);
			memory[mempos] = mask_result_to_int;
			free(num_in_bits);
			free(mask_result);
		}
		else
			printf("Error line input\n");
		printf("mask = %s\n", mask);
		printf("mem[%i] = %li\n", mempos, mask_result_to_int);
		bzero(line, 50);
	}
	fclose(file);
	
	long sumsum = 0;
	mempos = 0;
	while (mempos < memslots)
	{
		sumsum += memory[mempos];
		mempos++;
	}
	printf("Sum of memory = %li\n", sumsum);


	return (0);
}

/*
** PART2
**	adjust write mask function - leave X
**	while reading lines, loop over all 2^(number of X) - apply X variations 0 / 1
**	for every variation, write value to mem

*/

int		*get_bits(long n, int bitswanted)
{
	int *bits = calloc(sizeof(int), bitswanted);
	int k = 0;

	while (k < bitswanted)
	{
		long mask =  1 << k;
		long masked_n = n & mask;
		int thebit = masked_n >> k;
		bits[k] = thebit;
		k++;
	}
	return (bits);
}

int		*applymasktobits(char *mask, int *bits)
{
	int	size = 36;
	int k = 0;
	int *result = calloc(sizeof(int), size);

	while (k < size)
	{
		result[size - k - 1] = (mask[k] == 'X') ? bits[size - k - 1] : (int) (mask[k] - '0');
		k++;
	}
	return (result);
}

void	print_bits(int	*bits)
{
	int i = 36;

	while (i > 0)
	{
		i--;
		printf("%d", bits[i]);
	}
	printf("\n");
}

long	bits_to_int(int *bits)
{
	long	number = 0;
	int		bitnum = 35;

	while (bitnum >= 0)
	{
		number += bits[bitnum] * pow(2, bitnum);
		bitnum--;
	}
	return (number);
}
