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
	int		numq;		// maybe this is easier
	int		numqvalid[20];
}	rule;

typedef struct ticket
{
	int		val[20];	//based on ticket size ~ nrules
	int		invalid;	//set to 1 for invalid tickets
}	ticket;

//struct for meta??

void	fill_rules_q(rule *set, int Xrules, ticket *tics, int numtics);

int		fillruleX(rule *set, int ruleX, int nrules, int q);
int		check_X_rules_alltickets(rule *set, int Xrules);
int		check_rule_alltickets(rule check, ticket *tics, int numtics);
int		is_q_used(rule *set, int question, int nrules);
int		find_first_unass_rule(rule *set, int nrules);

void	initmapping(rule *set, int nrules, int from);
void	printmatrix_rule_q_valid(rule *set, int nrules);
void	printmapping(rule *set, int nrules);

int		check_val_rule(int value, rule check);

#include "part1_reuse.c"

int		main(int argc, char **argv)
{
	char	*filename;
	rule	*set;
	int		nrules = 20;
	int		ntickets = 269 - 25;
	ticket	*tics;

	//int		myticket[20] = {109,101,79,127,71,59,67,61,173,157,163,103,83,97,73,167,53,107,89,131};
	
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
	printrules(set, 2);
	printtickets(tics[1], 1, nrules);
	printtickets(tics[ntickets - 1], ntickets - 1, nrules);
	printf("= CHECK VALID (PT1) ==========\n");
	int errorrate = 0;

	errorrate = check_all_tickets(tics, set, nrules, ntickets);
	printf("part1: error rate = %i (correct answer: 23122)\n", errorrate);
	printf("= ANALYSE (PT2) ==============\n");
	int	cnt_invalid = 0;
	int i = 0;
	while (i < ntickets)
	{
		cnt_invalid += tics[i].invalid;
		i++;
	}
	printf("There are %i tickets invalid\n", cnt_invalid);

	initmapping(set, nrules, 0);
	printmapping(set, nrules);
	printmatrix_rule_q_valid(set, nrules);
	fill_rules_q(set, 20, tics, ntickets);
	printmatrix_rule_q_valid(set, nrules);
	
	printf("Testing the find av_q_rule function\n");
	//set[0].numq = 1;
	//set[1].numq = 0;
	//printf("First blank rule (ordered) = %i\n", find_first_unass_rule(set, nrules));
	printf("Is %i used? %i\n", 0, is_q_used(set, 0, nrules));
	printf("Is %i used? %i\n", 1, is_q_used(set, 1, nrules));
	printf("Is %i used? %i\n", 2, is_q_used(set, 2, nrules));
	int	safeconfig = 0;

	safeconfig = check_X_rules_alltickets(set, 20);
	printf("Start: safeconfig = %i\n", safeconfig);

	//process
	// 1 - fill first unassigned qnum with lowest available [0-19]
			// 1a - determine first unassigned [ != 99 ? ]
			// 1b - find lowest available [0 - 19] 

	//init mp with name, rule number?? and valpos (set to 99 as default?)
	//Create 'driver function'
		//try rule0.valpos = 1 --> check if there is an error --> if error then increment
	
	//while loop for X rules. If X becomes 21. Succes (or not)
	// check undo - back to 99 if failed?

	safeconfig = fillruleX(set, 0, nrules, 0);
	printf("Recursion result: safeconfig = %i\n", safeconfig);
	printmapping(set, nrules);
	safeconfig = check_X_rules_alltickets(set, 20);
	printf("Double check: safeconfig = %i\n", safeconfig);

	free(set);
	free(tics);
	return (0);
}

//check for a question if it is used (1) or not (0)
//int		is_q_used(rule *set, int question, int nrules)

//THE RECUR FUNCTION
// base case = safe config for all rules (e.g. Xrules = 20 && safe = 0)
int		fillruleX(rule *set, int ruleX, int nrules, int q)
{
	int	qnum = q;
	
	if (ruleX == 20)
		return (0);
	while (is_q_used(set, qnum, nrules) == 1)
		qnum++;									//increment q when used
	printf("Trying to map rule %i on q = %i\n", ruleX, qnum);
	if (qnum < nrules)
	{
		set[ruleX].numq = qnum;
		if (check_X_rules_alltickets(set, ruleX) == 0)
			fillruleX(set, ruleX + 1, nrules, 0);
		else
			fillruleX(set, ruleX, nrules, q + 1);
	}
	set[ruleX].numq = 99;						//back to init value
	return (1);
}

//FILL RULE-Q MATRIX
void	fill_rules_q(rule *set, int Xrules, ticket *tics, int numtics)
{
	int rule = 0;
	int	rule_gives_error;
	int	q = 0;

	while (q < 20)
	{
		rule = 0;
		while (rule < Xrules)
		{
			set[rule].numq = q;
			rule_gives_error = check_rule_alltickets(set[rule], tics, numtics);
			if (rule_gives_error == 1)
				set[rule].numqvalid[q] = 4;	
			else
				set[rule].numqvalid[q] = 0;
			rule++;
		}
		q++;
	}
}

//returns 0 if SAFE (no rule/ticket combi has a conflict). Else 1.
int		check_X_rules_alltickets(rule *set, int Xrules)
{
	int rule = 0;
	int	q;

	while (rule < Xrules)
	{
		q = set[rule].numq;
		if (q >= 20)						//this should not happen
			return (404);
		if (set[rule].numqvalid[q] == 4)		// earlier set value (error) -> not safe
			return (1);
		else									// if not init value, and not 2, it is 1 -> check next
			rule++;
	}
	return (0);
}

//returns 0 if SAFE, otherwise 1.
int		check_rule_alltickets(rule check, ticket *tics, int numtics)
{
	int	tic = 0;
	int	ticval = -101;
	int	invalidticket = 0;

	while (tic < numtics)
	{
		if (tics[tic].invalid != 1)
		{
			ticval = tics[tic].val[check.numq];
			invalidticket = check_val_rule(ticval, check);
			if (invalidticket == 0)
				return (1);
		}
		tic++;
	}
	return (0);
}
//returns 1 if value is valid acc to THIS rule, otherwise 0
//int		check_val_rule(int value, rule check)


//check for a question if it is used (1) or not (0)
int		is_q_used(rule *set, int question, int nrules)
{
	int	rule = 0;

	while (rule < nrules)		//checking for all rules, but no problem since init (back) to 99
	{
		if (question == set[rule].numq)
				return (1);
		rule++;
	}
	return (0);
}

int		find_first_unass_rule(rule *set, int nrules)
{
	int rulenum = 0;

	while (set[rulenum].numq != 99 && rulenum < nrules)
		rulenum++;
	return (rulenum);
}

//init function, currently handling both options
void	initmapping(rule *set, int nrules, int from)
{
	int i = from;

	while (i < nrules)
	{
		set[i].numq = 99;
		i++;
	}
}

void	printmatrix_rule_q_valid(rule *set, int nrules)
{
	int	i = 0;
	int	j;

	while (i < nrules)
	{
		printf("Rule %-2i: ", i);
		j = 0;
		while (j < nrules)
		{
			printf("%i ", set[i].numqvalid[j]);
			j++;
		}
		printf("\n");
		i++;
	}
}

void	printmapping(rule *set, int nrules)
{
	int i = 0;
	
	while (i < nrules)
	{
		printf("rule for: %-20s >> question %i\n", set[i].name, set[i].numq);
		i++;
	}
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
