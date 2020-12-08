#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
//#include "libft.h"

#define	MAXBUF 200

typedef struct	ds
{
	char	base[15];
	char	adj[15];
	char	col[40];
	int		csg;		//Contains Shiny Gold #
	int		empty;		//Equal to 1 for empty bag
	char	cont[200];
}			color;


// A structure to represent an adjacency list node 
struct AdjListNode 
{ 
    int dest; 
    struct AdjListNode* next; 
}; 
  
// A structure to represent an adjacency list 
struct AdjList 
{ 
    struct AdjListNode *head;  
}; 
  
// A structure to represent a graph. A graph 
// is an array of adjacency lists. 
// Size of array will be V (number of vertices  
// in graph) 
struct Graph 
{ 
    int V; 
    struct AdjList* array; 
}; 
 
void printGraph(struct Graph* graph); 
void addEdge(struct Graph* graph, int src, int dest); 
struct Graph* createGraph(int V);
struct AdjListNode* newAdjListNode(int dest); 

void	storedata(char *file, int lines, struct ds *groups);
int		getnumberoflines(char *file, int *ptparts);
int		getcolid(char *colname, color *dat, int lines);
void	filldata(int lines, struct ds *dat, struct Graph* graph);
int		checkCSG(struct Graph* graph, color *dat, int lines, int v);
int		goldfinder(struct Graph *graph, color *dat, int lines, int line);

int		main(void)
{
	char	*file = "input.txt";
	//char	*file = "example.txt";
	FILE	*in_file;

	printf("= CHECKS AND BALANCES ========================================\n");
	//Step 1: Getting number of lines
	int		lines;
	int		parts;
	int		*ptparts;
	ptparts = &parts;
	
	lines = getnumberoflines(file, ptparts);

	//Checks tbd
	// 1 - does every line has one 'rule', e.g. not "a contain b contain c"? YES
	// 2 - does every line represent a unique color, e.g. not two lines "a contain DEF" 
	// "a contain IJK"? YES
	// 3 - are there similar colors, like "blue" and "pale blue"?
	//		NOT APPLICABLE - EXTENSIVE CHECK 2.
	printf("Number of lines: %i\n", lines);
	printf("Number of 'contains': %i\n", parts);

	printf("= ALLOC MEMORY & READING DATA ================================\n");
	//Step 2: Allocating arrays
	color	*colorlines;
    
	int V = lines;; 
    struct Graph* graph = createGraph(V); 

	printf("Allocating data array...\n");
	colorlines = calloc(lines, sizeof(color));
	if (colorlines == NULL)
		printf("Allocation for data array failed");
	
	//Step 3: Reading file per MAXBUF chars
	printf("Start reading input...\n");
	storedata(file, lines, colorlines);
	filldata(lines, colorlines, graph);
	
	printf("= CHECKS EXTRA ===============================================\n");
	int	i = 0, j = 0;
	int	db = 0;

	while (i < lines && db == 0)
	{
		j = i + 1;
		while (j < lines && db == 0)
		{
			if (	!strcmp(colorlines[i].adj, colorlines[j].adj) &&\
					!strcmp(colorlines[i].base, colorlines[j].base)	)
				db = 1;
			j++;
		}
		i++;
	}
	printf("db = %i\n", db);

	printf("= PRINT TEST DATA ============================================\n");
	int	lastprint = 30;
	
	//Validate if reading data is succes
	i = 0;
	while (i <= lastprint)
	{
		printf("line %-3i ", i);
		printf("col=%-20s", colorlines[i].col);
		printf("oth=%-60s", colorlines[i].cont);
		//printf("adj=%s\t", colorlines[i].adj);
		//printf("base=%s\t", colorlines[i].base);
		printf("csg=%i ", colorlines[i].csg);
		printf("empty=%i\n", colorlines[i].empty);
		i++;
	}

	printGraph(graph); 
	
	printf("= PT1 ANALYSIS ===============================================\n");
    
	
	int	bagswithgold = 0;
	//Calc number of questions with any person "y", per group
	i = 0;
	int ret = 99;
	while (i < lines)
	{
		//ret = checkCSG(graph, colorlines, lines, i);
		ret = goldfinder(graph, colorlines, lines, i);
		if (ret == 42)
			colorlines[i].csg = 1;
		//CheckGold colorline(i)
		//	- this fun can see "empty" - DONE / return 2; ALWAYS IN THE END
		//  - this fun can see "gold" - DONE / return 1;
		//	- this fun can see sth else - CALL AGAIN
		if (colorlines[i].csg > 0)
			bagswithgold++;
		printf("i = %i, ret = %i, bags = %i\n", i, ret, bagswithgold);
		i++;
	}

	printf("Answer part 1 = %i\n", bagswithgold);
/*	
	printf("= PT2 ANALYSIS ===============================================\n");
	//Calc number of questions all "y", per group
	long	num_q_all_yes = 0;
	i = 0;
	while (i < parts)
	{
		q = 0;
		while (q < 26)
		{
			num_q_all_yes += groups[i].q_all[q];
			q++;
		}
		i++;
	}
	
	printf("Answer part 2 = %li\n", num_q_all_yes);
*/
	printf("= END =======================================================\n");
	free(colorlines);
	return (0);
}

