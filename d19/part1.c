/*

APPROACH:
1. Store data in a good structure, likely linked.
2. Iterate through the different input strings
3. Find rule 0.
4. For every string, check rule 0.
5. Expand rule 0 - this generates 2^122 options...
		Other way around: start from "a", how many steps to 0?
		a (116) = 0
		47 = 1
		103 = 2
		71 = 3
		10 = 4
		100 = 5
		42 = 6
		8 = 0

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct s_rule
{
	char			*expr;
	struct s_rule	*r1a;
	struct s_rule	*r1b;
	struct s_rule	*r2a;
	struct s_rule	*r2b;

}	rule;

void loadexample(rule *rules);

int	main(void)
{
	rule	*rules;

	rules = calloc(sizeof(rule), 7);
	loadexample(rules);

	rule	*print;

	print = rules + 6;
	printf("expr print: %s\n", print->expr);
	while (print->expr == NULL)
	{
		print = print->r1a;
		printf("expr print: %s\n", print->expr);
	}

	return (0);
}

//Done manually, this could be automated with a parse function
void loadexample(rule *rules)
{
	rules[7].expr = NULL;
	rules[7].r1a = &rules[8];		//experiment
	
	rules[8].expr = NULL;
	rules[8].r1a = &rules[4];		//experiment
	
	rules[0].expr = NULL;
	rules[0].r1a = &rules[6];
	rules[0].r1b = &rules[5];
	rules[0].r2a = NULL;
	rules[0].r2a = NULL;
	
	rules[6].expr = NULL;
	rules[6].r1a = &rules[7];		//adjusted
	rules[6].r1b = &rules[1];
	rules[6].r2a = NULL;
	rules[6].r2a = NULL;
	
	rules[1].expr = NULL;
	rules[1].r1a = &rules[2];
	rules[1].r1b = &rules[3];
	rules[1].r2a = &rules[3];
	rules[1].r2a = &rules[2];
	
	rules[2].expr = NULL;
	rules[2].r1a = &rules[4];
	rules[2].r1b = &rules[4];
	rules[2].r2a = &rules[5];
	rules[2].r2a = &rules[5];
	
	rules[3].expr = NULL;
	rules[3].r1a = &rules[4];
	rules[3].r1b = &rules[5];
	rules[3].r2a = &rules[5];
	rules[3].r2a = &rules[4];
	
	rules[4].expr = strdup("a");
	rules[4].r1a = NULL;
	rules[4].r1b = NULL;
	rules[4].r2a = NULL;
	rules[4].r2a = NULL;;
	
	rules[5].expr = strdup("b");
	rules[5].r1a = NULL;
	rules[5].r1b = NULL;
	rules[5].r2a = NULL;
	rules[5].r2a = NULL;
}
