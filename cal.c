#include <stdio.h>
#include <unistd.h>

int main()
{
    char str[] = "\033[2J\033[%d;%dH@\033[%d;%dH*";
    int charcout = sizeof str - 1;
    printf("String:\t\\033[2J\\033[%%d;%%dH@\\033[%%d;%%dH* \n");
    printf("Length:\t%i\n", charcout);
    return 0;
}