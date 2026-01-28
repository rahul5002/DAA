#include <stdio.h>
#include <string.h>
#define d 256
#define q 101

void naive_search(const char *text, const char *pattern) 
{
    int n = strlen(text);
    int m = strlen(pattern);
    for (int s = 0; s <= n - m; s++) 
    {
        int j;
        for (j = 0; j < m; j++) 
        {
            if (text[s + j] != pattern[j]) 
            {
                break; 
            }
        }
        if (j == m) 
        {
            printf("  -> Pattern found at index %d\n", s);
        }
    }
}
void rabin_karp_search(const char *text, const char *pattern) 
{
    int n = strlen(text);
    int m = strlen(pattern);
    int p_hash = 0; 
    int t_hash = 0; 
    int h = 1;      
    for (int i = 0; i < m - 1; i++) 
    {
        h = (h * d) % q;
    }
    for (int i = 0; i < m; i++) 
    {
        p_hash = (d * p_hash + pattern[i]) % q;
        t_hash = (d * t_hash + text[i]) % q;
    }
    for (int s = 0; s <= n - m; s++) 
    {        
        if (p_hash == t_hash) 
        {            
            int match = 1; 
            for (int j = 0; j < m; j++) 
            {
                if (text[s + j] != pattern[j]) 
                {
                    match = 0; 
                    break;
                }
            }
            if (match) 
            {
                printf("  -> Pattern found at index %d\n", s);
            }
        }
        if (s < n - m) 
        {
            t_hash = (d * (t_hash - text[s] * h) + text[s + m]) % q;
            if (t_hash < 0) 
            {
                t_hash = t_hash + q;
            }
        }
    }
}

int main() 
{
    const char *text_to_search = "AABAACAADAABAABA";
    const char *pattern_to_find = "AABA";
    printf("Demonstrating String Matching Algorithms\n");
    printf("========================================\n");
    printf("Text:    %s\n", text_to_search);
    printf("Pattern: %s\n", pattern_to_find);
    printf("----------------------------------------\n");
    printf("[1] Naive Search:\n");
    naive_search(text_to_search, pattern_to_find);
    printf("----------------------------------------\n");
    printf("[2] Rabin-Karp Search:\n");
    rabin_karp_search(text_to_search, pattern_to_find);
    printf("----------------------------------------\n");

    return 0;
}