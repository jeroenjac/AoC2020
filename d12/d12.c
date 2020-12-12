/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   d12.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jjacobs <jjacobs@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/12 14:17:43 by jjacobs       #+#    #+#                 */
/*   Updated: 2020/12/12 14:26:20 by jjacobs       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define	MAXBUF 30
 
typedef struct	ds
{
	char	str[MAXBUF];
	int		val;			//	Steps to take for this instruction
	int		dir;			//	Direction of the ship (0 - 360); 0 = North, etc
	int		legdir;			//	Direction for this instruction (0 - 360); 0 = North, etc
	int		x;				//	x after the instruction
	int		y;				//	y after the instruction
}			step;

void	getdata(char *file, int lines, struct ds *groups);
int		getmeta(char *file, int *maxline);

int		main(int argc, char **argv)
{
	int		ft = 0;
	char	*file0 = "input.txt";
	char	*file1 = "example.txt";
	char	*file;

	if (argc > 1)
		ft = atoi(argv[1]);
	if (ft == 1)
		file = file1;
	else
		file = file0;

	printf("= INPUT CHECK ================================================\n");
	int		lines;
	int		maxline;

	lines = getmeta(file, &maxline);
	printf("Number of lines: %i\n", lines);
	printf("(Max) line length: %i\n", maxline);;

	printf("= ALLOC MEMORY & READING DATA ================================\n");

	printf("Allocating data array...\n");
	step *data;
	data = calloc(lines, sizeof(step));
	if (data == NULL)
		printf("Allocation for data array failed");
		
	printf("Start reading input...\n");
	data[0].dir = 90;
	data[0].x = 0;
	data[0].y = 0;
	getdata(file, lines, data + 1);
	
	printf("= CHECKS EXTRA ===============================================\n");
	printf("= PRINT TEST DATA ============================================\n");
	int	lastprint = 5;
	int i = 0;

	if (argc > 2)
		lastprint = atoi(argv[2]);
	
	while (i < lastprint)
	{
		printf("inst %i = \"%-4s\": head = %-3i dir = %-3i. NewPos = (%i, %i)\n", i, data[i].str, \
		data[i].dir, data[i].legdir, data[i].x, data[i].y);
		i++;
	}

	printf("= PT1 ANALYSIS ===============================================\n");
	int		a1 = 0;
	int		fin_x = data[lines].x;
	int		fin_y = data[lines].y;

	a1 += (fin_x > 0) ? fin_x : -fin_x;
	a1 += (fin_y > 0) ? fin_y : -fin_y;
	
	printf("Answer part 1 = %i\n", a1);

	//1991 is too high
	//1975 is too high
	//631 is too high
	//441 is correct
		
	printf("= PT2 ANALYSIS ===============================================\n");
	long long		a2 = 0;

	printf("Answer part 2 = %lli\n", a2);
	
	//Freeing data
	free(data);
	printf("= END ========================================================\n");
	return (0);
}

/*
** Functions 'test reads' the data and returns number of lines. Also via pointer, it can provide
** + stores number of 'parts/files' (for example sep. by newline)
** + stores max line length
*/

int		getmeta(char *file, int *ptmaxline)
{
	FILE	*in_file = fopen(file, "r");
	
	if (in_file == NULL)
		printf("File read failed\n");
	
	char	line[MAXBUF];
	char	*lpt;
	int		lines = 0;
	int		linesize;

	*ptmaxline = linesize;;
	while (fgets(line, sizeof(line), in_file) != NULL)
	{
		lines++;
		lpt = line;
		linesize = 0;
		while (*lpt != '\n' && lpt++ != NULL)
			linesize++;
		if (linesize > *ptmaxline)
			*ptmaxline = linesize;
	}
	fclose(in_file);

	return (lines);
}
void	calc_position(step *legs, int legnum)
{
	int	x_old = legs[legnum - 1].x;
	int	y_old = legs[legnum - 1].y;
	int	*x_new = &legs[legnum].x;
	int	*y_new = &legs[legnum].y;
	int	steps = legs[legnum].val;
	int	dir = legs[legnum].legdir;
	
	*x_new = x_old;
	*y_new = y_old;
	
	if (dir % 180 == 0)
		*y_new += (dir == 0) ? steps : -steps;
	else
		*x_new += (dir == 90) ? steps : -steps;	
}

void		getdata(char *file, int lines, struct ds *dat)
{
	FILE	*in_file;
	char	str[MAXBUF];
	int		line = 0;
	int		turn;

	in_file = fopen(file, "r");
	if (in_file == NULL)
		printf("File read failed\n");
	
	while (line < lines && fgets(str, MAXBUF, in_file) != NULL)
	{	
		turn = 0;
		//printf("reading: %s", str);
		strncpy(dat[line].str, str, strchr(str, '\n') - str);
		if (str[0] == 'L' || str[0] == 'R' )			//Case 1a: instruction is turn L / R
		{
			turn = (str[0] == 'L') ? -1 : 1;
			turn *= atoi(str + 1);
			dat[line].val = 0;
		}
		else if (str[0] == 'F')							//Case 1b: instruction is F
		{
			dat[line].legdir = dat[line - 1].dir;
			dat[line].val = atoi (str + 1);
		}
		else											//Case 2: instruction is move N / E / S / W.
		{
			if (str[0] == 'N')
				dat[line].legdir = 0;
			else if (str[0] == 'E')
				dat[line].legdir = 90;
			else if (str[0] == 'S')
				dat[line].legdir = 180;
			else if (str[0] == 'W')
				dat[line].legdir = 270;
			dat[line].val = atoi(str + 1);
		}
		dat[line].dir = (dat[line - 1].dir + turn + 360) % 360;
		calc_position(dat, line);
		line++;
	}
	fclose(in_file);
}
