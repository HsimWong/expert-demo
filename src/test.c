#include <stdio.h>
#include <stdlib.h>
char * func_ret() {
    char *hlw = (char *)malloc(16 * sizeof(char));
    hlw[0] = 'H';
    hlw[1] = 'w';
    hlw[2] = 'w';
    hlw[3] = 'w';
    hlw[1] = '\0';


    return hlw;
}

int main(int argc, char const *argv[]) {
    printf("%s", func_ret());
    return 0;
}
