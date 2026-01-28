#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int index;
    int value;
    int weight;
    double ratio;
} Item;
int compare(const void *a, const void *b) 
{
    double ratio1 = ((Item *)a)->ratio;
    double ratio2 = ((Item *)b)->ratio;
    return (ratio2 > ratio1) ? 1 : (ratio2 < ratio1) ? -1 : 0;
}
void fractional_knapsack(int values[], int weights[], int n, int capacity, double *total_value, int *selected_count, int selected[]) 
{
    Item *items = (Item *)malloc(n * sizeof(Item));
    for (int i = 0; i < n; i++) 
    {
        items[i].index = i;
        items[i].value = values[i];
        items[i].weight = weights[i];
        items[i].ratio = (double)values[i] / weights[i];
    }
    qsort(items, n, sizeof(Item), compare);
    *total_value = 0.0;
    int remaining_capacity = capacity;
    *selected_count = 0;
    for (int i = 0; i < n; i++) 
    {
        if (remaining_capacity >= items[i].weight) 
        {
            selected[(*selected_count)++] = items[i].index;
            *total_value += items[i].value;
            remaining_capacity -= items[i].weight;
        } 
        else 
        {
            double fraction = (double)remaining_capacity / items[i].weight;
            selected[(*selected_count)++] = items[i].index;
            *total_value += items[i].value * fraction;
            remaining_capacity = 0;
            break; 
        }
    }
    free(items);
}
int zero_one_knapsack(int values[], int weights[], int n, int capacity, int selected[]) 
{
    int **dp = (int **)malloc((n + 1) * sizeof(int *));
    for (int i = 0; i <= n; i++) 
    {
        dp[i] = (int *)malloc((capacity + 1) * sizeof(int));
        for (int w = 0; w <= capacity; w++) 
        {
            dp[i][w] = 0;
        }
    }
    for (int i = 1; i <= n; i++) 
    {
        for (int w = 0; w <= capacity; w++) 
        {
            if (weights[i-1] <= w) 
            {
                dp[i][w] = (values[i-1] + dp[i-1][w - weights[i-1]] > dp[i-1][w])
                           ? values[i-1] + dp[i-1][w - weights[i-1]]
                           : dp[i-1][w];
            } 
            else 
            {
                dp[i][w] = dp[i-1][w];
            }
        }
    }
    int selected_count = 0;
    int w = capacity;
    for (int i = n; i > 0 && w > 0; i--) 
    {
        if (dp[i][w] != dp[i-1][w]) 
        {
            selected[selected_count++] = i-1;
            w -= weights[i-1];
        }
    }
    int max_value = dp[n][capacity];
    for (int i = 0; i <= n; i++) 
    {
        free(dp[i]);
    }
    free(dp);
    for (int i = 0; i < selected_count / 2; i++) 
    {
        int temp = selected[i];
        selected[i] = selected[selected_count - 1 - i];
        selected[selected_count - 1 - i] = temp;
    }

    return max_value;
}
void generate_random_data(int n, int max_capacity, int values[], int weights[], int *capacity) 
{
    for (int i = 0; i < n; i++) 
    {
        values[i] = rand() % 91 + 10;
        weights[i] = rand() % 26 + 5;
    }
    *capacity = rand() % (max_capacity / 2 + 1) + max_capacity / 2;
}
int main() 
{
    srand(42); 
    int test_cases[][2] = 
    {
        {100, 500},
        {1000, 5000},
        {5000, 25000}
    };
    int num_tests = 3;
    printf("Comparison of Fractional and 0/1 Knapsack Algorithms (Profit and Time)\n");
    printf("-----------------------------------------------------------------------------------------------\n");
    printf("%-10s %-12s %-15s %-15s %-15s %-15s\n", "n", "Capacity", "Frac Profit", "0/1 Profit", "Fractional (s)", "0/1 Knapsack (s)");
    printf("-----------------------------------------------------------------------------------------------\n");
    for (int t = 0; t < num_tests; t++) 
    {
        int n = test_cases[t][0];
        int max_capacity = test_cases[t][1];
        int *values = (int *)malloc(n * sizeof(int));
        int *weights = (int *)malloc(n * sizeof(int));
        int *selected = (int *)malloc(n * sizeof(int));
        int capacity;
        generate_random_data(n, max_capacity, values, weights, &capacity);
        clock_t start = clock();
        double frac_profit;
        int selected_count;
        for (int i = 0; i < 5; i++) 
        {
            fractional_knapsack(values, weights, n, capacity, &frac_profit, &selected_count, selected);
        }
        double frac_time = (double)(clock() - start) / CLOCKS_PER_SEC / 5;
        start = clock();
        int zero_one_profit;
        for (int i = 0; i < 5; i++) 
        {
            zero_one_profit = zero_one_knapsack(values, weights, n, capacity, selected);
        }
        double zero_one_time = (double)(clock() - start) / CLOCKS_PER_SEC / 5;
        printf("%-10d %-12d %-15.2f %-15d %-15.6f %-15.6f\n", n, capacity, frac_profit, zero_one_profit, frac_time, zero_one_time);
        free(values);
        free(weights);
        free(selected);
    }
    return 0;
}