#include <stdio.h>

void swap(int *a, int *b)
{
    int swap_temp = *a;
    *a = *b;
    *b = swap_temp;
}
int get_pivot(int p, int r)
{
    return r;
}
int partition(int a[], int p, int r)
{
    int i = get_pivot(p, r);
    int x = a[i];
    int j;
    
    i = p - 1;
    for (j = p; j < r; j++) 
    {
        if (a[j] <= x)
        {
           i++;
           swap(&a[i], &a[j]);
        }
    }
    swap(&a[++i], &a[r]);
    return i;
}
void quicksort(int a[], int p, int r)
{
    int q;
    if (r > p)
    {
        q = partition(a, p, r);
        quicksort(a, p, q-1);
        quicksort(a, q+1, r);
    }
}
int main(int argc, const char *argv[])
{
    int a[] = {1, 3, 4, 5, 0, 0, 7, -1};
    int length = sizeof(a) / sizeof(int);

    quicksort(a, 0, length-1);
    
    int i;
    for (i = 0; i < length; i++)
        printf("%d\t", a[i]);
    printf("\n");
    return 0;
}
