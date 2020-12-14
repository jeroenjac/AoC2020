#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>

#define MAXBUF 500

typedef struct ds
{
	int		*busID;
	int		*busPOS;
	int		buses;
	int		busIDs;
}	sched;

void		getdata(char *file, struct ds *ret);
int			getsize(char *file);
void		print_test(sched schema);

int			busIDdeparts(int ID, unsigned long time)
{
	if (time % ID == 0)
		return(1);
	return (0);
}

void		findMaxID(sched s, int *maxID, int *maxPOS, int *maxi)
{
	int		i = 0;
	
	*maxPOS = s.busID[i];
	*maxID = s.busID[*maxPOS];
	while (i < s.busIDs)
	{
		if (s.busID[i] > *maxID)
		{
			*maxPOS = s.busPOS[i];
			*maxID = s.busID[i];
			*maxi = i;
		}
		i++;
	}
}

int		main(int argc, char **argv)
{
	char	*file0 = "input.txt";
	char	*file1 = "example.txt";
	char	*file;
	int		ft = 0;
	struct	ds	s;
	struct	ds	*schema = &s;

	printf("= GET FILE & ALLOC MEM ======================\n");
	if (argc > 1)
		file = argv[1];
	else
		file = file0;
	schema->busIDs = getsize(file);
	schema->busID = calloc(schema->busIDs, sizeof(int));
	schema->busPOS = calloc(schema->busIDs, sizeof(int));
	if (schema->busID == NULL || schema->busPOS == NULL)
		printf("Some mem alloc failed\n");
	getdata(file, schema);
	print_test(*schema);
	printf("= START ANALYSIS ============================\n");
	int		maxID;
	int		maxPOS;
	int		i_maxID;
	findMaxID(*schema, &maxID, &maxPOS, &i_maxID);
	printf("Highest busID is %i on pos %i\n", maxID, maxPOS);
	//Find at which T this bus should depart ~ POS
	unsigned long tM = ULONG_MAX;
	printf("num   = %li\n", 100000000000000);
	printf("max   = %lu\n", tM);
	unsigned long tL = maxID;
	unsigned long t0, tX;
	int			i; // needed ??
	int			succes = 0;

	if (tL < maxPOS)
		tL += maxPOS;
	printf("first t0 = %lu\n", tL - maxPOS);
	int	imax = schema->busIDs;
	//imax = 5;
	while (succes == 0)
	{
		//check bus 0
		t0 = tL - maxPOS;
		//succes = busIDdeparts(schema->busID[0], t0);
		// try to do the same for other buses, e.g. t1, t2, etc.
		// best in a function, to make use of returning early
		i = 0;
		succes = 1;
		while (i < imax && succes == 1)
		{
			tX = t0 + schema->busPOS[i];
			printf("checking bus %i for t = %lu\n", i, tX);
			succes = busIDdeparts(schema->busID[i], tX);
			i++;
			if (i == i_maxID)
				i++;
		}
		if (succes != 1)
			tL += maxID;
		/*
		if (t0 % 100000 == 0)
			printf("Testing for t = %lu...\n", t0);
		*/
	}
	printf("At t0 = %lu (and +1 ..) all buses depart\n", t0);
	//For multiples of this T, get T' on which other buses should depart
	//Test if all depart
	//Check if t0/tL hits tM?
	
	printf("= END (FREE DATA) ===========================\n");
	free(schema->busID);
	free(schema->busPOS);
	return (0);
}

int			getsize(char *file)
{
	FILE		*in_file;
	char		str[MAXBUF];
	int			busIDs = 1;
	int			i = 0;
	
	in_file = fopen(file, "r");
	if (in_file == NULL)
		printf("File could not be read\n");
	
	fgets(str, MAXBUF, in_file);
	bzero(str, MAXBUF);
	fgets(str, MAXBUF, in_file);
	while (str[i] != '\0')
	{	
		if (isdigit(str[i]))
			busIDs++;
		while (isdigit(str[i]))
			i++;
		i++;
	}
	fclose(in_file);
	return (busIDs);
}


void	getdata(char *file, struct ds *ret)
{
	FILE		*in_file;
	char		str[MAXBUF];
	int			line = 0;
	int			i = 0, j = 0, k = 0;
	ret->buses = 1;

	in_file = fopen(file, "r");
	if (in_file == NULL)
		printf("File could not be read\n");
	
	fgets(str, MAXBUF, in_file);
	bzero(str, MAXBUF);
	fgets(str, MAXBUF, in_file);
	while (str[i] != '\0')
	{	
		while (isdigit(str[i]))
			i++;
		if (str[i] == ',')
			ret->buses++;
		i++;
	}
	i = 0;
	j = 0;
	k = 0;
	while (str[i] != '\0')
	{
		if (isdigit(str[i]))
		{
			ret->busID[j] = atoi(&str[i]);
			ret->busPOS[j] = k;
			j++;
			k++;
			while (isdigit(str[i]))
				i++;
		}
		else if (str[i] == ',')
			k++;
		i++;
	}
	fclose(in_file);
}

void	print_test(sched schema)
{
	int		i = 0;
	
	printf("= PRINT INPUT ===============================\n");
	printf("%i buses & %i with ID\n", schema.buses, schema.busIDs);
	while (i < schema.busIDs - 1)
	{
		printf("Bus %-2i on pos %-2i has ID %-2i\n", i, \
		schema.busPOS[i], schema.busID[i]);
		i++;
	}
}
