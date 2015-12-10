#include "head.h"

int main(int argc, char *argv[]){
    char str[256] = "0";
    Expression usrEx, *pe = &usrEx; setupEx(pe);
    if(argc > 1){
        strcpy(str, argv[1]);
        parse(str, pe);
        printf(">== %lf\n", eval(pe, 0, pe->parts));
    }
    else{
        while(str[0] != 'q'){
            getInput(str);
            parse(str, pe);
            printEx(pe);
            printf("%lf\n\n", eval(pe, 0, pe->parts));
        }
    }
    return 1;
}
