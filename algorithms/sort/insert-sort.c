#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void insertSort(void *base, int n, size_t size,
                int (*cmp)(const void *, const void *))
{
    int i;
    void *tmp = malloc(size);

    for (i = 1; i < n; i++)
    {
        int j = i - 1;
        void *value = base + i * size;

        while (j >= 0 && cmp(value, base+j*size) < 0)
            j--;
        if (++j == i) continue;
        memmove(tmp, value, size);
        memmove(base+(j+1)*size, base+j*size, (i-j)*size);
        memmove(base+j*size, tmp, size);
    }
}
int intcmp(const void *e1, const void *e2)
{
    return *(int *)e1 - *(int *)e2;
}
int main()
{
    int A[] = {2,3, -1, 0, 5, 7, -2, -5};
    int length = sizeof(A) / sizeof(int);

    insertSort(A, length, sizeof(int), intcmp);

    int i;
    for (i = 0; i < length; i++)
        printf("%d, ", A[i]);
    printf("\n");
    return 0;
}
