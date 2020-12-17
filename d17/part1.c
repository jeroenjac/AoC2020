#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define	maxbuf 100

typedef struct gr
{
	int		dimension;
	char	on;
	char	off;
	int		***state;
	int		center[3];
	int		cycles;
	int		simdim;
}	gr;

int		get_meta(char *filename, gr *grid, int maxcycles);
int		alloc_grid(gr *ret);
void	free_grid(gr *grid);

void	readinput(char *filename, gr *grid);
void	printlayer(gr grid, int z);
void	copy_grid(gr *to, gr *from);

int		sum_grid(gr grid);
void	runcycle(gr *grid);
int		newstate(gr *grid, int z, int y, int x);
int		check_neighbours_on(gr *grid, int z, int y, int x);

int		main(int argc, char **argv)
{
	char	*filename;

	if (argc < 2)
	{
		printf("No input file\n");
		return (0);
	}
	filename = strdup(argv[1]);

	gr		grid;

	get_meta(filename, &grid, 6);
	printf("Grid dimension = %i\n", grid.dimension);
	printf("Grid on / off  = %c / %c\n", grid.on, grid.off);
	
	printf("Alloc for grid %s\n", (alloc_grid(&grid) == 1) ? "worked" : "failed");

	readinput(filename, &grid);

	//Testing the check neighbours & newstate function - WORKSs
	int try_z = 0;
	int	try_y = 0;
	int	try_x = 0;
	try_z += grid.cycles;
	try_y += grid.cycles;
	try_x += grid.cycles;
	printf("Point = %i\n", grid.state[try_z][try_y][try_x]);
	printf("How many neighbours on? %i\n", check_neighbours_on(&grid, try_z, try_y, try_x));
	printf("New state: %i\n", newstate(&grid, try_z, try_y, try_x));

/*
	//testing the copy function - WORKS
	gr	new;

	get_meta(filename, &new, 6);
	alloc_grid(&new);
	printlayer(new, try_z);
	copy_grid(&new, &grid);
	printlayer(new, try_z);
	free_grid(&new);
*/	
	printf("Print, run 6 cycles, print again\n");
	printlayer(grid, try_z - 1);
	printlayer(grid, try_z);
	printlayer(grid, try_z + 1);
	printf("== CYCLE 1 ==\n");
	runcycle(&grid);
	printlayer(grid, try_z - 1);
	printlayer(grid, try_z);
	printlayer(grid, try_z + 1);
	printf("== CYCLE 2 ==\n");
	runcycle(&grid);
	printlayer(grid, try_z);
	printf("== CYCLE 3 ==\n");
	runcycle(&grid);
	printlayer(grid, try_z);
	printf("== CYCLE 4 ==\n");
	runcycle(&grid);
	printf("== CYCLE 5 ==\n");
	runcycle(&grid);
	printf("== CYCLE 6 ==\n");
	runcycle(&grid);
	printlayer(grid, try_z - 5);
	printlayer(grid, try_z - 4);
	printlayer(grid, try_z - 3);
	printlayer(grid, try_z - 2);
	printlayer(grid, try_z - 1);
	printlayer(grid, try_z);
	printlayer(grid, try_z + 1);
	printlayer(grid, try_z + 2);
	printlayer(grid, try_z + 3);
	printlayer(grid, try_z + 4);
	printlayer(grid, try_z + 5);

	printf("After 6 cycles, there are %i active cubes\n", sum_grid(grid));
	
	free_grid(&grid);

	return (1);
}

int		sum_grid(gr grid)
{
	int	z, y, x;
	int	sum = 0;
	int dim = grid.simdim;

	z = 0;
	while (z < dim)
	{
		y = 0;
		while (y < dim)
		{
			x = 0;
			while (x < dim)
			{
				sum += grid.state[z][y][x];
				x++;
			}
			y++;
		}
		z++;
	}
	return (sum);
}

void	runcycle(gr *grid)
{
	gr	new;
	int	x, y, z;
	int dim = grid->simdim;

	new.simdim = dim;
	//for every position, calc new state
	alloc_grid(&new);
	z = 0;
	while (z < dim)
	{
		y = 0;
		while (y < dim)
		{
			x = 0;
			while (x < dim)
			{
				new.state[z][y][x] = newstate(grid, z, y, x);
				x++;
			}
			y++;
		}
		z++;
	}
	//copy new to grid.states
	copy_grid(grid, &new);
	free_grid(&new);
}

