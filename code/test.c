#include "head.h"

int main(void){
    char usrin[20];
    while(usrin[0] != 'q'){
        printf(">> ");
        fgets(usrin, 20, stdin);
        printf("%lf\n", eval(usrin));
    }
    return 1;
}
