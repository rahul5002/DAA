#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>
#define MAX_V 100
#define MAX_W 100

struct Edge {
    int s, d, w;
};
struct Graph {
    int V, E;
    struct Edge* edge;
};
struct subset {
    int p;
    int rank;
};
struct Graph* create_Graph(int V, int E) {
    struct Graph* g = (struct Graph*)malloc(sizeof(struct Graph));
    g->V = V;
    g->E = E;
    g->edge = (struct Edge*)malloc(E * sizeof(struct Edge));
    return g;
}

int find(struct subset ss[], int i)
{
    if (ss[i].p != i)
        ss[i].p = find(ss, ss[i].p);
    return ss[i].p;
}
void Union(struct subset ss[], int x, int y)
{
    int xr = find(ss, x);
    int yr = find(ss, y);
    if (ss[xr].rank < ss[yr].rank)
        ss[xr].p = yr;
    else if (ss[xr].rank > ss[yr].rank)
        ss[yr].p = xr;
    else
    {
        ss[yr].p = xr;
        ss[xr].rank++;
    }
}
int Edges(const void* a, const void* b)
{
    return ((struct Edge*)a)->w - ((struct Edge*)b)->w;
}
void Kruskal_MST(struct Graph* g)
{
    int V = g->V;
    struct Edge res[V];
    int e = 0, i = 0;
    qsort(g->edge, g->E, sizeof(g->edge[0]), Edges);
    struct subset* ss = (struct subset*)malloc(V * sizeof(struct subset));
    for (int v = 0; v < V; ++v)
    {
        ss[v].p = v;
        ss[v].rank = 0;
    }
    while (e < V - 1 && i < g->E)
    {
        struct Edge next_e = g->edge[i++];
        int x = find(ss, next_e.s);
        int y = find(ss, next_e.d);
        if (x != y)
        {
            res[e++] = next_e;
            Union(ss, x, y);
        }
    }
    printf("Edge \tWeight\n");
    int total_w = 0;
    for (i = 0; i < e; ++i)
    {
        printf("%d - %d \t%d \n", res[i].s, res[i].d, res[i].w);
        total_w += res[i].w;
    }
    printf("Total MST Weight: %d\n", total_w);
    free(ss);
}
int min_Key(int k[], bool in_mst[], int V)
{
    int min = INT_MAX, min_idx = -1;
    for (int v = 0; v < V; v++)
    {
        if (in_mst[v] == false && k[v] < min)
        {
            min = k[v];
            min_idx = v;
        }
    }
    return min_idx;
}
void print_prim(int p[], int g[MAX_V][MAX_V], int V)
{
    printf("Edge \tWeight\n");
    int total_w = 0;
    for (int i = 1; i < V; i++)
    {
        printf("%d - %d \t%d \n", p[i], i, g[i][p[i]]);
        total_w += g[i][p[i]];
    }
    printf("Total MST Weight: %d\n", total_w);
}
void prim_MST(int g[MAX_V][MAX_V], int V)
{
    int p[V];
    int k[V];
    bool in_mst[V];
    for (int i = 0; i < V; i++)
    {
        k[i] = INT_MAX;
        in_mst[i] = false;
    }
    k[0] = 0;
    p[0] = -1;
    for (int count = 0; count < V - 1; count++)
    {
        int u = min_Key(k, in_mst, V);
        if(u == -1) break; 

        in_mst[u] = true;
        for (int v = 0; v < V; v++)
        {
            if (g[u][v] && in_mst[v] == false && g[u][v] < k[v])
            {
                p[v] = u;
                k[v] = g[u][v];
            }
        }
    }
    print_prim(p, g, V);
}
void Random_Graph(int V, int E, int p_g[MAX_V][MAX_V], struct Graph* k_g)
{
    for (int i = 0; i < V; i++)
    {
        for (int j = 0; j < V; j++)
        {
            p_g[i][j] = 0;
        }
    }
    int e_count = 0;
    int verts[V];
    for (int i = 0; i < V; i++) verts[i] = i;
    for (int i = 0; i < V; i++)
    {
        int temp = verts[i];
        int rand_idx = rand() % V;
        verts[i] = verts[rand_idx];
        verts[rand_idx] = temp;
    }
    for (int i = 0; i < V - 1; i++)
    {
        int u = verts[i];
        int v = verts[i + 1];
        int w = rand() % MAX_W + 1;
        p_g[u][v] = p_g[v][u] = w;
        k_g->edge[e_count++] = (struct Edge){u, v, w};
    }
    while (e_count < E)
    {
        int u = rand() % V;
        int v = rand() % V;
        if (u != v && p_g[u][v] == 0)
        {
            int w = rand() % MAX_W + 1;
            p_g[u][v] = p_g[v][u] = w;
            k_g->edge[e_count++] = (struct Edge){u, v, w};
        }
    }
}
int main()
{
    int V, E;
    clock_t start, end;
    double time_taken;
    srand(time(NULL));
    printf("Enter the number of vertices (max %d): ", MAX_V);
    scanf("%d", &V);
    if (V <= 1 || V > MAX_V)
    {
        printf("Invalid number of vertices.\n");
        return 1;
    }
    int min_e = V - 1;
    long long max_e = (long long)V * (V - 1) / 2;
    printf("Enter the number of edges (between %d and %lld): ", min_e, max_e);
    scanf("%d", &E);
    if (E < min_e || E > max_e)
    {
        printf("Invalid number of edges for a connected graph.\n");
        return 1;
    }
    int p_graph[MAX_V][MAX_V];
    struct Graph* k_graph = create_Graph(V, E);
    Random_Graph(V, E, p_graph, k_graph);
    printf("\nGenerated a random connected graph with %d vertices and %d edges.\n\n", V, E);
    printf("--- Prim's Algorithm MST ---\n");
    start = clock();
    prim_MST(p_graph, V);
    end = clock();
    time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken by Prim's Algorithm: %f seconds\n\n", time_taken);
    printf("--- Kruskal's Algorithm MST ---\n");
    start = clock();
    Kruskal_MST(k_graph);
    end = clock();
    time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken by Kruskal's Algorithm: %f seconds\n", time_taken);
    free(k_graph->edge);
    free(k_graph);

    return 0;
}