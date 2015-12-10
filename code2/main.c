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
        while(1){
            getInput(str);
            if(str[0] == 'q') break;
            parse(str, pe);
            printf("%lf\n\n", eval(pe, 0, pe->parts));
        }
    }
    return 1;
}
