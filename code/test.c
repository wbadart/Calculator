#include "head.h"

int main(void){
    char usrin[20];
    while(usrin[0] != 'q'){
        printf(">> ");
        fgets(usrin, 20, stdin);
        usrin[strlen(usrin) - 1] = '\0';
        printf("==>%s<==\n", usrin);
        printf("eval:\t%lf\n", eval(usrin));
        printf("evalF:\t%lf\n", evalF(usrin));
        printf("evalM:\t%lf\n", evalMexp(usrin));
        if(usrin[0] == 'c') printf("\e[1;1H\e[2J");
    }
    return 1;
}
