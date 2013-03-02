#include <stdio.h>

#define LENGTH 10

void count_sort_simple(int a[], int length, int max_num)
{
    max_num++;
    int b[max_num];
    int i;

    for (i = 0; i < max_num; i++)
        b[i] = 0;
    for (i = 0; i < length; i++)
        b[a[i]]++;

    int idx = 0;
    for (i = 0; i < max_num; i++)
    {
        while (b[i] > 0)
        {
            a[idx++] = i;
            b[i]--;
        }
    }
}
void count_sort(int a[], int result[], int length, int max_num)
{
    max_num++; //0-max_num, so the length of array 'c' is max_num+1
    int c[max_num]; 
    int i;

    for (i = 0; i < max_num; i++)
        c[i] = 0;
    for (i = 0; i < length; i++)
        c[a[i]]++;
    for (i = 1; i < max_num; i++)
        c[i] += c[i-1];

    for (i = length - 1; i >= 0; i--)
    {
        result[c[a[i]]-1] = a[i];
        c[a[i]]--;
    }
}
int main(int argc, const char *argv[])
{
    int a[LENGTH];
    int result[LENGTH];
    int i;

    for (i = 0; i < LENGTH; i++)
        a[i] = 10 - i;
    printf("use count_sort:\t\t");
    count_sort(a, result, LENGTH, LENGTH);
    for (i = 0; i < LENGTH; i++)
        printf("%d\t", result[i]);
    printf("\n");

    printf("use count_sort_simple:\t");
    count_sort_simple(a, LENGTH, LENGTH);
    for (i = 0; i < LENGTH; i++)
        printf("%d\t", a[i]);
    printf("\n");
}
