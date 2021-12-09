#include <stdio.h>
#include <time.h>
#include <math.h>
#include <omp.h>


#define INT_MAX 100000
#define TRUE    1
#define FALSE   0
#define V 8
#define E 11

//boolean type
typedef int bool;

//Represents an edge or path between Vertices
typedef struct
{
	int u;
	int v;

} Edge;

//Represents a Vertex
typedef struct
{
	int title;
	bool visited;	

} Vertex;


void printArray(int *array)
{
	int i;
	for(i = 0; i < V; i++)
	{
		printf("Path to Vertex %d is %d\n", i, array[i]);
	}
}




int findEdge(Vertex u, Vertex v, Edge *edges, int *weights)
{

	int i;
	for(i = 0; i < E; i++)
	{

		if(edges[i].u == u.title && edges[i].v == v.title)
		{
			return weights[i];
		}
	}

	return INT_MAX;




}


int minimum(int A, int B)
{
	if( A > B)
	{
		return B;
	}

	else{
		return A;
	}
}


int minWeight(int *len, Vertex *vertices)
{
	int i;
	int minimum = INT_MAX;
	for(i = 0; i < V; i++)
	{
		if(vertices[i].visited == TRUE)
		{
			continue;
		}
		
		else if(vertices[i].visited == FALSE && len[i] < minimum)
		{
			minimum = len[i];
			
		}
		
	}
	return minimum;
}


int minPath(Vertex *vertices, int *len)
{
	int i;
	int min = minWeight(len, vertices);
	
	for(i = 0; i < V; i++)
	{
		if(vertices[i].visited == FALSE && len[vertices[i].title] == min)
		{
			vertices[i].visited = TRUE;
			return i;

		}
	}
	
	
	
	
}


void DijkstraOMP(Vertex *vertices, Edge *edges, int *weights, Vertex *root)
{	


	double start, end;
	root->visited = TRUE;
	
	int len[V];
	len[(int)root->title] = 0;

	int i, j;
	
	for(i = 0; i < V;i++)
	{
		if(vertices[i].title != root->title)
		{
			len[(int)vertices[i].title] = findEdge(*root, vertices[i], edges, weights);
			

		}
		else{
		
			vertices[i].visited = TRUE;
		}


	}

	for(j = 0; j < V; j++){
		Vertex u;
		int h = minPath(vertices, len);
		u = vertices[h];
		
		start = omp_get_wtime();
		//OpenMP Parallelization Starts here!!!
		#pragma omp parallel for schedule(runtime) private(i)
			for(i = 0; i < V; i++)
			{
				
				if(vertices[i].visited == FALSE)
				{	
					int c = findEdge( u, vertices[i], edges, weights);
					len[vertices[i].title] = minimum(len[vertices[i].title], len[u.title] + c);
				
				}
			}
	

	}
	printArray(len);
	end = omp_get_wtime();
	printf("Running time: %f ms\n", (end - start));
	
	
	
}

int main(void)
{
	int nThreads;
	double start,end;
	//clock_t start=clock();
	printf("Enter Number of threads: ");
	scanf("%d", &nThreads);
	omp_set_num_threads(nThreads);
	
	Vertex vertices[V];
	// Edge edges[E];
	// int weights[E];

	Edge edges[E] ={{0, 4}, {0, 6}, {0,2}, {4,6}, {4,7}, {0, 7}, {7, 3}, {3, 1}, {2,5}, {2, 1}, {5,3}};
	int weights[E] = {10, 90, 30, 20, 20, 50, 10, 20, 10, 10, 10};

	int i = 0;
	for(i = 0; i < V; i++)
	{
		Vertex a = { .title =i , .visited=FALSE};
		vertices[i] = a;


	}

	Vertex root = {0, FALSE};

	printf("OpenMP Results for Small Graph of 8 Vertices:\n");
	DijkstraOMP(vertices, edges, weights, &root);
	
	}