/*
** Functions 'test reads' the data and returns number of lines
** + stores number of 'partsi/files' (for example sep. by newline) to the pointers
*/

int		getnumberoflines(char *file, int *ptparts)
{
	FILE	*in_file = fopen(file, "r");
	
	if (in_file == NULL)
		printf("File read failed\n");
	
	char	line[MAXBUF];
	char	*lpt;
	int		lines = 0;
	int		ppfiles = 0;
	
	while (fgets(line, sizeof(line), in_file) != NULL)
	{
		lines++;
		lpt = line;
		while (strstr(lpt, "contain"))
		{
			ppfiles++;
			lpt = strstr(lpt, "contain") + 1;
		}
	}
	*ptparts = ppfiles;
	fclose(in_file);

	return (lines);
}

void		storedata(char *file, int lines, struct ds *dat)
{
	FILE	*in_file;
	char	line[MAXBUF];
	char	*pt, *end, end2;;
	char	newcolor[40];
	int		i, j;

	in_file = fopen(file, "r");
	if (in_file == NULL)
		printf("File read failed\n");
	
	i = 0;
	j = -1;
	while (fgets(line, MAXBUF, in_file) != NULL)
	{
		//printf("reading: %s", line);
		pt = line;
		end = strchr(pt, ' ');
		strncpy(dat[i].adj, pt, end - pt);
		pt = end + 1;
		end = strchr(pt, ' ');
		strncpy(dat[i].base, pt, end - pt);
		//Only this one is important for endresult
		strncpy(dat[i].col, line, end - line);
		pt = strstr(pt, "contain") + 7;
		if (strstr(pt, "no other"))
			dat[i].empty = 1;
		else
		{
			pt = pt + 3;			//pt at begin of new col
			while (pt != NULL)
			{
				bzero(newcolor, 40);
				end = strchr(pt, ' ') + 1;
				end = strchr(end, ' ');
				strncpy(newcolor, pt, end - pt);
				strncat(dat[i].cont, newcolor, strlen(newcolor));
				strcat(dat[i].cont, "|");
				pt = strchr(pt, ',');
				if (pt != NULL)
					pt = pt + 4;
			}
		}
		i++;
	}
	fclose(in_file);
}

//This is data enrichment
void	filldata(int lines, struct ds *dat, struct Graph* graph)
{
	char	*pt, *end;
	char	newcolor[40];
	int		i, j;
		
	i = 0;
	j = -1;
	//Replace with get from dat
	while (i < lines)
	{
		dat[i].csg = 0;
		pt = dat[i].cont;
		while (*pt != '\0')
		{
			bzero(newcolor, 40);
			end = strchr(pt, '|');
			strncpy(newcolor, pt, end - pt);
			// ADD vertices
			j = getcolid(newcolor, dat, lines);
			if (j >= 0)
				addEdge(graph, i, j); 
			pt = end + 1;
		}
		i++;
	}
}

int		containshinygold(color *checkcolor)
{
	if (checkcolor->empty == 1)
		return 0;
	else if (checkcolor->csg == 1)
		return 1;
	return 2; //Check next color, but this is a list of different length :(
}

//How to 'start'  this fun?
//For every line...?

int		goldfinder(struct Graph *graph, color *dat, int lines, int line)
{
	struct	AdjListNode *pCrawl;
	int	goldid = getcolid("shiny gold", dat, lines);

	pCrawl = graph->array[line].head;
	printf("check 'line' %i\n", line);
	while (pCrawl)
	{
		if (pCrawl->dest == goldid)
			return (42);
		pCrawl = pCrawl->next;
		return(goldfinder(graph, dat, lines, pCrawl->dest));
	}
	return (0);
}


