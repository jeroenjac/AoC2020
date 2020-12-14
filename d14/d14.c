#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ds
{
	char	type[5];
	char	*mask;
	int		addr;
	long	newval;
}			inst;

int		*get_bits(long n, int bitswanted);
int		*makemask36(char *mask, int x_bit);
int		*applymasktobits(char *mask, int *bits);
void	print_bits(int	*bits);

int	main(void)
{
	inst	*test;
	int		testsize = 5;
	int		*memory;

	test = calloc(testsize, sizeof(inst));
	if (test == NULL)
		printf("Calloc for test failed\n");
	memory = calloc(60, sizeof(*memory));
	if (memory == NULL)
		printf("Calloc for memory failed\n");

	//test data
	test[0].mask = strdup("XXXXXXXXXXXXXXXXXXXXXXXXXXXXX1XXXX01");
	test[1].newval = 101;
	
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
	//TBD

	free(bits);
	free(aftermask);

	return (0);
}

// get lines --> create struct instructions
//	type mask/mem (str)
//	mask value (str)
//	address(int)
//	newvalue(int)
// get mem instr & put in struct
// get mask instr & put in struct
// get max mem address -> define mem struct/int?
//
// interpretation...
//


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
