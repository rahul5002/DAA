#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Merge Sort 
void merge(int arr[], int l, int m, int r) 
{
    int n1 = m - l + 1;
    int n2 = r - m;
    int* L = (int*)malloc(n1 * sizeof(int));
    int* R = (int*)malloc(n2 * sizeof(int));
    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];
    
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) 
    {
        if (L[i] <= R[j]) 
        {
            arr[k] = L[i];
            i++;
        } 
        else 
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1) 
    {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) 
    {
        arr[k] = R[j];
        j++;
        k++;
    }
    free(L);
    free(R);
}
void mergeSort(int arr[], int l, int r) 
{
    if (l < r) 
    {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}
// Quick Sort 
void swap(int* a, int* b) 
{
    int t = *a;
    *a = *b;
    *b = t;
}
int partition(int arr[], int low, int high) 
{
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) 
    {
        if (arr[j] <= pivot) 
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}
void quickSort(int arr[], int low, int high) 
{
    if (low < high) 
    {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}
void printArray(int arr[], int n) 
{
    for (int i = 0; i < n; i++) 
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}
// Performance Comparison
void compare() 
{
    int size;
    printf("Enter the size of the array: ");
    scanf("%d", &size);
    if (size <= 0) 
    {
        printf("Invalid size! Please enter a positive number.\n");
        return;
    }
    int* arr = (int*)malloc(size * sizeof(int));
    int* mergeArr = (int*)malloc(size * sizeof(int));
    int* quickArr = (int*)malloc(size * sizeof(int));
    if (arr == NULL || mergeArr == NULL || quickArr == NULL) 
    {
        printf("Memory allocation failed!\n");
        free(arr);
        free(mergeArr);
        free(quickArr);
        return;
    }
    // Generate random array
    srand(time(NULL)); // Seed with current time for randomness
    for (int i = 0; i < size; i++) 
    {
        arr[i] = rand() % 1000000 + 1; 
        mergeArr[i] = arr[i];
        quickArr[i] = arr[i];
    }

    printf("\nOriginal array: ");
    printArray(arr, size);

    clock_t start = clock();
    mergeSort(mergeArr, 0, size - 1);
    double mergeTime = (double)(clock() - start) / CLOCKS_PER_SEC;

    start = clock();
    quickSort(quickArr, 0, size - 1);
    double quickTime = (double)(clock() - start) / CLOCKS_PER_SEC;

    printf("Merge Sort Result: ");
    printArray(mergeArr, size);
    printf("Quick Sort Result: ");
    printArray(quickArr, size);

    printf("\nMerge Sort Time: %.6f seconds\n", mergeTime);
    printf("Quick Sort Time: %.6f seconds\n", quickTime);

    free(arr);
    free(mergeArr);
    free(quickArr);
}
int main() 
{
    compare();
    return 0;
}