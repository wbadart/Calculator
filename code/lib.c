#include "head.h"

void parse(char *usrin){
    int n, i, len = strlen(usrin);
    //order of operations-> compute () first
    n = lastIndexOf(usrin, '(');
    if(n != -1){
        switch(usrin[n - 1]){
            case 's':
                //cos
            case 'n':
                //sin tan or ln
            case 'g':
                //log
            default
        }
    }
}

int lastIndexOf(char *str, char target){
    int i = strlen(str);
    for(i = i - 1; i >= 0; i--)
        if(str[i] == target) return i;
    return -1;
}
