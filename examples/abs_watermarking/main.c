#include <stdio.h>
#include <stdlib.h>

int absval1(int x);
int absval2(int x);

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        printf("You did not feed me arguments, I will die now :( ...");
        return 1;
    }
    int arg1 = atoi(argv[1]);
    return absval1(arg1) + absval2(arg1);
}
