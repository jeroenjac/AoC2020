#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define	maxline 100

typedef struct rule
{
	char	name[40];
	int		min1;
	int		max1;
	int		min2;
	int		max2;
}	rule;

typedef struct ticket
{
	int		val[20];	//based on ticket size ~ nrules
	int		invalid;	//set to 1 for invalid tickets
}	ticket;

//struct for meta??

void	readtickets(char *fn, ticket *tics, int startline, int endline);
void	readrules(char *fn, rule *set, int startline, int endline);
void	printrules(rule *set, int number);
void	printtickets(ticket tic, int number, int ntickets);

int		check_all_tickets(ticket *tic, rule *set, int nrules, int ntickets);
int		check_1ticket_all_vals(ticket tic, rule *set, int nrules);
int		check_val_allrules(int value, rule *set, int nrules);
int		check_val_rule(int value, rule checkrule);

int		main(int argc, char **argv)
{
	char	*filename;
	rule	*set;
	int		nrules = 20;
	int		ntickets = 269 - 25;
	ticket	*tics;
	
	printf("= GET DATA ===================\n");
	filename = (argc == 1) ? strdup("input.txt") : argv[1];
	set = calloc(nrules, sizeof(*set));
	if (set == NULL)
		printf("Calloc 1 failed\n");
	tics = calloc(ntickets, sizeof(*tics));
	if (tics == NULL)
		printf("Calloc 2 failed\n");

	readrules(filename, set, 0, nrules - 1);
	readtickets(filename, tics, 25, 268);
	
	
	printf("= TEST PRINT =================\n");
	printrules(set, 5);
	printtickets(tics[1], 1, nrules);
	printtickets(tics[2], 2, nrules);
	printtickets(tics[ntickets - 1], ntickets - 1, nrules);
	printf("= CHECK VALID (PT1)===========\n");

	int errorrate = 0;

	errorrate = check_all_tickets(tics, set, nrules, ntickets);
	printf("part1: error rate = %i\n", errorrate);
	
	return (0);
}

int		check_all_tickets(ticket *tics, rule *set, int nrules, int ntickets)
{
	int ticket = 0;
	int	tot_error = 0;

	while (ticket < ntickets)
	{
		tot_error += check_1ticket_all_vals(tics[ticket], set, nrules);
		ticket++;
	}
	return (tot_error);
}

//check all values of a ticket
// ASSUMPTION1: ticket can have 2 errors)
// ASSUMPTION2: # of rules == # of field in ticket
int		check_1ticket_all_vals(ticket tic, rule *set, int nrules)
{
	int	error = 0;
	int	field = 0;

	while (field < nrules)
	{
		error += check_val_allrules(tic.val[field], set, nrules);
		field++;
	}
	return (error);
}

//returns 0 if value is valid acc to ANY rule, otherwise value
int		check_val_allrules(int value, rule *set, int nrules)
{
	int	rule = 0;
	int	valid = 0;

	while (valid == 0 && rule < nrules)
	{
		valid = check_val_rule(value, set[rule]);
		if (valid == 1)
			return (0);
		rule++;
	}
	return (value);
}

//returns 1 if value is valid acc to THIS rule, otherwise 0
int		check_val_rule(int value, rule check)
{
	if (value >= check.min1 && value <= check.max1)
		return (1);
	if (value >= check.min2 && value <= check.max2)
		return (1);
	return (0);	
}

void	readtickets(char *fn, ticket *tics, int startline, int endline)
{
	FILE	*file = fopen(fn, "r");
	int		lc = 0;
	int		tc = 0;
	char	*pt;
	int		i;

	if (file != NULL)
	{
		char line[maxline];
		while (fgets(line, sizeof(line), file) != NULL)
		{
			if (lc >= startline && lc <= endline)
			{
				pt = line;
//				printf("Reading: %s", pt);
				i = 0;
				while (pt != NULL && *pt != '\n')
				{
					tics[tc].val[i] = atoi(pt);
					pt = strchr(pt, ',');
					if (pt != NULL)
						pt++;
					i++;
				}
				tc++;
			}
			lc++;
		}
		fclose(file);
	}
	else
		printf("Wrong filename\n");
}

void	readrules(char *fn, rule *set, int startline, int endline)
{
	FILE	*file = fopen(fn, "r");
	int		cnt = 0;
	char	*pt;

	if (file != NULL)
	{
		char line[maxline];
		while (fgets(line, sizeof(line), file) != NULL)
		{
			if (cnt >= startline && cnt <= endline)
			{
//				printf("Reading: %s", line);
				pt = strchr(line, ':');
				strncpy(set[cnt].name, line, pt - line);
				set[cnt].min1 = atoi(pt + 2);
				pt = strchr(pt, '-') + 1;
				set[cnt].max1 = atoi(pt);
				pt = strstr(pt, "or ") + 3;
				set[cnt].min2 = atoi(pt);
				pt = strchr(pt, '-') + 1;
				set[cnt].max2 = atoi(pt);
			}
			cnt++;
		}
		fclose(file);
	}
	else
		printf("Wrong filename\n");
}

void	printrules(rule *set, int number)
{
	int		i = 0;

	while (i < number)
	{
		printf("rule %i: \"%s\" in range (%i-%i) or (%i-%i)\n", \
		i, set[i].name, set[i].min1, set[i].max1, set[i].min2, set[i].max2);
		i++;
	}
}

void	printtickets(ticket tic, int ticnum, int numbers)
{
	int	j = 0;

	printf("ticket %-3i:", ticnum);
	while (tic.val[j] > 0 && j < numbers)
	{
		printf("%i-", tic.val[j]);
		j++;
	}
	printf("\n");
}
