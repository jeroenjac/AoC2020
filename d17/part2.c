#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define	maxbuf 100

typedef struct gr
{
	int		dimension;
	int		***state;
	int		cycles;
	int		simdim;
}	gr;

typedef struct gr4
{
	int		dimension;
	int		****state;
	int		cycles;
	int		simdim;
}	gr4;

int		get_meta(char *filename, gr4 *grid, int maxcycles);
int		alloc_grid(gr4 *ret);
void	free_grid(gr4 *grid);

void	readinput(char *filename, gr4 *grid);
void	printlayer(gr4 grid, int z, int w);;
void	copy_grid(gr4 *to, gr4 *from);

int		sum_grid(gr4 grid);
void	runcycle(gr4 *grid, int run);
int		newstate(gr4 *grid, int w, int z, int y, int x);
int		check_neighbours_on(gr4 *grid, int w, int z, int y, int x);


int		main(int argc, char **argv)
{
	char	*filename;

	if (argc < 2)
	{
		printf("No input file\n");
		return (0);
	}
	filename = strdup(argv[1]);

	gr		grid3;
	gr4		grid;

	get_meta(filename, &grid, 6);
	printf("Grid dimension = %i\n", grid.dimension);

	printf("Alloc for grid %s\n", (alloc_grid(&grid) == 1) ? "worked" : "failed");

	readinput(filename, &grid);

	//Testing the check neighbours & newstate function - WORKSs
	int try_w = 0;
	int try_z = 0;
	int	try_y = 0;
	int	try_x = 0;
	try_w += grid.cycles;
	try_z += grid.cycles;
	try_y += grid.cycles;
	try_x += grid.cycles;
	printf("Point = %i\n", grid.state[try_w][try_z][try_y][try_x]);

	printf("How many neighbours on? %i\n", check_neighbours_on(&grid, try_w, try_z, try_y, try_x));
	printf("New state: %i\n", newstate(&grid, try_w, try_z, try_y, try_x));


	//testing the copy function - WORKS
	gr4	new;
/*
	get_meta(filename, &new, 6);
	alloc_grid(&new);
	printlayer(new, try_z, try_w);
	copy_grid(&new, &grid);
	printlayer(new, try_z, try_w);
	free_grid(&new);
*/

	printf("Print, run 6 cycles, print again\n");
	printlayer(grid, try_z - 1, try_w);
	printlayer(grid, try_z, try_w);
	printlayer(grid, try_z + 1, try_w);
	printf("== CYCLE 1 ==\n");
	runcycle(&grid, 1);
	printlayer(grid, try_z - 1, try_w);
	printlayer(grid, try_z, try_w);
	printlayer(grid, try_z + 1, try_w);

	printf("== CYCLE 2 ==\n");
	runcycle(&grid, 2);
	printlayer(grid, try_z, try_w);
	printf("== CYCLE 3 ==\n");
	runcycle(&grid, 3);
	printlayer(grid, try_z, try_w);
	printf("== CYCLE 4 ==\n");
	runcycle(&grid, 4);
	printf("== CYCLE 5 ==\n");
	runcycle(&grid, 5);
	printf("== CYCLE 6 ==\n");
	runcycle(&grid, 6);
/*	printlayer(grid, try_z - 5);
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
*/
	printf("After 6 cycles, there are %i active cubes\n", sum_grid(grid));

	free_grid(&grid);

	return (1);
}

int		sum_grid(gr4 grid)
{
	int	w, z, y, x;
	int	sum = 0;
	int dim = grid.simdim;

w = 0;
while (w < dim)
{
	z = 0;
	while (z < dim)
	{
		y = 0;
		while (y < dim)
		{
			x = 0;
			while (x < dim)
			{
				sum += grid.state[w][z][y][x];
				x++;
			}
			y++;
		}
		z++;
	}
	w++;
}

	return (sum);
}

void	runcycle(gr4 *grid, int run)
{
	gr4	new;
	int	x, y, z, w;
	int dim = grid->simdim;
	int	c = grid->cycles;

	new.simdim = dim;
	//for every position, calc new state
	
	alloc_grid(&new);

w = 0;
while (w < dim)
{
	printf("Run %i with w = %i\n", run, w);
	z = 0;
	while (z < dim)
	{
		y = 0;
		while (y < dim)
		{
			x = 0;
			while (x < dim)
			{
				new.state[w][z][y][x] = newstate(grid, w, z, y, x);
				x++;
			}
			y++;
		}
		z++;
	}
	w++;
}

	//copy new to grid.states
	
	copy_grid(grid, &new);
	
	free_grid(&new);

}

