#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define	MAXBUF 200
 
typedef struct	ds
{
	char	**seats;
	int		rows;
	int		rowlen;
}			dat;

void	getdata(char *file, struct ds data);
int		getnumberoflines(char *file, int *ptparts, int *maxline);

char **alloc_seat_mem(int rows, int rowlen)
{
	char	**ret;
	int		line = 0;
	
	ret = calloc(rows + 2, sizeof(char *));
	if (ret == NULL)
		printf("Allocation for rows failed\n");
	while (line < rows + 2)
	{
		ret[line] = calloc(rowlen + 2, sizeof(char));
		if (ret[line] == NULL)
			printf("Allocation for row %i failed\n", line);
		line++;
	}
	return (ret);
}

void	copydat(dat *to, dat *from)
{
	int y = 0;
	int x;

	to->rows = from->rows;
	to->rowlen = from->rowlen;

	while (y < to->rows + 2)
	{
		x = 0;
		while (x < to->rowlen + 2)
		{
			to->seats[y][x] = from->seats[y][x];
			x++;
		}
		y++;
	}
}

void	printgrid(char **area, int rows, int rowlen)
{
	int	row = 0;
	int	seat;

	while (row < rows + 2)
	{
		seat = 0;
		while (seat < rowlen + 2)
		{
			printf("%c", area[row][seat]);
			seat++;
		}
		printf("\n");
		row++;
	}
}

int		cnt_adj_occ(int posy, int posx, dat datc)
{
	int ct = 0;
	int	y = posy - 1;
	int x;
	
	if (posx == 0 || posx == datc.rowlen + 1 || posy == 0 || posy == datc.rows + 1)
		printf("error input coord for cnt_adj_occ (%i, %i)\n", posy, posx);
	while (y <= posy + 1)
	{
		x = posx - 1;
		while (x <= posx + 1)
		{
			if (!(x == posx && y == posy) && datc.seats[y][x] == '#')
				ct++;
			x++;
		}
		y++;
	}
	return (ct);
}

// To do: make steps = 1 or more variable. So it can be used for pt 1.
// And due to boundary condition check, get rid of the extra ..... around the area.

int		steps_in_dir(int y_start, int x_start, int y_step, int x_step, dat datc)
{
	int y_lim = datc.rows;
	int x_lim = datc.rowlen;
	int y = y_start;
	int x = x_start;

	if (y_start < 1 || y_start > datc.rows || x_start < 1 || x_start > datc.rowlen)
	{
		printf("Error for (%i, %i)\n", y, x);
		return (-1e6);
	}
	while (y > 0 && x > 0 && y <= y_lim && x <= x_lim )
	{
		y = y + y_step;
		x = x + x_step;
		if (datc.seats[y][x] == '#')
			return (1);
		if (datc.seats[y][x] == 'L')
			return (0);
	}
	return (0);
}
int		cnt_see_occ(int y_check, int x_check, dat datc)
{
	int	occ = 0;

	//checking for y
	occ += steps_in_dir(y_check, x_check, 1, 0, datc);
	occ += steps_in_dir(y_check, x_check, -1, 0, datc);
	occ += steps_in_dir(y_check, x_check, 0, 1, datc);
	occ += steps_in_dir(y_check, x_check, 0, -1, datc);
	occ += steps_in_dir(y_check, x_check, 1, 1, datc);
	occ += steps_in_dir(y_check, x_check, -1, -1, datc);
	occ += steps_in_dir(y_check, x_check, 1, -1, datc);
	occ += steps_in_dir(y_check, x_check, -1, 1, datc);

	return (occ);
}

int		checkseat(int y, int x, dat dato, dat datn, int pt)
{
	char	cur = dato.seats[y][x];
	char	*update = &datn.seats[y][x];
	
	if (cur  == '.')
		return (0);
	else if (pt == 1 && cur == 'L' && cnt_adj_occ(y, x, dato) == 0)
		*update = '#';
	else if (pt == 1 && cur == '#' && cnt_adj_occ(y, x, dato) >= 4)
		*update  = 'L';
	else if (pt == 2 && cur == 'L' && cnt_see_occ(y, x, dato) == 0)
		*update = '#';
	else if (pt == 2 && cur == '#' && cnt_see_occ(y, x, dato) >= 5)
		*update  = 'L';
	else if (cur != 'L' && cur != '#' && cur != '.')
		printf("Error seat can not be %c\n", cur);
	if (*update == cur)
		return (0);			// No changes
	return (1);
}