int		checkCSG(struct Graph* graph, color *dat, int lines, int v) 
{ 
	int	goldid = getcolid("shiny gold", dat, lines);
	int	foundgold = 0;

	printf("Checking line/col %i\n", v);
    struct AdjListNode* pCrawl = graph->array[v].head; //In this v I need to fill dest
	//printf("\n Adjacency list of vertex %d\n head ", v); 
	while (pCrawl && foundgold != 1) 
    {
		//printf("-> %d", pCrawl->dest);
		printf("dest: %i (=? %i)\n", pCrawl->dest, pCrawl->dest == goldid);
		if (pCrawl->dest == goldid)
		{
			dat[v].csg = 33;
			foundgold = 1;
			return (1);
		}
		else
			checkCSG(graph, dat, lines, pCrawl->dest);
		pCrawl = pCrawl->next; 
    }
	return (2);
	//printf("\n");
}

int		getcolid(char *colname, color *dat, int lines)
{
	int	i = 0;

	while (i < lines)
	{
		if (strcmp(colname, dat[i].col) == 0)
			return (i);
		i++;
	}
	return (-1);
}
  
// A utility function to create a new adjacency list node 
struct AdjListNode* newAdjListNode(int dest) 
{ 
    struct AdjListNode* newNode = 
     (struct AdjListNode*) malloc(sizeof(struct AdjListNode)); 
    newNode->dest = dest; 
    newNode->next = NULL; 
    return newNode; 
} 
  
// A utility function that creates a graph of V vertices 
struct Graph* createGraph(int V) 
{ 
    struct Graph* graph =  
        (struct Graph*) malloc(sizeof(struct Graph)); 
    graph->V = V; 
  
    // Create an array of adjacency lists.  Size of  
    // array will be V 
    graph->array =  
      (struct AdjList*) malloc(V * sizeof(struct AdjList)); 
  
    // Initialize each adjacency list as empty by  
    // making head as NULL 
    int i; 
    for (i = 0; i < V; ++i) 
        graph->array[i].head = NULL;

    return graph; 
} 
  
// Adds an edge to an undirected graph 
void addEdge(struct Graph* graph, int src, int dest) 
{ 
    // Add an edge from src to dest.  A new node is  
    // added to the adjacency list of src.  The node 
    // is added at the beginning 
    struct AdjListNode* newNode = newAdjListNode(dest); 
    newNode->next = graph->array[src].head; 
    graph->array[src].head = newNode;

/*
**	POGING OM NEWNODE ACHTERAAN VAST TE MAKEN

	newNode->next = NULL;
	if (graph->array[src].head == NULL)
		graph->array[src].head = newNode;
	else
	{
		while (graph->array[src].head->dest != NULL)
			graph->array[src].head->dest = graph->array[src].head->next;
		graph->array[src].head->dest = newNode;
	}
*/

/* 
** NOT NEEDED FOR THIS PROBLEM
    
	// Since graph is undirected, add an edge from 
    // dest to src also 
    newNode = newAdjListNode(src); 
    newNode->next = graph->array[dest].head; 
    graph->array[dest].head = newNode; 
*/
} 
  
// A utility function to print the adjacency list  
// representation of graph 
void printGraph(struct Graph* graph) 
{ 
    int v; 
    for (v = 0; v < graph->V; ++v) 
    { 
        struct AdjListNode* pCrawl = graph->array[v].head; 
        printf("\n Adjacency list of vertex %d\n head ", v); 
        while (pCrawl) 
        { 
            printf("-> %d", pCrawl->dest); 
            pCrawl = pCrawl->next; 
        } 
        printf("\n"); 
    } 
} 
/*
// Driver program to test above functions 
int main() 
{ 
    // create the graph given in above fugure 
    int V = 5; 
    struct Graph* graph = createGraph(V); 
    addEdge(graph, 0, 1); 
    addEdge(graph, 0, 4); 
    addEdge(graph, 1, 2); 
    addEdge(graph, 1, 3); 
    addEdge(graph, 1, 4); 
    addEdge(graph, 2, 3); 
    addEdge(graph, 3, 4); 
  
    // print the adjacency list representation of the above graph 
    printGraph(graph); 
  
    return 0; 
}
*/
