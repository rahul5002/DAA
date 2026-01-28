#include <stdio.h>
#include <stdlib.h> 
#include <limits.h> 
#include <time.h>   
#define V 5 
#define INF (INT_MAX / 2)

void Solution(int dist[][V], const char* algorithmName) 
{
    printf("\n--- Shortest distance matrix (using %s) ---\n", algorithmName);
    for (int i = 0; i < V; i++) 
    {
        for (int j = 0; j < V; j++) 
        {
            if (dist[i][j] == INF)
                printf("%7s", "INF");
            else
                printf("%7d", dist[i][j]);
        }
        printf("\n");
    }
}

int min_dist(int dist[], int sptSet[]) 
{
    int min = INF, min_index = -1;

    for (int v = 0; v < V; v++) 
    {
        if (sptSet[v] == 0 && dist[v] <= min) 
        {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}
void floyd_Warshall(int dist[][V]) 
{
    int i, j, k;
    for (k = 0; k < V; k++) 
    {
        for (i = 0; i < V; i++) 
        {
            for (j = 0; j < V; j++) 
            {
                if (dist[i][k] != INF && dist[k][j] != INF && 
                    dist[i][k] + dist[k][j] < dist[i][j]) 
                {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }
}

void dijkstra(int graph[V][V], int src, int allPairsDist[V][V]) 
{
    int dist[V];  
    int sptSet[V]; 
    for (int i = 0; i < V; i++) 
    {
        dist[i] = INF;
        sptSet[i] = 0;
    }
    dist[src] = 0;
    for (int count = 0; count < V - 1; count++) 
    {
        int u = min_dist(dist, sptSet);
        if (u == -1) break;
        sptSet[u] = 1;
        for (int v = 0; v < V; v++) 
        {
            if (!sptSet[v] && graph[u][v] != INF &&
                dist[u] != INF &&
                dist[u] + graph[u][v] < dist[v]) 
            {
                
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }
    for(int i = 0; i < V; i++) 
    {
        allPairsDist[src][i] = dist[i];
    }
}
int main() 
{
    int graph[V][V] = { {0, 5, INF, 10, INF},
                        {INF, 0, 3, INF, INF},
                        {INF, INF, 0, 1, 2},
                        {INF, INF, INF, 0, INF},
                        {INF, INF, INF, INF, 0}
                      };
    int floyd_Sol[V][V];
    int dijkstraSol[V][V];
    for(int i=0; i < V; i++) 
    {
        for(int j=0; j < V; j++) 
        {
            floyd_Sol[i][j] = graph[i][j];
        }
    }
    clock_t start, end;
    double cpu_time_used;
    printf("Running Floyd-Warshall...\n");
    start = clock();
    floyd_Warshall(floyd_Sol);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Floyd-Warshall took %f seconds to execute.\n", cpu_time_used);
    Solution(floyd_Sol, "Floyd-Warshall");
    printf("\nRunning Dijkstra V times...\n");
    start = clock();
    for (int i = 0; i < V; i++) 
    {
        dijkstra(graph, i, dijkstraSol);
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Dijkstra (x%d) took %f seconds to execute.\n", V, cpu_time_used);
    Solution(dijkstraSol, "Dijkstra (V runs)");

    return 0;
} 