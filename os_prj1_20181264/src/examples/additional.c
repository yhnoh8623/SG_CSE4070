#include<stdio.h>
#include<syscall.h>
#include<stdlib.h>

int main (int argc, char **argv)
{
    int fibo_result = fibonacci(atoi(argv[1]));
    int max_result = max_of_four_int(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
    //printf("check = %s\n", argv[0]);
    //printf("num = %d\n", atoi(argv[1]));
    printf("%d %d", fibo_result, max_result);
    printf("\n");

    return EXIT_SUCCESS;
}