int		newstate(gr *grid, int z, int y, int x)
{
	int current_state = grid->state[z][y][x];
	int	ct = check_neighbours_on(grid, z, y, x);

	if (current_state == 1)
	{
		if (ct == 2 || ct == 3)
			return (1);
		else
			return (0);
	}
	else if (current_state == 0)
	{
		if (ct == 3)
			return (1);
		else
			return (0);
	}
	else
		return (current_state);	//not needed
/*

	if (ct < 2 || ct > 3)
		return (current_state);
	else if (ct == 2 && current_state == 1)
		return (1);
	else if (ct == 3)
	{
		if (current_state == 1)
			return (0);
		else
			return (1);
	}
	return (current_state);	//not needed
*/
}

int		check_neighbours_on(gr *grid, int z, int y, int x)
{
	int sz;
	int	sy;
	int	sx;
	int	dim = grid->simdim;
	int	cnt_on = 0;

	sz = -1;
	while (sz <= 1)
	{
		if (z + sz > 0 && z + sz < dim)
		{
			sy = -1;
			while (sy <= 1)
			{
				if (y + sy > 0 && y + sy < dim)
				{
					sx = -1;
					while (sx <= 1)
					{
						if (x + sx > 0 && x + sx < dim)
						{
							if (!(sx == 0 && sy == 0 && sz == 0))
								cnt_on += grid->state[z + sz][y + sy][x + sx];
							if (cnt_on > 3)
								return (cnt_on);
						}
						sx++;
					}
				}
				sy++;
			}
		}
		sz++;
	}
	return (cnt_on);
}

void	copy_grid(gr *to, gr *from)
{
	int z, y, x;
	int dim = from->simdim;

	z = 0;
	while (z < dim)
	{
		y = 0;
		while (y < dim)
		{
			x = 0;
			while (x < dim)
			{
				to->state[z][y][x] = from->state[z][y][x];
				x++;
			}
			y++;
		}
		z++;
	}
}

void	printlayer(gr grid, int z)
{
	int	x, y;

	printf("Printing for z = %i (transformed: - %i)\n", z - grid.cycles, grid.cycles);
	y = 0;
	while (y < grid.simdim)
	{
		x = 0;
		while (x < grid.simdim)
		{
			printf("%c", (grid.state[z][y][x] == 1) ? '#' : '.');
			x++;
		}
		printf("\n");
		y++;
	}
}

void	readinput(char *filename, gr *grid)
{
	FILE	*file = fopen(filename, "r");
	char	line[maxbuf];
	int		x, y, z;

	z = grid->cycles;
	y = grid->cycles;
	while (fgets(line, sizeof(line), file) != NULL)
	{
		printf("Reading: %s", line);
		x = grid->cycles;
		while (line[x - grid->cycles] != '\n')
		{
			grid->state[z][y][x] = (line[x - grid->cycles] == grid->on) ? 1 : 0;
			x++;
		}
		y++;
	}
	printf("Read complete\n");
}

int		get_meta(char *filename, gr *grid, int maxcycles)
{
	FILE	*file = fopen(filename, "r");
	int		dim = 0;
	char	line[maxbuf];

	fgets(line, sizeof(line), file);

	if (file != NULL)
		while (line[dim] != '\n')
		 dim++;

	grid->cycles = maxcycles;
	grid->dimension = dim;
	grid->simdim = dim + 2 * maxcycles;
	grid->on = '#';
	grid->off = '.';
	grid->center[0] = 1 + maxcycles;
	grid->center[1] = 1 + maxcycles;
	grid->center[2] = 1 + maxcycles;
	return (1); //succes
}

int		alloc_grid(gr *grid)
{
	int	z;
	int	y;
	int	dim = grid->simdim;

	grid->state = calloc(dim, sizeof(*grid->state));
	if (grid->state == NULL)
		return (0);
	z = 0;
	while (z < dim)
	{
		grid->state[z] = calloc(dim, sizeof(**grid->state));
		if (grid->state[z] == NULL)
			return (0);
		y = 0;
		while (y < dim)
		{
			grid->state[z][y] = calloc(dim, sizeof(***grid->state));
			if (grid->state[z][y] == NULL)
				return (0);
			y++;
		}
		z++;
	}	
	return (1); //succes
}

void	free_grid(gr *grid)
{
	int	z;
	int	y;
	int	dim = grid->simdim;
	
	z = 0;
	while (z < dim)
	{
		y = 0;
		while (y < dim)
		{
			free(grid->state[z][y]);
			y++;
		}
		free(grid->state[z]);
		z++;
	}
	free(grid->state);
	printf("ALL GRID STATES ARE FREE\n");
}
