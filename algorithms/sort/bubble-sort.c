#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void bubbleSort(int *A, int n)
{
    int i, j;
    int tmp;
    int hasSwap;

    for (i = n; i > 0; i--)
    {
        hasSwap = 0;
        for (j = 1; j < i; j++)
        {
            if (A[j] < A[j-1])
            {
                tmp = A[j];
                A[j] = A[j-1];
                A[j-1] = tmp;
                hasSwap = 1;
            }
        }
        if (!hasSwap)
            break;
    }
}


int main()
{
    int A[] = {5,3,0,4,2,1,-1};
    int length = sizeof(A) / sizeof(int);
    bubbleSort(A, length);

    int i;
    for (i = 0; i < length; i++)
        printf("%d, ", A[i]);
    printf("\n");
    return 0;
}
