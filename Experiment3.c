#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int** allocate_matrix(int n) 
{
    int** matrix = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) 
    {
        matrix[i] = (int*)malloc(n * sizeof(int));
    }
    return matrix;
}
void free_matrix(int** matrix, int n) 
{
    for (int i = 0; i < n; i++) 
    {
        free(matrix[i]);
    }
    free(matrix);
}
void print_matrix(int** matrix, int n) 
{
    for (int i = 0; i < n; i++) 
    {
        for (int j = 0; j < n; j++) 
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}
// Traditional Matrix Multiplication
void traditional_multiply(int** A, int** B, int** C, int n) 
{
    for (int i = 0; i < n; i++) 
    {
        for (int j = 0; j < n; j++) 
        {
            C[i][j] = 0;
            for (int k = 0; k < n; k++) 
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}
// Matrix Addition for Strassen
void matrix_add(int** A, int** B, int** result, int n) 
{
    for (int i = 0; i < n; i++) 
    {
        for (int j = 0; j < n; j++) 
        {
            result[i][j] = A[i][j] + B[i][j];
        }
    }
}
// Matrix Subtraction for Strassen
void matrix_subtract(int** A, int** B, int** result, int n) 
{
    for (int i = 0; i < n; i++) 
    {
        for (int j = 0; j < n; j++) 
        {
            result[i][j] = A[i][j] - B[i][j];
        }
    }
}
// Strassen Matrix Multiplication (handles arbitrary n by padding)
void strassen_multiply(int** A, int** B, int** C, int n) 
{
    // If n is 1, perform direct multiplication
    if (n <= 1) 
    {
        C[0][0] = A[0][0] * B[0][0];
        return;
    }

    // Find the next power of 2 for padding
    int padded_size = 1;
    while (padded_size < n) 
    {
        padded_size *= 2;
    }

    // If n is not a power of 2, pad the matrices
    if (padded_size != n) 
    {
        // Allocate padded matrices
        int** A_padded = allocate_matrix(padded_size);
        int** B_padded = allocate_matrix(padded_size);
        int** C_padded = allocate_matrix(padded_size);

        // Initialize padded matrices with zeros
        for (int i = 0; i < padded_size; i++) 
        {
            for (int j = 0; j < padded_size; j++) 
            {
                A_padded[i][j] = 0;
                B_padded[i][j] = 0;
                C_padded[i][j] = 0;
            }
        }

        // Copy original matrices into padded matrices
        for (int i = 0; i < n; i++) 
        {
            for (int j = 0; j < n; j++) 
            {
                A_padded[i][j] = A[i][j];
                B_padded[i][j] = B[i][j];
            }
        }
        // Perform Strassen multiplication on padded matrices
        strassen_multiply(A_padded, B_padded, C_padded, padded_size);
        // Copy result back to original C matrix
        for (int i = 0; i < n; i++) 
        {
            for (int j = 0; j < n; j++) 
            {
                C[i][j] = C_padded[i][j];
            }
        }
        // Free padded matrices
        free_matrix(A_padded, padded_size);
        free_matrix(B_padded, padded_size);
        free_matrix(C_padded, padded_size);
        return;
    }
    // Allocate submatrices for power of 2 case
    int new_size = n / 2;
    int** A11 = allocate_matrix(new_size);
    int** A12 = allocate_matrix(new_size);
    int** A21 = allocate_matrix(new_size);
    int** A22 = allocate_matrix(new_size);
    int** B11 = allocate_matrix(new_size);
    int** B12 = allocate_matrix(new_size);
    int** B21 = allocate_matrix(new_size);
    int** B22 = allocate_matrix(new_size);
    int** M1 = allocate_matrix(new_size);
    int** M2 = allocate_matrix(new_size);
    int** M3 = allocate_matrix(new_size);
    int** M4 = allocate_matrix(new_size);
    int** M5 = allocate_matrix(new_size);
    int** M6 = allocate_matrix(new_size);
    int** M7 = allocate_matrix(new_size);
    int** temp1 = allocate_matrix(new_size);
    int** temp2 = allocate_matrix(new_size);

    // Divide matrices A and B into quadrants
    for (int i = 0; i < new_size; i++) 
    {
        for (int j = 0; j < new_size; j++) 
        {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + new_size];
            A21[i][j] = A[i + new_size][j];
            A22[i][j] = A[i + new_size][j + new_size];
            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j + new_size];
            B21[i][j] = B[i + new_size][j];
            B22[i][j] = B[i + new_size][j + new_size];
        }
    }
    // Compute the 7 products (M1 to M7) recursively
    // M1 = (A11 + A22) * (B11 + B22)
    matrix_add(A11, A22, temp1, new_size);
    matrix_add(B11, B22, temp2, new_size);
    strassen_multiply(temp1, temp2, M1, new_size);
    // M2 = (A21 + A22) * B11
    matrix_add(A21, A22, temp1, new_size);
    strassen_multiply(temp1, B11, M2, new_size);
    // M3 = A11 * (B12 - B22)
    matrix_subtract(B12, B22, temp1, new_size);
    strassen_multiply(A11, temp1, M3, new_size);
    // M4 = A22 * (B21 - B11)
    matrix_subtract(B21, B11, temp1, new_size);
    strassen_multiply(A22, temp1, M4, new_size);
    // M5 = (A11 + A12) * B22
    matrix_add(A11, A12, temp1, new_size);
    strassen_multiply(temp1, B22, M5, new_size);
    // M6 = (A21 - A11) * (B11 + B12)
    matrix_subtract(A21, A11, temp1, new_size);
    matrix_add(B11, B12, temp2, new_size);
    strassen_multiply(temp1, temp2, M6, new_size);
    // M7 = (A12 - A22) * (B21 + B22)
    matrix_subtract(A12, A22, temp1, new_size);
    matrix_add(B21, B22, temp2, new_size);
    strassen_multiply(temp1, temp2, M7, new_size);
    // Compute quadrants of result matrix C
    for (int i = 0; i < new_size; i++) 
    {
        for (int j = 0; j < new_size; j++) 
        {
            // C11 = M1 + M4 - M5 + M7
            C[i][j] = M1[i][j] + M4[i][j] - M5[i][j] + M7[i][j];
            // C12 = M3 + M5
            C[i][j + new_size] = M3[i][j] + M5[i][j];
            // C21 = M2 + M4
            C[i + new_size][j] = M2[i][j] + M4[i][j];
            // C22 = M1 - M2 + M3 + M6
            C[i + new_size][j + new_size] = M1[i][j] - M2[i][j] + M3[i][j] + M6[i][j];
        }
    }
    // Free allocated memory
    free_matrix(A11, new_size);
    free_matrix(A12, new_size);
    free_matrix(A21, new_size);
    free_matrix(A22, new_size);
    free_matrix(B11, new_size);
    free_matrix(B12, new_size);
    free_matrix(B21, new_size);
    free_matrix(B22, new_size);
    free_matrix(M1, new_size);
    free_matrix(M2, new_size);
    free_matrix(M3, new_size);
    free_matrix(M4, new_size);
    free_matrix(M5, new_size);
    free_matrix(M6, new_size);
    free_matrix(M7, new_size);
    free_matrix(temp1, new_size);
    free_matrix(temp2, new_size);
}
int main() 
{
    int n;
    printf("Enter the dimension of the square matrix (1 or greater): ");
    scanf("%d", &n);
    // Validate input
    if (n < 1) 
    {
        printf("Error: Dimension must be 1 or greater.\n");
        return 1;
    }
    // Seed random number generator
    srand(time(NULL));
    // Allocate matrices
    int** A = allocate_matrix(n);
    int** B = allocate_matrix(n);
    int** C_traditional = allocate_matrix(n);
    int** C_strassen = allocate_matrix(n);
    // Initialize matrices A and B with random values (0 to 9)
    for (int i = 0; i < n; i++) 
    {
        for (int j = 0; j < n; j++) 
        {
            A[i][j] = rand() % 10;
            B[i][j] = rand() % 10;
        }
    }
    printf("Matrix A:\n");
    print_matrix(A, n);
    printf("\nMatrix B:\n");
    print_matrix(B, n);

    // Measure time for traditional multiplication
    clock_t start_traditional = clock();
    traditional_multiply(A, B, C_traditional, n);
    clock_t end_traditional = clock();
    double time_traditional = ((double)(end_traditional - start_traditional)) / CLOCKS_PER_SEC;
    printf("\nTraditional Multiplication Result:\n");
    print_matrix(C_traditional, n);
    printf("Time taken for Traditional Multiplication: %f seconds\n", time_traditional);

    // Measure time for Strassen multiplication
    clock_t start_strassen = clock();
    strassen_multiply(A, B, C_strassen, n);
    clock_t end_strassen = clock();
    double time_strassen = ((double)(end_strassen - start_strassen)) / CLOCKS_PER_SEC;
    printf("\nStrassen Multiplication Result:\n");
    print_matrix(C_strassen, n);
    printf("Time taken for Strassen Multiplication: %f seconds\n", time_strassen);

    // Free matrices
    free_matrix(A, n);
    free_matrix(B, n);
    free_matrix(C_traditional, n);
    free_matrix(C_strassen, n);
    return 0;
}