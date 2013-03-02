#include <stdio.h>

#define LEFT(i)     (2 * (i) + 1)
#define RIGHT(i)    (2 * (i) + 2)

void swap(int *a, int *b)
{
    int swap_temp;
    swap_temp = *a;
    *a = *b;
    *b = swap_temp;
}
void max_heapify(int a[], int i, int length)
{
   int left = LEFT(i);
   int right = RIGHT(i);
   int largest;

   if (left < length && a[left] > a[i])
       largest = left;
   else
       largest = i;
   if (right < length && a[right] > a[largest])
       largest = right;

   if (largest != i)
   {
       swap(&a[largest], &a[i]);
       max_heapify(a, largest, length);
   }
}
void build_max_heap(int a[], int length)
{
    int i;
    for (i = length / 2 - 1; i >= 0; i--)
        max_heapify(a, i, length);
}
void heapsort(int a[], int length)
{
    build_max_heap(a, length);

    int i;
    for (i = length; i >= 2; i--)
    {
        swap(&a[0], &a[i-1]);
        length--;
        max_heapify(a, 0, length);
    }
}
int main(int argc, const char *argv[])
{
    int a[] = {2, 3, 4, 5, 1, 2, 7, 0};
    int length = sizeof(a) / sizeof(int);
    heapsort(a, length);

    int i;
    for (i = 0; i < length; i++)
        printf("%d\t", a[i]);
    printf("\n");
    return 0;
}
