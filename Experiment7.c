#include <stdio.h>
#include <stdlib.h> 
#include <time.h>  
#include <limits.h> 
#include <stdbool.h> 

#define INF INT_MAX
struct Edge {
    int src, dest, weight;
};
struct Graph 
{
    int V; 
    int E; 
    int** adjMatrix; 
    struct Edge* edgeList;
    int edgeCount; 
};
struct Graph* createGraph(int V, int E) 
{
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->V = V;
    graph->E = E;
    graph->edgeCount = 0;
    graph->edgeList = (struct Edge*)malloc(E * sizeof(struct Edge));
    graph->adjMatrix = (int**)malloc(V * sizeof(int*));
    for (int i = 0; i < V; i++) 
    {
        graph->adjMatrix[i] = (int*)malloc(V * sizeof(int));
        for (int j = 0; j < V; j++) 
        {
            graph->adjMatrix[i][j] = 0;
        }
    }
    return graph;
}
void addEdge(struct Graph* graph, int u, int v, int weight) 
{
    graph->adjMatrix[u][v] = weight;
    if (graph->edgeCount < graph->E) 
    {
        graph->edgeList[graph->edgeCount].src = u;
        graph->edgeList[graph->edgeCount].dest = v;
        graph->edgeList[graph->edgeCount].weight = weight;
        graph->edgeCount++;
    }
}
void freeGraph(struct Graph* graph) 
{
    for (int i = 0; i < graph->V; i++) 
    {
        free(graph->adjMatrix[i]);
    }
    free(graph->adjMatrix);
    free(graph->edgeList);
    free(graph);
}
void printSolution(const char* algoName, int dist[], int V, double timeTaken) {
    printf("\n--- %s Results ---\n", algoName);
    if (dist) 
    {
        printf("Vertex \t Distance from Source\n");
        for (int i = 0; i < V; i++) 
        {
            if (dist[i] == INF) 
            {
                printf("%d \t\t INF\n", i);
            } 
            else 
            {
                printf("%d \t\t %d\n", i, dist[i]);
            }
        }
    } 
    else 
    {
        printf("No solution found (negative cycle detected).\n");
    }
    printf("Time taken: %.8f seconds\n", timeTaken);
}
int minDistance(int dist[], bool visited[], int V) 
{
    int min = INF, min_index = -1;
    for (int v = 0; v < V; v++) 
    {
        if (visited[v] == false && dist[v] <= min) 
        {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}
int* dijkstra(struct Graph* graph, int src) 
{
    int V = graph->V;
    int* dist = (int*)malloc(V * sizeof(int));
    bool* visited = (bool*)malloc(V * sizeof(bool));
    for (int i = 0; i < V; i++) 
    {
        dist[i] = INF;
        visited[i] = false;
    }
    dist[src] = 0;
    for (int count = 0; count < V - 1; count++) 
    {
        int u = minDistance(dist, visited, V);
        if (u == -1 || dist[u] == INF) break;
        visited[u] = true;
        for (int v = 0; v < V; v++) 
        {
            int weight = graph->adjMatrix[u][v];
            if (!visited[v] && weight != 0 && dist[u] != INF && 
                dist[u] + weight < dist[v]) 
            {
                dist[v] = dist[u] + weight;
            }
        }
    }
    free(visited);
    return dist;
}
int* bellmanFord(struct Graph* graph, int src) 
{
    int V = graph->V;
    int E = graph->edgeCount;
    int* dist = (int*)malloc(V * sizeof(int));
    for (int i = 0; i < V; i++) 
    {
        dist[i] = INF;
    }
    dist[src] = 0;
    for (int i = 1; i <= V - 1; i++) 
    {
        for (int j = 0; j < E; j++) 
        {
            int u = graph->edgeList[j].src;
            int v = graph->edgeList[j].dest;
            int weight = graph->edgeList[j].weight;
            if (dist[u] != INF && dist[u] + weight < dist[v]) 
            {
                dist[v] = dist[u] + weight;
            }
        }
    }
    for (int j = 0; j < E; j++) 
    {
        int u = graph->edgeList[j].src;
        int v = graph->edgeList[j].dest;
        int weight = graph->edgeList[j].weight;
        if (dist[u] != INF && dist[u] + weight < dist[v]) 
        {
            printf("\n[Bellman-Ford] Error: Graph contains a negative weight cycle\n");
            free(dist);
            return NULL; 
        }
    }

    return dist;
}
bool compareDistArrays(int* dist1, int* dist2, int V) 
{
    if (dist1 == NULL || dist2 == NULL) return false;
    for (int i = 0; i < V; i++) 
    {
        if (dist1[i] != dist2[i]) return false;
    }
    return true;
}
void runComparison(const char* graphName, struct Graph* graph, int src) 
{
    printf("\n=======================================================\n");
    printf(" %s (Source Vertex: %d)\n", graphName, src);
    printf("=======================================================\n");
    clock_t start_d = clock();
    int* dist_d = dijkstra(graph, src);
    clock_t end_d = clock();
    double time_d = ((double)(end_d - start_d)) / CLOCKS_PER_SEC;
    printSolution("Dijkstra's Algorithm", dist_d, graph->V, time_d);
    clock_t start_b = clock();
    int* dist_b = bellmanFord(graph, src);
    clock_t end_b = clock();
    double time_b = ((double)(end_b - start_b)) / CLOCKS_PER_SEC;
    printSolution("Bellman-Ford Algorithm", dist_b, graph->V, time_b);
    printf("\n--- Analysis ---\n");
    if (dist_b == NULL) 
    {
        printf("INFO: Bellman-Ford detected a negative cycle, as expected.\n");
    } 
    else if (compareDistArrays(dist_d, dist_b, graph->V)) {
        printf("SUCCESS: Both algorithms produced the same (correct) result.\n");
    } 
    else 
    {
        printf("WARNING: Dijkstra's result is INCORRECT due to negative weights.\n");
        printf("Bellman-Ford produced the correct result.\n");
    }
    free(dist_d);
    if (dist_b) free(dist_b);
}
int main() 
{
    int V, E;
    V = 5;
    E = 8;
    struct Graph* g1 = createGraph(V, E);
    addEdge(g1, 0, 1, 9);
    addEdge(g1, 0, 2, 6);
    addEdge(g1, 0, 3, 5);
    addEdge(g1, 0, 4, 3);
    addEdge(g1, 1, 4, 1);
    addEdge(g1, 2, 1, 2);
    addEdge(g1, 2, 3, 4);
    addEdge(g1, 1, 2, 0); 
    runComparison("Test Case 1: Positive Weights Graph", g1, 0);
    freeGraph(g1);
    V = 5;
    E = 8;
    struct Graph* g2 = createGraph(V, E);
    addEdge(g2, 0, 1, 1);
    addEdge(g2, 0, 2, 4);
    addEdge(g2, 1, 2, 3);
    addEdge(g2, 1, 3, 2);
    addEdge(g2, 1, 4, 2);
    addEdge(g2, 3, 2, 5);
    addEdge(g2, 3, 1, 1);
    addEdge(g2, 4, 3, -3);
    runComparison("Test Case 2: Negative Weights (No Cycle)", g2, 0);
    freeGraph(g2);
    V = 4;
    E = 4;
    struct Graph* g3 = createGraph(V, E);
    addEdge(g3, 0, 1, 1);
    addEdge(g3, 1, 2, -1);
    addEdge(g3, 2, 3, -1);
    addEdge(g3, 3, 1, -1); 
    runComparison("Test Case 3: Negative Weight Cycle", g3, 0);
    freeGraph(g3);

    return 0;
}