// C program for Dijkstra's single source shortest path algorithm. 
// The program is for adjacency matrix representation of the graph. 
#include <stdio.h> 
#include <limits.h> 
#include <iostream>
#include <iomanip>

using namespace std;
// Number of vertices 
// in the graph 
#define V 19 //19 x matrix not 9 x 9 

// A utility function to find the vertex with minimum distance value, from the set of vertices 
// not yet included in shortest path tree 
int minDistance(int dist[], bool sptSet[]) 
{ 
	// Initialize min value 
	int min = INT_MAX, min_index; 

	for (int v = 0; v < V; v++) 
		if (sptSet[v] == false && 
				dist[v] <= min) 
			min = dist[v], min_index = v; 

	return min_index; 
} 

// Function to print shortest 
// path from source to j 
// using parent array 
void printPath(int parent[], int j) 
{ 
	// Base Case : If j is source 
	if (parent[j] == - 1) 
		return; 

	printPath(parent, parent[j]); 

	printf("%d ", j); 
} 

// A utility function to print 
// the constructed distance 
// array 
int printSolution(int dist[], int n, int parent[]) 
{ 
	int src = 0; 
	printf("\n"); 
	int start, end;
	printf("Choose a region index from 0-18\n");
	printf("0.Choa_Chu_Kang  1.Tuas  2.Woodlands  3.Mandai\n");          				
    printf("4.Suntec  5.Bukit_Timah  6.Jurong  7.Clementi\n");          			
    printf("8.Bukit_Panjang  9.Bedok  10.Ang_Mo_Kio  11.Serangoon\n");          				
    printf("12.Punggol  13.Upper_thomson   14.Expo  15.Toa_Payoh\n");          				
    printf("16.Bishan   17.Sentosa   18.Marina_Bay\n");  
        cout << "Please enter starting destination: ";
        cin >> start;
	printf("\n");
	cout << "Please enter end destination: ";
	cin >> end;
	printf("Vertex\t\t     Distance\t\t\t Path");
	for (int i = 1; i <= V; i++) 
	{ 
	    if (i == end) {	
	       printf("\n%d  -> %d \t\t %d\t\t\t%d ", start, end, dist[i], start); //0 -> 3          19        0 
	       printPath(parent, i); //1 2 3
	    } else {
	       printf (" "); 
	    }
	} 
} 

// Funtion that implements Dijkstra's 
// single source shortest path 
// algorithm for a graph represented 
// using adjacency matrix representation 
void dijkstra(int graph[V][V], int src) 
{ 
	// The output array. dist[i] 
	// will hold the shortest 
	// distance from src to i 
	int dist[V]; 

	// sptSet[i] will true if vertex 
	// i is included / in shortest 
	// path tree or shortest distance 
	// from src to i is finalized 
	bool sptSet[V]; 

	// Parent array to store 
	// shortest path tree 
	int parent[V]; 

	// Initialize all distances as 
	// INFINITE and stpSet[] as false 
	for (int i = 0; i < V; i++) 
	{ 
		parent[0] = -1; 
		dist[i] = INT_MAX; 
		sptSet[i] = false; 
	} 

	// Distance of source vertex 
	// from itself is always 0 
	dist[src] = 0; 

	// Find shortest path 
	// for all vertices 
	for (int count = 0; count < V - 1; count++) 
	{ 
		// Pick the minimum distance 
		// vertex from the set of 
		// vertices not yet processed. 
		// u is always equal to src 
		// in first iteration. 
		int u = minDistance(dist, sptSet); 

		// Mark the picked vertex 
		// as processed 
		sptSet[u] = true; 

		// Update dist value of the 
		// adjacent vertices of the 
		// picked vertex. 
		for (int v = 0; v < V; v++) 

			// Update dist[v] only if is 
			// not in sptSet, there is 
			// an edge from u to v, and 
			// total weight of path from 
			// src to v through u is smaller 
			// than current value of 
			// dist[v] 
			if (!sptSet[v] && graph[u][v] && 
				dist[u] + graph[u][v] < dist[v]) 
			{ 
				parent[v] = u; 
				dist[v] = dist[u] + graph[u][v]; 
			} 
	} 

	// print the constructed 
	// distance array 
	printSolution(dist, V, parent); 
} 

// Driver Code 
int main() 
{ 
     int a[V][V];
     int i,j;
     FILE *fp;

     if((fp = fopen("file.txt","r")) == NULL)
     {     fprintf(stderr,"Unable to open file\n");
           exit(1);
     }
     for(i = 0; i < V; i++)
          for(j = 0; j < V; j++)
               fscanf(fp, "%d", &a[i][j]);
     fclose(fp);
     
     //71:6: note: candidate function not viable: no known conversion from
     //'int [MAX_ROWS][MAX_COLS]' to 'int (*)[19]' for 1st argument
     //void dijkstra(int graph[V][V], int src) 
     dijkstra(a, 0); 
     printf("\n");
     return 0; 
} 
