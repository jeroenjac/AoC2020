#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>

#define maxbuf 100

int	calc_line(char *c, int *i);
int	calc_line_simple(char *c, int *i);
int	calc(char *c, int in, int *i);

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

	printf("Read line: %s", line);
	pt = line;
	int i;

	int	tot;
	i = 0;
	tot = calc_line_simple(pt, &i);
	printf("Sum of line = %i (chars parsed: %i)\n", tot, i);
	
	printf("TEST: first digit * 10 = %i\n", 10 * ( pt[0] - '0' ));

	return (0);
}

int	calc_line_simple(char *c, int *j)
{
	int sum = 0;
	int	i = *j;
	
	printf("START FUNCTION: i = %i / sum = %i on %s", i, sum, c);

	while (c[i] != '\n')
	{
		printf("BEG loop i = %i / sum = %i\n", i, sum);
		if (isdigit(*c))
			sum += *c - '0';
		else if (c[i] == '*' || c[i] == '+')
			sum = calc(&c[i], sum, &i);		//fun calc?
		else if (c[i] == ')')
		{
			*j = i + 1;
			return (sum);
		}
		i++;
		printf("END loop i = %i / sum = %i\n", i, sum);
	}
	*j = i;
	return (sum);
}

int	calc(char *c, int in, int *j)
{
	int		out = 0;
	int		term = 0;
	int		i = *j;
	char	operation = c[0];

	printf("i = %i. starting operation %c on string %s", i, operation, c);
	i += 2;
	printf("c[2] = %c\n", c[2]);
	if (c[2] == '(')
	{
		i++;
		term = calc_line_simple(&c[3], &i);
	}
	else
		term = c[2] - '0';

	if (operation == '*')
		out = in * term;
	if (operation == '+')
		out = in + term;
	*j = i;
	return (out);
}
