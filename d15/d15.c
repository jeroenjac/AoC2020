#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define maxturns 30000000

typedef	struct	pos
{
	int	called;
	int	prev1;
	int	prev2;
} pos;

int		arrlen(int *arr);
void	printarr(int *arr);
void	printturns(int *allturns, int from, int to);
void	printlookup(pos *pos, int s, int e);
void	fillpos(pos *posit, int turn);
int		guess_age(int *tur, int t);
int		guess_fast(int *tur, int t, pos *posit);

int	main(int argc, char **argv)
{
	printf("= START ========================\n");
	//different examples
	int		example1[4] = {0, 3, 6, -42};
	int		example2[4] = {1, 3, 2, -42};
	int		example3[4] = {2, 1, 3, -42};
	int		example4[4] = {1, 2, 3, -42};
	int		example5[4] = {2, 3, 1, -42};
	int		example6[4] = {3, 2, 1, -42};
	int		example7[4] = {3, 1, 2, -42};
	//my input
	int		input[8] = {16, 12, 1, 0, 15, 7, 11, -42};

	int		*arr;
	int		ft = 0;
	int		printlim = (argc > 2) ? atoi(argv[2]) : 5;
	
	//easy running examples/input
	ft = (argc > 1) ? atoi(argv[1]) : 0;
	if (ft == 1)
		arr = &example1[0];
	else if (ft== 2)
		arr = &example2[0];
	else if (ft == 3)
		arr = &example3[0];
	else if (ft == 4)
		arr = &example4[0];
	else if (ft == 5)
		arr = &example5[0];
	else if (ft == 6)
		arr = &example6[0];
	else if (ft == 7)
		arr = &example7[0];
	else
		arr = &input[0];
	
	printf("= TEST =========================\n");

	//check if int datatype is OK
	printf("max   = %i\n", INT_MAX);
	printf("turnt = %i\n", maxturns);
	printf("Length of input is %i\n", arrlen(arr));

	pos		*positions;
	int		*allturns;
	
	positions = calloc(maxturns + 1, sizeof(*positions));
	if (positions == NULL)
		printf("Calloc pos failed");
	allturns = calloc(maxturns + 1, sizeof(*allturns));
	
	int		turns = 2020;
	int		t = 0;

	printarr(arr);
	
	printf("= PLAY PART1 ===================\n");
	while (t < turns)
	{
		if (t < arrlen(arr))
			allturns[t] = arr[t];
		else
			//allturns[t] = guess_age(allturns, t);
			allturns[t] = guess_fast(allturns, t, positions);
		fillpos(&positions[allturns[t]], t);
		t++;
	}
	printturns(allturns, 0, printlim);
	printturns(allturns, t - printlim, t);
	printf("PART1: Number for turn %i is %i\n", t, allturns[t - 1]);

	printf("= PLAY PART2 ===================\n");
	int		turns2target =	30000000;
	int		turns2 =		30000000;
	
	printf("turnt = %i\n", turns2target);
	printf("turns = %i\n", turns2);
	
	while (t < turns2)
	{
		allturns[t] = guess_fast(allturns, t, positions);
		fillpos(&positions[allturns[t]], t);
		t++;
	}
	printturns(allturns, turns, turns + printlim + 1);
	printturns(allturns, t - printlim + 1, t + 1);
	printf("PART2: Number %i spoken is %i\n", t, allturns[t - 1]);
	printarr(arr);
}

void fillpos(pos *posit, int turn)
{
	if (posit->called == 0)
	{
		posit->prev1 = turn;
		posit->called = 1;
	}
	else 
	{
		posit->prev2 = posit->prev1;
		posit->prev1 = turn;
		posit->called = 2;
	}
}

int	guess_age(int *tur, int t)
{
	int prev1 = t - 1;
	int	lookfor = tur[prev1];
	int	prev2 = prev1 - 1;
	
	while (prev2 >= 0)
	{
		if (lookfor == tur[prev2])
			return (prev1 - prev2);
		prev2--;
	}
	return (0);
}

int	guess_fast(int *tur, int t, pos *posit)
{
	int prev1 = t - 1;					// calc previous t (1 before this turn)
	int	lookfor = tur[prev1];			// what is the value there
	
	if (posit[lookfor].called != 2)	// if not a valid turn (e.g 1st val)
		return (0);
	else
		return (prev1 - posit[lookfor].prev2);
}
	
void	printturns(int *tur, int from, int to)
{
	int t = from;
	
	to = (to > maxturns) ? maxturns : to;

	while (t < to)
	{
		printf("Turn %-4i: %-4i\n", t + 1, tur[t]);
		t++;
	}
}

int	arrlen(int *arr)
{
	int	len = 0;

	while (arr[len] != -42)
		len++;
	return (len);
}

void printarr(int *arr)
{
	int		test = 0;
	printf("Print input array\n");
	while (test <= arrlen(arr))
	{
		printf("Num %i is %d\n", test + 1, arr[test]);
		test++;
	}
}

void printlookup(pos *pos, int s, int e)
{
	printf("Check lookup table\n");
	int		i = s;
	while (i < e)
	{
		printf("num %i called? %i\t(t =  %i & %i)\n", i, \
		pos[i].called, \
		pos[i].prev1, \
		pos[i].prev2);
		i++;
	}
}
