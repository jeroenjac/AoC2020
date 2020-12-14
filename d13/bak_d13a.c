#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int		main()
{
	FILE	*ptr = fopen("input.txt","r");
	int		earliest_departure;
	int		buses = 1;
	
	if (ptr == NULL)
	{
		printf("Can not read file");
		return 0;
	}
	fscanf(ptr, "%i", &earliest_departure);
	printf("dep = %i\n", earliest_departure);
	while (*ptr != '\n')
	{
		if (*ptr == ',')
			buses++;
	}
	printf("number of buses = %i\n", buses);

/*
	while (fscanf(ptr, "%i,", &earliest_departure) == 1)
	{
		printf("dep = %i\n", earliest_departure);
	}
*/	
	return (0);
}
