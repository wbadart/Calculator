#include "head.h"

int main(int argc, char *argv[]){
    char str[256] = "0"; int n;
    Expression usrEx, *pe = &usrEx; setupEx(pe);
    if(argc > 1){
        strcpy(str, argv[1]);
        parse(str, pe);
        printf("\t=%lf\n", eval(pe, 0, pe->parts));
    } else{
        while(1){
            getInput(str);
            if(str[0] == 'q') break;
            if(str[0] == 'c'){
                printf("\e[1;1H\e[2J");
                continue;
            }
            if(strncmp("set", str, 3) == 0)
            	settings(str);
            else{
            	parse(str, pe);
                n = nvars(pe);
            	if(n == 1){
	            	plot(pe);
    	        }else if(n > 1){
        	        printf("err: too many variables\n");
            	}else{
                	printf("\t=%lf\n\n", eval(pe, 0, pe->parts));
            	}
            }
        }
    }
    return 1;
}
