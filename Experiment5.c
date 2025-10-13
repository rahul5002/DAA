#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h> 
#include <limits.h>
#define MAX_JOBS 10
#define MAX_DEADLINE 5
#define MAX_PROFIT 150

struct Job {
    char id;
    int deadline;
    int profit;
};
int comparison(const void* a, const void* b) 
{
    struct Job* jobA = (struct Job*)a;
    struct Job* jobB = (struct Job*)b;
    return jobB->profit - jobA->profit;
}
void MaxProfit_BruteForce(
    struct Job arr[], int n, int max_d, 
    int index, 
    int current_profit, 
    bool current_slots[], 
    char current_schedule[], 
    int* max_profit, 
    char final_schedule[] 
) 
{
    if (index == n) 
    {
        if (current_profit > *max_profit) 
        {
            *max_profit = current_profit;
            for (int i = 0; i < max_d; i++) 
            {
                final_schedule[i] = current_schedule[i];
            }
        }
        return;
    }
    MaxProfit_BruteForce(
        arr, n, max_d, 
        index + 1, 
        current_profit, 
        current_slots, 
        current_schedule, 
        max_profit, 
        final_schedule
    );
    struct Job current_job = arr[index];
    for (int j = current_job.deadline - 1; j >= 0; j--) 
    {
        if (!current_slots[j]) 
        {
            current_slots[j] = true;
            char temp_char = current_schedule[j]; 
            current_schedule[j] = current_job.id; 
            MaxProfit_BruteForce(
                arr, n, max_d, 
                index + 1, 
                current_profit + current_job.profit, 
                current_slots, 
                current_schedule, 
                max_profit, 
                final_schedule
            );
            current_slots[j] = false;
            current_schedule[j] = temp_char; 
            break;
        }
    }
}
void jobSched_BruteForce(struct Job arr[], int n, int max_d) 
{
    int max_profit = 0;
    bool current_slots[MAX_DEADLINE] = {false};
    char current_schedule[MAX_DEADLINE];
    for (int i = 0; i < max_d; i++) current_schedule[i] = ' '; 
    char final_schedule[MAX_DEADLINE];
    for (int i = 0; i < max_d; i++) final_schedule[i] = ' ';
    MaxProfit_BruteForce(
        arr, n, max_d, 
        0, 
        0, 
        current_slots, 
        current_schedule, 
        &max_profit, 
        final_schedule
    );
    int scheduled_jobs = 0;
    printf("\n--- Optimal Job Schedule (Brute Force Result) ---\n");
    printf("Max Schedule Length (Max Deadline): %d\n", max_d);
    printf("-------------------------------------------\n");
    printf("Slot | Job ID | Profit\n");
    printf("-------------------------------------------\n");
    for (int i = 0; i < max_d; i++) 
    {
        if (final_schedule[i] != ' ') 
        {
            scheduled_jobs++;
            int job_index = -1;
            for(int k=0; k<n; k++) 
            {
                if (arr[k].id == final_schedule[i]) 
                {
                    job_index = k;
                    break;
                }
            }
            if (job_index != -1) 
            {
                 printf(" %2d  |   %c    |  %d\n", 
                         i + 1, arr[job_index].id, arr[job_index].profit);
            }
        }
    }
    printf("-------------------------------------------\n");
    printf("Jobs Scheduled: %d\n", scheduled_jobs);
    printf("Total Maximum Profit: %d\n", max_profit);
}
void jobSched_Greedy(struct Job arr[], int n, int max_d) 
{
    qsort(arr, n, sizeof(struct Job), comparison);
    int result[max_d]; 
    bool slot[max_d]; 
    for (int i = 0; i < max_d; i++) 
    {
        slot[i] = false;
    }
    int total_profit = 0;
    int scheduled_jobs = 0; 
    for (int i = 0; i < n; i++) 
    {
        struct Job current_job = arr[i];
        for (int j = current_job.deadline - 1; j >= 0; j--) 
        {
            if (slot[j] == false) 
            {
                result[j] = i; 
                slot[j] = true; 
                total_profit += current_job.profit;
                scheduled_jobs++;
                break; 
            }
        }
    }
    printf("\n--- Optimal Job Schedule (Greedy Result) ---\n");
    printf("Jobs Scheduled: %d\n", scheduled_jobs);
    printf("Max Schedule Length (Max Deadline): %d\n", max_d);
    printf("-------------------------------------------\n");
    printf("Slot | Job ID | Deadline | Profit\n");
    printf("-------------------------------------------\n");
    for (int i = 0; i < max_d; i++) 
    {
        if (slot[i]) 
        {
            printf(" %2d  |   %c    |    %d     |  %d\n", 
                    i + 1, arr[result[i]].id, arr[result[i]].deadline, arr[result[i]].profit);
        }
    }
    printf("-------------------------------------------\n");
    printf("Total Maximum Profit: %d\n", total_profit);
}
int main() 
{
    srand(time(0)); 
    int n = 5 + (rand() % (MAX_JOBS - 4)); 
    struct Job jobs[MAX_JOBS];
    int max_deadline = 0;
    printf("--- Randomly Generated Jobs (N=%d) ---\n", n);
    printf("Index | Job ID | Deadline | Profit\n");
    printf("-----------------------------------\n");
    for (int i = 0; i < n; i++) 
    {
        jobs[i].id = 'A' + i; 
        jobs[i].deadline = 1 + (rand() % MAX_DEADLINE); 
        jobs[i].profit = 10 + (rand() % (MAX_PROFIT - 9)); 
        if (jobs[i].deadline > max_deadline) 
        {
            max_deadline = jobs[i].deadline;
        }
        printf(" %4d |   %c    |    %d     |  %d\n", 
                i + 1, jobs[i].id, jobs[i].deadline, jobs[i].profit);
    }
    printf("-----------------------------------\n");
    if (max_deadline == 0) max_deadline = 1;
    jobSched_Greedy(jobs, n, max_deadline);
    jobSched_BruteForce(jobs, n, max_deadline);
    return 0;
}