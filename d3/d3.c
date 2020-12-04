#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

#define	MAXBUF 200

void	printnumfield(int **numfield, int ymax, int xmax);
void	printrawdata(char **field, int ymax, int xmax);

int	sledge(int **arr, char **carr, int move_y, int move_x, int ymax, int xmax)
{
	int	trees = 0;
	int	y_pos = 0;
	int	x_pos = 0;
	int	run = 0;

	while (y_pos < ymax)
	{
		carr[y_pos][x_pos % xmax] = 'O';
		//printf("RUN %i (%i trees):\n", run, trees);
		//printrawdata(carr, ymax, xmax);
		trees = trees + arr[y_pos][x_pos % xmax];
		y_pos = y_pos + move_y;
		x_pos = x_pos + move_x;
		run++;
	}
	return (trees);
}

int		main(void)
{
	char	*file = "input.txt";
	FILE	*in_file;
	char	line[MAXBUF];

	printf("==== CHECKS AND BALANCES ====\n");
	//Step 1: Getting number of lines + length of line
	in_file = fopen(file, "r");
	if (in_file == NULL)
		printf("File read failed\n");
	
	int		n_lines = 0;
	while (fgets(line, sizeof(line), in_file) != NULL)
		n_lines++;
	fclose(in_file);

	printf("Number of lines: %i\n", n_lines);
	
	int xmax = 0;
	while (line[xmax] != '\n')
		xmax++;
	printf("Length of line: %i\n", xmax);
	

	//Step 2: Allocating array
	char	*field[n_lines];
	int		*numfield[n_lines];

	int y = 0;
	int x = 0;
	while (y < n_lines)
	{
		numfield[y] = (int *)calloc(xmax, sizeof(int));
		field[y] = (char *)calloc(xmax, sizeof(char));
		if (numfield[y] == NULL || field[y] == NULL)
			printf("Alloc %i failed\n", y);
		//printf("Alloc %i ok\n", y);
		y++;
	}

	char	str[MAXBUF];
	char	*pt;
	int		j = 0;
	char	tree = '#';
	char	open = '.';

	//Step 3: Reading file per MAXBUF chars
	in_file = fopen(file, "r");
	if (in_file == NULL)
		printf("File read failed\n");
	int i = 0;
	while (fgets(str, MAXBUF, in_file) != NULL)
	{
		pt = str;
		j = 0;
		//printf("reading: %s\n", str);
		while (*pt != '\n')
		{
			field[i][j] = *pt;
			if (*pt == open)
				numfield[i][j] = 0;
			if (*pt == tree)
				numfield[i][j] = 1;
			pt++;
			j++;
		}
		pt++;
		i++;
	}
	//Step 4: Validate if reading data is succesfull
	printrawdata(field, n_lines, xmax);
	printnumfield(numfield, n_lines, xmax);
	printf(">>>>>> Test reads\n");
	x = 1;
	y = 4;
	printf("field[%i][%i] = %c\n", y, x, field[y][x]);
	printf("numfield[%i][%i] = %i\n", y, x, numfield[y][x]);
	x = 30;
	y = 1;
	printf("field[%i][%i] = %c\n", y, x, field[y][x]);
	printf("numfield[%i][%i] = %i\n", y, x, numfield[y][x]);
	//x = 9;
	//y = 322;
	//printf("field[%i][%i] = %c\n", y, x, field[y][x]);
	//printf("numfield[%i][%i] = %i\n", y, x, numfield[y][x]);

	//Step 5: Solving
	printf(">>>>>> Solution (pt 1)\n");
	printf("Sledge sees %i trees\n", sledge(numfield, field, 1, 3, n_lines, xmax));
		
	printf(">>>>>> Solution (pt 2)\n");
	int		right = 1;
	int		down = 1;
	int		trees2[5];
	int		run = 0;
	int		mv[5][2] = {{1, 1}, {3, 1}, {5, 1}, {7, 1}, {1, 2}};
	
	while (run < 5)
	{
		trees2[run] = sledge(numfield, field, mv[run][1], mv[run][0],\
		n_lines, xmax);
		printf("Run %i: %i trees\n", run, trees2[run]);
		run++;
	}
	run = 0;
	long	answer = 1;
	while (run < 5)
	{
		answer = answer * trees2[run];
		run++;
	}
	printf("Final answer = %li\n", answer);
	
	//printf(">>>>>> DEBUG\n");
	//printrawdata(field, n_lines, xmax);
	//printnumfield(numfield, n_lines, xmax);

	return (0);

}

void	printrawdata(char **field, int ymax, int xmax)
{
	printf("Raw data\n");
	int y = 0;
	int x = 0;
	while(y < ymax)
	{
		x = 0;
		while(x < xmax)
		{
			printf("%c", field[y][x]);
			x++;
		}
		printf("\n");
		y++;
	}
}

void	printnumfield(int **numfield, int ymax, int xmax)
{
	printf("Num data\n");
	int y = 0;
	int x = 0;
	while(y < ymax)
	{
		x = 0;
		while(x < xmax)
		{
			printf("%i", numfield[y][x]);
			x++;
		}
		printf("\n");
		y++;
	}
}
