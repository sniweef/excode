#include <stdio.h>
#include <string.h>
int main(int argc, const char *argv[])
{
    int i;
    for (i = 0; i < 50; i++) 
        //printf("%d:%s\n", i, strerror(i));
        perror(strerror(i));
    return 0;
}
