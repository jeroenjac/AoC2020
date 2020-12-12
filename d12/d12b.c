/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   d12.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jjacobs <jjacobs@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/12 14:17:43 by jjacobs       #+#    #+#                 */
/*   Updated: 2020/12/12 20:22:17 by jjacobs       ########   odam.nl         */
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
	int		wpx;
	int		wpy;
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
	step *data2;
	data2 = calloc(lines, sizeof(step));
	if (data2 == NULL)
		printf("Allocation for data array failed");

	printf("Start reading input (pt 2)...\n");
	data2[0].wpx = 10;
	data2[0].wpy = 1;
	getdata(file, lines, data2 + 1);
	
	printf("= CHECKS EXTRA ===============================================\n");
	printf("= PRINT TEST DATA ============================================\n");
	int	lastprint = 5;
	int i = 0;

	if (argc > 2)
		lastprint = atoi(argv[2]);
	
	printf("= PRINT TEST DATA (PT2) ======================================\n");
	i = 0;
	while (i < lastprint)
	{
		printf("inst %i = \"%-4s\" (val = %-2i) -> NewPos = (%-3i, %-3i) -> NextWP = (%-3i, %-3i)\n", i, data2[i].str, \
		data2[i].val, data2[i].x, data2[i].y, data2[i].wpx, data2[i].wpy);
		i++;
	}

	printf("= PT2 ANALYSIS ===============================================\n");
	int		a2 = 0;	

	int fin_x = data2[lines].x;
	int fin_y = data2[lines].y;
	a2 += (fin_x > 0) ? fin_x : -fin_x;
	a2 += (fin_y > 0) ? fin_y : -fin_y;
	printf("Answer part 2 = %i\n", a2);
	
	//Freeing data
	free(data2);
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
	int		linesize = -1;

	*ptmaxline = linesize;
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
void	move_to_wp_steps(step *legs, int legnum)
{
	int	x_old = legs[legnum - 1].x;
	int	y_old = legs[legnum - 1].y;
	int	steps = legs[legnum].val;

	legs[legnum].x = x_old + legs[legnum].wpx * steps;
	legs[legnum].y = y_old + legs[legnum].wpy * steps;
}

step	rotate_wp(step leg)
{
	int		wpx0 = leg.wpx;
	int		wpy0 = leg.wpy;
	step	ret = leg;
	int		rot;	
	int		cosZ;
	int		sinZ;

	rot = (leg.str[0] == 'R') ? 1 : -1;
	rot = leg.val * rot + 360;
	if (rot % 180 == 0)
	{
		cosZ = (rot % 360 == 0) ? 1 : -1;
		sinZ = 0;
	}
	if (rot % 180 == 90)
	{
		cosZ = 0;
		sinZ = (rot % 360 == 90) ? -1 : 1;
	}
	
	ret.wpx = wpx0 * cosZ - wpy0 * sinZ;
	ret.wpy = wpx0 * sinZ + wpy0 * cosZ;

	return (ret);
}

step	move_wp(step leg)
{
	char	dir = leg.str[0];
	int		steps = leg.val;
	step	ret = leg;
	
	if (dir == 'E' || dir == 'W')
		ret.wpx += (dir == 'E') ? steps : -steps;
	else if (dir == 'N' || dir == 'S')
		ret.wpy += (dir == 'N') ? steps : -steps;
	return (ret);
}

// To do
// Split the 2nd part off, sep for pt 1 and pt 2.

void		getdata(char *file, int lines, struct ds *dat)
{
	FILE	*in_file;
	char	str[MAXBUF];
	int		line = 0;

	in_file = fopen(file, "r");
	if (in_file == NULL)
		printf("File read failed\n");
	
	while (line < lines && fgets(str, MAXBUF, in_file) != NULL)
	{	
		//printf("reading: %s", str);
		dat[line] = dat[line - 1];								// initialize step N = step N - 1;
		bzero(dat[line].str, 3);								// remove old bytes from str
		strncpy(dat[line].str, str, strchr(str, '\n') - str);	// store new instrucion - can be handy / not req;
		dat[line].val = atoi(str + 1);							// get arg value
		if (str[0] == 'F')
			move_to_wp_steps(dat, line);
		if (str[0] == 'L' || str[0] == 'R')
			dat[line] = rotate_wp(dat[line]);
		if (str[0] == 'N' || str[0] == 'E' || str[0] == 'S' || str[0] == 'W')
			dat[line] = move_wp(dat[line]);
		line++;
	}
	fclose(in_file);
}
