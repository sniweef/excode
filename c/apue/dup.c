#include <stdio.h>
#include <unistd.h>
int main(int argc, const char *argv[])
{
    printf("%d\n", dup(5));
    return 0;
}
