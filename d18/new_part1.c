#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>
#include <limits.h>

#define maxbuf 200

unsigned long long	calc_line_simple(char *c, int *i);
unsigned long long	calc(char *c, unsigned long long in, int *i);

int main (int argc, char **argv)
{
	char	*filename;
	if (argc < 2)
	{	
		printf("No input\n");
		return (0);
	}
	filename = strdup(argv[1]);

	FILE	*file = fopen(filename, "r");
	char	line[maxbuf];
	char	*pt;

	fgets(line, sizeof(line), file);

	printf("read line: %s", line);
	pt = line;
	printf("TEST: first digit * 10 = %i\n", 10 * ( pt[0] - '0' ));
	int	tot0;
	int i = 0;
	tot0 = calc_line_simple(pt, &i);
	printf("Sum of line = %i (chars parsed: %i)\n", tot0, i);
	fclose(file);

	file = fopen(filename, "r");
	unsigned long long tot = 0;
	int	j = 0;
	int	chars = 0;
	while (fgets(line, sizeof(line), file) != NULL)
	{
		printf(">>>> read line %-3i: %s", j + 1, line);
		pt = line;
		chars += i;
		i = 0;
		tot += calc_line_simple(pt, &i);
		bzero(line, sizeof(line));
		j++;
	}
	printf("Sum of all lines = %llu (chars parsed: %i)\n", tot, chars);
	// 9768840475 is too low
	// 11298180591 too low
	// 3644840513007 too low
	// 315307856637318172 not correct ?
	printf("MAX INT          = %lu\n", ULONG_MAX); 

	return (0);
}

unsigned long long calc_line_simple(char *c, int *j)
{
	unsigned long long sum = 0;
	int	i = 0;
	
	printf("START FUNCTION: i = %i / sum = %lli\n", i, sum);
//	printf("String[0] = %s", &c[0]);
//	printf("String[i] = %s", &c[i]);

	while (c[i] != '\n' && c[i] != ')')
	{
		if (c[i] == ' ' )
			i += 1;
		else if (isdigit(c[i]))
		{
			sum += (unsigned long long) (c[i] - '0');
			i += 2;
		}
		else if (c[i] == '*' || c[i] == '+')
			sum = calc(c, sum, &i);
		else if (c[i] == '(')
		{
			i += 1;
			sum += calc_line_simple(&c[i], &i);
			i += 1;
		}
	}
	*j += i;
	printf("ENDOF FUNCTION: i = %i / sum = %llu\n", i, sum);
	return (sum);
}

unsigned long long	calc(char *c, unsigned long long in, int *j)
{
	char	operation = c[*j];
	char	term = c[*j + 2];
	unsigned long long	out = 0;

//	printf("i = %i. starting operation %c on term %c\n", *j, operation, term);
//	printf("String[0] = %s", &c[0]);
//	printf("String[i] = %s", &c[*j]);
	
	if (term != '(')
	{
//		printf("OP1: * or +\n");
		if (operation == '*')
			out = (unsigned long long) ( term - '0' ) * in;
		else if (operation == '+')
			out = (unsigned long long) ( term - '0' ) + in;
		*j += 3;	//CHECK RIGHT METHOD
	}
	else if (term == '(')
	{
//		printf("OP2: Calling function\n");
		*j += 3;
		if (operation == '*')
			out = in * calc_line_simple(&c[*j], j);
		else if (operation == '+')
			out = in + calc_line_simple(&c[*j], j);
		*j += 1;
	}
//	printf("i = %i. end of operation. out = %lli\n", *j, out);
//	printf("String[i] = %s", &c[*j]);
	return (out);
}

//Two functions
// 1 - start calc with new line or (. This function exits wiht resp \n or ).
// 2 - run calc. Following options:
//		a. digit - use for operation
//		b. operation
//		c. start new (one level up) with (
