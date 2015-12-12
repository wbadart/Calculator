#include "head.h"

int main(int argc, char *argv[]){
    char str[256] = "0"; int n;
    Expression usrEx, *pe = &usrEx; setupEx(pe);
    if(argc > 1){
        strcpy(str, argv[1]);
        parse(str, pe);
        if(nvars(pe) > 0){
            plot(pe);
            printf("\t...\n");
            getchar();
        }else printf("\t=%lf\n", eval(pe, 0, pe->parts));
    }else{
        getInput(str);
    }
    while(argc <= 1){
        if(str[0] == 'q') break;
        if(str[0] == 'c' && str[1] == '\0'){
            printf("\e[1;1H\e[2J");
        }else  if(strncmp("set", str, 3) == 0)
            settings(str);
        else if(strncmp("help", str, 4) == 0){
            help();
        }else{
            n = validstr(str);
            if(n > 0) errmsg(n);
            else{
                parse(str, pe);
                n = nvars(pe);
                if(n == 1){
	               plot(pe);
                }else if(n > 1){
                   printf("\terr: too many variables\n");
                }else{
                   printf("\t=%lf\n", eval(pe, 0, pe->parts));
                }
             }
        }
        getInput(str);
    }
    return 1;
}
