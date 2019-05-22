#include "bubble_sort.h"

void bubble_sort(ulong n, int *arr){
    ulong i;
    ulong j;
    for(i = 0; i < n; i++){
        for(j = i; j < n; j++){
            if(arr[j] < arr[i]){
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
}
