#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct Node 
{
    int data;
    struct Node* left;
    struct Node* right;
};
struct Node* createNode(int value) 
{
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = value;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}
// Recursive Insertion 
struct Node* insert_recur(struct Node* root, int value) 
{
    if (root == NULL) 
    {
        return createNode(value);
    }
    if (value < root->data) 
    {
        root->left = insert_recur(root->left, value);
    } else if (value > root->data) 
    {
        root->right = insert_recur(root->right, value);
    }
    return root;
}
// Recursive Inorder Traversal (left, root, right)
void inorder_recur(struct Node* root) 
{
    if (root != NULL) 
    {
        inorder_recur(root->left);
        printf("%d\t", root->data);
        inorder_recur(root->right);
    }
}
// Iterative Inorder Traversal
void inorder_iter(struct Node* root) 
{
    if (root == NULL) return;
    struct Node* stack[100];
    int top = -1;
    struct Node* current = root;
    while (current != NULL || top != -1) 
    {
        while (current != NULL) 
        {
            stack[++top] = current;
            current = current->left;
        }
        current = stack[top--];
        printf("%d\t", current->data);
        current = current->right;
    }
}
// Recursive Preorder Traversal (root, left, right)
void preorder_recur(struct Node* root) 
{
    if (root != NULL) 
    {
        printf("%d\t", root->data);
        preorder_recur(root->left);
        preorder_recur(root->right);
    }
}
// Iterative Preorder Traversal
void preorder_iter(struct Node* root) 
{
    if (root == NULL) return;
    struct Node* stack[100]; 
    int top = -1;
    stack[++top] = root;
    while (top != -1) 
    {
        struct Node* current = stack[top--];
        printf("%d\t", current->data);
        if (current->right != NULL) 
        {
            stack[++top] = current->right;
        }
        if (current->left != NULL) 
        {
            stack[++top] = current->left;
        }
    }
}
// Recursive Postorder Traversal (left, right, root)
void postorder_recur(struct Node* root) 
{
    if (root != NULL) 
    {
        postorder_recur(root->left);
        postorder_recur(root->right);
        printf("%d\t", root->data);
    }
}
// Iterative Postorder Traversal (using two stacks)
void postorder_iter(struct Node* root) 
{
    if (root == NULL) return;
    struct Node* stack1[100];
    struct Node* stack2[100];
    int top1 = -1, top2 = -1;
    stack1[++top1] = root;
    while (top1 != -1) 
    {
        struct Node* current = stack1[top1--];
        stack2[++top2] = current;
        if (current->left != NULL) 
        {
            stack1[++top1] = current->left;
        }
        if (current->right != NULL) 
        {
            stack1[++top1] = current->right;
        }
    }
    while (top2 != -1) 
    {
        printf("%d\t", stack2[top2--]->data);
    }
}
int main() 
{
    struct Node* root = NULL;
    int n, value, choice;
    printf("Enter the number of elements for the BST: ");
    scanf("%d", &n);
    for (int i = 0; i < n; i++) 
    {
        printf("Enter element %d: ", i + 1);
        scanf("%d", &value);
        root = insert_recur(root, value);
    }

    // Variables for timing
    clock_t start, end;
    double time_taken;
    while (1) 
    {
        printf("\nMenu:\n1. Inorder Traversal (Recursive)\n2. Inorder Traversal (Iterative)\n3. Preorder Traversal (Recursive)\n4. Preorder Traversal (Iterative)\n5. Postorder Traversal (Recursive)\n6. Postorder Traversal (Iterative)\n7. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        switch (choice) 
        {
            case 1:
                printf("Inorder Traversal (Recursive): ");
                start = clock();
                inorder_recur(root);
                end = clock();
                time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
                printf("\nTime taken: %f seconds\n", time_taken);
                break;
            case 2:
                printf("Inorder Traversal (Iterative): ");
                start = clock();
                inorder_iter(root);
                end = clock();
                time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
                printf("\nTime taken: %f seconds\n", time_taken);
                break;
            case 3:
                printf("Preorder Traversal (Recursive): ");
                start = clock();
                preorder_recur(root);
                end = clock();
                time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
                printf("\nTime taken: %f seconds\n", time_taken);
                break;
            case 4:
                printf("Preorder Traversal (Iterative): ");
                start = clock();
                preorder_iter(root);
                end = clock();
                time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
                printf("\nTime taken: %f seconds\n", time_taken);
                break;
            case 5:
                printf("Postorder Traversal (Recursive): ");
                start = clock();
                postorder_recur(root);
                end = clock();
                time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
                printf("\nTime taken: %f seconds\n", time_taken);
                break;
            case 6:
                printf("Postorder Traversal (Iterative): ");
                start = clock();
                postorder_iter(root);
                end = clock();
                time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
                printf("\nTime taken: %f seconds\n", time_taken);
                break;
            case 7:
                return 0;
            default:
                printf("Invalid choice!\n");
        }
    }
    return 0;
}