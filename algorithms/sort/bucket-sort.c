#include <stdio.h>
#include <stdlib.h>

static int num;
int numBuckets(int numElements)
{
    num = numElements;
    return num;
}
int hash(double *d)
{
    int bucket = num * (*d);
    return bucket;
}
int cmp(const void *e1, const void *e2)
{
    double e1_val = *(double *)e1;
    double e2_val = *(double *)e2;
    if (e1_val > e2_val)
        return 1;
    if (e1_val < e2_val)
        return -1;
    if (e1_val == e2_val)
        return 0;
}
/*above are example*/

typedef struct entry {
    void         *element;
    struct entry *next;
} Entry;
typedef struct {
    Entry   *head;
    int     size;
} Bucket;

static Bucket *buckets = NULL;
static int num = 0;


void extract(Bucket *buckets, void **ar, int n)
{
    int i;
    Entry *entry_tra, *tmp;
    int ar_index = 0;
    int low;

    for (i = 0; i < num; i++)
    {
        if (buckets[i].size == 0)
            continue;

        low = ar_index;
        entry_tra = buckets[i].head;
        ar[ar_index++] = entry_tra->element;
        tmp = entry_tra;
        entry_tra = entry_tra->next;
        free(tmp);

        while (entry_tra != NULL)
        {
            int i = ar_index - 1;
            while (i >= low && cmp(entry_tra->element, ar[i]) < 0)
            {
                ar[i+1] = ar[i];
                i--;
            }
            ar[i+1] = entry_tra->element;
            tmp = entry_tra;
            entry_tra = entry_tra->next;
            free(tmp);
            ar_index++;
        }
        buckets[i].size = 0;
    }
}
void bucket_sort(void **ar, int n)
{
    int i, k; 
    Entry *entry;
    num = numBuckets(n);
    buckets = calloc(num, sizeof(Bucket));
    for (i = 0; i < n; i++)
    {
        k = hash(ar[i]);
        entry = calloc(1, sizeof(Entry));
        entry->element = ar[i];
        if (buckets[k].head == NULL)
        {
            buckets[k].head = entry;
            entry->next = NULL;
        }
        else
        {
            entry->next = buckets[k].head;
            buckets[k].head = entry;
        }
        buckets[k].size++;
    }
    extract(buckets, ar, n);
    free(buckets);
    buckets = NULL;
}

int main(int argc, const char *argv[])
{
    double data[] = {0.111, 0.23, 0.1, 0.8, 0.98, 0.13, 0.53};
    int n = sizeof(data) / sizeof(double);
    int i;
    double *ar[n];

    for (i = 0; i < n; i++)
        ar[i] = &data[i];
    bucket_sort((void **)ar, n);
    for (i = 0; i < n; i++)
        printf("%f\n", *ar[i]);
    return 0;
}
