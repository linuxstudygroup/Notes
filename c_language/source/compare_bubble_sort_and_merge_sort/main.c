#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

#include "my_type.h"
#include "merge_sort.h"
#include "bubble_sort.h"

void deep_copy(ulong n, const int * arr, int *dest_arr){
    ulong i;
    for(i = 0; i < n; i++){
        dest_arr[i] = arr[i];
    }
}

clock_t counter(SortFunc sort, ulong n, int *arr){
    clock_t start_t, end_t;
    start_t = clock();
    sort(n, arr);
    end_t = clock();
    return end_t - start_t;
}

void generate_random_arr(ulong n, int* arr){
    int i = 0;
    srand((unsigned)time(NULL));          
    for(i = 0; i < n; i++){
        arr[i] = rand();
    }                          
}

void compare_n(ulong n, clock_t *bubble_sort_t, clock_t *dc_sort_t){
    int *arr = malloc(sizeof(int) * n);
    int *bubble_sorted_arr = malloc(sizeof(int) * n);
    int *dc_sorted_arr = malloc(sizeof(int) * n);
    ulong i = 0;
    generate_random_arr(n, arr);
    deep_copy(n, arr, bubble_sorted_arr);
    *bubble_sort_t = counter(bubble_sort, n, bubble_sorted_arr);
    deep_copy(n, arr, dc_sorted_arr);
    *dc_sort_t = counter(divide_and_conquer_sort, n, dc_sorted_arr);

    /*
    printf("origin array:\n");
    for(i = 0; i < n; i++){
        printf("%d\t", arr[i]);
    }
    printf("\n");
    printf("====================================\n");
    printf("duration of bubble sort: %lu\n", *bubble_sort_t);
    for(i = 0; i < n; i++){
        printf("%d\t", bubble_sorted_arr[i]);
    }
    printf("\n");
    printf("====================================\n");
    printf("duration of divide and conquer sort: %lu\n", *dc_sort_t);
    for(i = 0; i < n; i++){
        printf("%d\t", dc_sorted_arr[i]);
    }
    printf("\n");
    printf("====================================\n");
    */
}

void compare(ulong scale){
    ulong n = 10;
    FILE *bubble,*dc;
    if((bubble=fopen("bubble.csv","w"))==NULL){
        printf("cannot open file/n");
        exit(0);
    }
    if((dc=fopen("dc.csv","w"))==NULL){
        printf("cannot open file/n");
        exit(0);
    }
    
    clock_t bubble_t, dc_t;
    for(n = 10; n <= scale; n += 10){
        ulong i;
        double sum_of_bubble = 0;
        double sum_of_dc = 0;
        for(i = 0; i < 10; i++){
            compare_n(n, &bubble_t, &dc_t);
            sum_of_bubble += bubble_t;
            sum_of_dc += dc_t;
        }
        fprintf(bubble, "%lu, %lf\n", n, sum_of_bubble/n);
        fprintf(dc, "%lu, %lf\n", n, sum_of_dc/n);
        printf("%lu/%lu\n", n, scale); 
    }
    fclose(bubble);
    fclose(dc);
}

int main(int argc, char *argv[]){
    if(argc != 2){
        printf("Wrong usage: \ncompare 1000");
        exit(-1);
    }
    char *leftover;
    ulong scale = strtoul(argv[1], &leftover, 10);
    compare(scale);
    return 0;
}