int		checkarea(dat dato, dat datn, int pt)
{
	int		updates = 0;
	int		it_y = 1;
	int		it_x;

	while (it_y <= dato.rows)
	{
		it_x = 1;
		while (it_x <= dato.rowlen)
		{
			updates += checkseat(it_y, it_x, dato, datn, pt);
			it_x++;
		}
		it_y++;
	}
	return (updates);
}
int		countseats(dat d, char c)
{
	int ct = 0;
	int	y = 1;
	int	x;
	
	while (y <= d.rows)
	{
		x = 1;
		while (x <= d.rowlen)
		{
			if (d.seats[y][x] == c)
				ct++;
			x++;
		}
		y++;
	}
	return (ct);
}
int		main(int argc, char **argv)
{
	int		ft = 0;
	char	*file0 = "input.txt";
	char	*file1 = "example.txt";
	char	*file2 = "pt2_example2.txt";
	char	*file3 = "adj_example.txt";
	char	*file;

	if (argc > 1)
		ft = atoi(argv[1]);	
	printf("ft = %i\n", ft);
	if (ft == 1)
		file = file1;
	else if (ft == 2)
		file = file2;	
	else if (ft == 3)
		file = file3;	
	else
		file = file0;

	printf("= INPUT CHECK ================================================\n");
	dat		data2;
	int		lines;
	int		parts;					//Parts can be different groups of lines separated by \n or sth.
	int		*ptparts = &parts;		//Not needed now, so not redirecting to data2 struct.
	int		maxline;

	data2.rows = getnumberoflines(file, ptparts, &data2.rowlen);
	lines = data2.rows;
	maxline = data2.rowlen;
	printf("Number of lines: %i\n", lines);
	printf("Number of 'parts': %i\n", parts);
	printf("(Max) line length: %i\n", maxline);

	printf("= ALLOC MEMORY & READING DATA ================================\n");

	printf("Allocating data array...\n");
	data2.seats = alloc_seat_mem(data2.rows, data2.rowlen);
	
	printf("Start reading input...\n");
	getdata(file, data2);
		
	printf("= CHECKS EXTRA ===============================================\n");
	printf("= PRINT TEST DATA ============================================\n");
	int	lastprint = 5;
	if (lastprint > lines)
		lastprint = lines;

	if (argc > 2)
		lastprint = atoi(argv[2]);	
	printf("PRINT AREA (cropped):\n");
	printgrid(data2.seats, lastprint, data2.rowlen);

	printf("= PT1 ANALYSIS ===============================================\n");
	dat		data2new;
	
	data2new.seats = alloc_seat_mem(data2.rows, data2.rowlen);
	int		rounds = 0;
	int		updates = 1;

	while (updates > 0)
	{
		updates = 0;
		if (rounds % 50 == 0)
			printf("Check input round %i\n", rounds);
		copydat(&data2new, &data2);
		updates = checkarea(data2, data2new, 1);
		if (rounds % 50 == 0)
			printgrid(data2new.seats, lastprint, data2.rowlen);
		copydat(&data2, &data2new);
		if (rounds % 50 == 0)
			printf("%i updates\n", updates);
		rounds++;
	}
	printf("Final seating part 1 (%i rounds):\n", rounds);
	printgrid(data2new.seats, lastprint, data2.rowlen);
	
	int	y = 1;
	int	occ = countseats(data2, '#');
	printf("Answer part 1 = %i\n", occ);
	// 2354

	printf("= PT2 ANALYSIS ===============================================\n");
	rounds = 0;
	updates = 1;
	dat		pt2;

	pt2.rows = data2.rows;
	pt2.rowlen = data2.rowlen;
	pt2.seats = alloc_seat_mem(pt2.rows, pt2.rowlen);
	getdata(file, pt2);
	printgrid(pt2.seats, lastprint, data2.rowlen);


	while (updates > 0)
	{
		updates = 0;
		if (rounds % 20 == 0)
			printf("Check input round %i\n", rounds);
		copydat(&data2new, &pt2);
		updates = checkarea(pt2, data2new, 2);
		if (rounds % 20 == 0)
			printgrid(data2new.seats, lastprint, data2.rowlen);
		copydat(&pt2, &data2new);
		if (rounds % 20 == 0)
			printf("%i updates\n", updates);
		rounds++;
	}
	printf("Final seating part 2 (%i rounds):\n", rounds);
	printgrid(data2new.seats, lastprint, data2.rowlen);
	
	occ = countseats(pt2, '#');;
	printf("Answer part 2 = %i\n", occ);
	// 2072

	//Freeing data
	y = 0;
	while (y < data2.rows + 2)
	{
		free(data2.seats[y]);
		free(data2new.seats[y]);
		y++;
	}
	free(data2.seats);
	free(data2new.seats);
	printf("= END ========================================================\n");
	return (0);
}

/*
** Functions 'test reads' the data and returns number of lines
** + stores number of 'partsi/files' (for example sep. by newline) to the pointers
*/

int		getnumberoflines(char *file, int *ptparts, int *maxline)
{
	FILE	*in_file = fopen(file, "r");
	
	if (in_file == NULL)
		printf("File read failed\n");
	
	char	line[MAXBUF];
	char	*lpt;
	int		lines = 0;
	int		ppfiles = 0;
	int		linesize;
	
	while (fgets(line, sizeof(line), in_file) != NULL)
	{
		lines++;
		lpt = line;
		linesize = 0;
		while (*lpt != '\n' && lpt++ != NULL)
			linesize++;
		if (linesize > *maxline)
			*maxline = linesize;
		while ((lpt = strstr(lpt, "contain")))			//adjust this condition to count 'parts'
			ppfiles++;
	}
	*ptparts = ppfiles;
	fclose(in_file);

	return (lines);
}

void		getdata(char *file, struct ds dat)
{
	FILE	*in_file;
	char	str[MAXBUF];
	int		line;
	char	*pt;

	in_file = fopen(file, "r");
	if (in_file == NULL)
		printf("File read failed\n");

	printf("rowlen + 2 = %i\n", dat.rowlen + 2);
	memset(dat.seats[0], '.' , dat.rowlen + 2);
	memset(dat.seats[dat.rows + 1], '.', dat.rowlen + 2);
	line = 1;
	while (line < dat.rows + 1 && fgets(str, MAXBUF, in_file) != NULL)
	{
		//printf("reading: %s", str);
		pt = str;
		dat.seats[line][0] = '.';
		strncpy(dat.seats[line] + 1, str, strchr(str, '\n') - str);
		dat.seats[line][dat.rowlen + 1] = '.';
		line++;
	}
	fclose(in_file);
}
