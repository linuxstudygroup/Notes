// hello.c

#include<stdio.h>
#include "head_1.h"

#define GREET "Hello, World!\n"
#define NUM (1 + 2)

// entry point of main method
int main(){
    head_1 = 123;
    printf(GREET);
    printf("%d\n", NUM);
    printf("2 * 3 == %d\n", 2 * NUM);
    printf("%d\n", head_1);
    return 0;
}
