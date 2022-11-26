#include "_i8080_cpu.h"
#include "stdio.h"


void F1()
{
    printf("From F1\n");
}

void F2()
{
    printf("From F2\n");
}

void F3()
{
    printf("From F3\n");
}

void F4()
{
    printf("From F4\n");
}


void (*CPU_FUNCTIONS[4]) () = {F1, F2, F3, F4};