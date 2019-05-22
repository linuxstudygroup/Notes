#include <stdlib.h>
#include "merge_sort.h"

static void Merge(int *array, ulong left, ulong m, ulong right, ulong n)
{
    int* aux = (int *)malloc(sizeof(int) * n);
    ulong i;
    ulong j;
    ulong k;

    for (i = left, j = m+1, k = 0; k <= right-left; k++)
    {
        if (i == m+1)
        {
            aux[k] = array[j++];
            continue;
        }
        if (j == right+1)
        {
            aux[k] = array[i++];
            continue;
        }
        if (array[i] < array[j])
        {
            aux[k] = array[i++];
        }
        else
        {
            aux[k] = array[j++];
        }
    }

    for (i = left, j = 0; i <= right; i++, j++)
    {
        array[i] = aux[j];
    }
}

static void MergeSort(int *array, ulong start, ulong end, ulong n)
{
    if (start < end)
    {
        ulong i;
        i = (end + start) / 2;
        MergeSort(array, start, i, n);
        MergeSort(array, i + 1, end, n);
        Merge(array, start, i, end, n);
    }
}

void divide_and_conquer_sort(ulong n, int * arr){
    MergeSort(arr, 0, n-1, n);
}
