#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXBUF 100;

int		busIDdeparts(int ID, int time)
{
	if (time % ID == 0)
		return(1);
	return (0);
}

int		main(int argc, char **argv)
{

	int		nbus = 9;
	int	buses[9] = {37, 41, 457, 13, 17, 23, 29, 431, 190};
	int		earl_dep = 1001796;
//	int		buses[5] = {7, 13, 59, 31, 19};
//	int		earl_dep = 939;
	int		departure = earl_dep - 1;
	int		bus;
	int		departs = 0;

	while (departs == 0)
	{
		departure++;
		bus = -1;
		while (bus < nbus && departs == 0)
		{
			bus++;
			departs = busIDdeparts(buses[bus], departure);
		}
	}
	printf("Bus %i departs at %i\n", buses[bus], departure);
	printf("Puzzle answer = %i\n", (departure - earl_dep) * buses[bus]);
	// 3879 is too high - offset issue
	// 130 is too low - did not use all data
	// 361 is correct
	return (0);
}

/*
int		getdata(char *file, struct ds *dat)
{
	FILE	*in_file;
	char	str[MAXBUF];
	int		line = 0;

	in_file = fopen(file, "r");
	if (in_file == NULL)
		printf("File could not be read\n");
	
	while (fgets(str, MAXBUF, in_file != NULL))
	{
		return (1);		
	}
}
*/