int		newstate(gr4 *grid, int w, int z, int y, int x)
{
	int current_state = grid->state[w][z][y][x];
	int	ct = check_neighbours_on(grid, w, z, y, x);

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
}

int		check_neighbours_on(gr4 *grid, int w, int z, int y, int x)
{
	int	sw;
	int sz;
	int	sy;
	int	sx;
	int	dim = grid->simdim;
	int	cnt_on = 0;

	sw = -1;
	while (sw <= 1)
	{
		if (w + sw > 0 && w + sw < dim)
		{
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
									if (!(sx == 0 && sy == 0 && sz == 0 && sw == 0))
										cnt_on += grid->state[w + sw][z + sz][y + sy][x + sx];
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
			
		}
		sw++;
	}

	return (cnt_on);
}

void	copy_grid(gr4 *to, gr4 *from)
{
	int w, z, y, x;
	int dim = from->simdim;

w = 0;
while (w < dim)
{
	z = 0;
	while (z < dim)
	{
		y = 0;
		while (y < dim)
		{
			x = 0;
			while (x < dim)
			{
				to->state[w][z][y][x] = from->state[w][z][y][x];
				x++;
			}
			y++;
		}
		z++;
	}
	w++;
}

}

void	printlayer(gr4 grid, int z, int w)
{
	int	x, y;

	printf("Printing for z = %i & w = %i (transformed: - %i)\n", z - grid.cycles, w - grid.cycles, grid.cycles);
	y = 0;
	while (y < grid.simdim)
	{
		x = 0;
		while (x < grid.simdim)
		{
			printf("%c", (grid.state[w][z][y][x] == 1) ? '#' : '.');
			x++;
		}
		printf("\n");
		y++;
	}
}

void	readinput(char *filename, gr4 *grid)
{
	FILE	*file = fopen(filename, "r");
	char	line[maxbuf];
	int		x, y, z, w;

	w = grid->cycles;
	z = grid->cycles;
	y = grid->cycles;
	while (fgets(line, sizeof(line), file) != NULL)
	{
		printf("Reading: %s", line);
		x = grid->cycles;
		while (line[x - grid->cycles] != '\n')
		{
			grid->state[w][z][y][x] = (line[x - grid->cycles] == '#') ? 1 : 0;
			x++;
		}
		y++;
	}
	printf("Read complete\n");
}

int		get_meta(char *filename, gr4 *grid, int maxcycles)
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
	grid->simdim = dim + 2 * maxcycles;			//valide for 4D??
	return (1); //succes
}

int		alloc_grid(gr4 *grid)
{
	int	w;
	int	z;
	int	y;
	int	dim = grid->simdim;

	grid->state = calloc(dim, sizeof(*grid->state));
	if (grid->state == NULL)
		return (0);
	w = 0;

while (w < dim)
{
	grid->state[w] = calloc(dim, sizeof(**grid->state));
	if (grid->state[w] == NULL)
		return (0);
	z = 0;
	while (z < dim)
	{
		grid->state[w][z] = calloc(dim, sizeof(***grid->state));
		if (grid->state[w][z] == NULL)
			return (0);
		y = 0;
		while (y < dim)
		{
			grid->state[w][z][y] = calloc(dim, sizeof(****grid->state));
			if (grid->state[w][z][y] == NULL)
				return (0);
			y++;
		}
		z++;
	}
	w++;
}	

	return (1); //succes
}

void	free_grid(gr4 *grid)
{
	int w;
	int	z;
	int	y;
	int	dim = grid->simdim;
	
w = 0;
while (w < dim)
{
	z = 0;
	while (z < dim)
	{
		y = 0;
		while (y < dim)
		{
			free(grid->state[w][z][y]);
			y++;
		}
		free(grid->state[w][z]);
		z++;
	}
	w++;
}

	free(grid->state);
	printf("ALL GRID STATES ARE FREE\n");
}
