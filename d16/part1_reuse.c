void	readtickets(char *fn, ticket *tics, int startline, int endline);
void	readrules(char *fn, rule *set, int startline, int endline);
void	printrules(rule *set, int number);
void	printtickets(ticket tic, int number, int ntickets);
int		check_all_tickets(ticket *tic, rule *set, int nrules, int ntickets);	//not used
int		check_1ticket_all_vals(ticket tic, rule *set, int nrules);				//not used
int		check_val_allrules(int value, rule *set, int nrules);					//not used

int		check_all_tickets(ticket *tics, rule *set, int nrules, int ntickets)
{
	int ticket = 0;
	int	tot_error = 0;
	int	error;

	while (ticket < ntickets)
	{
		error = check_1ticket_all_vals(tics[ticket], set, nrules);
		if (error > 0)
			tics[ticket].invalid = 1;
		tot_error += error;
		ticket++;
	}
	return (tot_error);
}

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
