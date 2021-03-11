#include <stdio.h>

int main()
{
    void *p = sbrk(0);
    printf("%x\n", p);
    getchar();
}
