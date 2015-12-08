#include "head.h"

void parse(char *str){
    int n, i, len = strlen(str);
    //order of operations-> compute () first
    n = lastIndexOf(str, '(');
        switch(str[n - 1]){
            case 's':
                //cos
                break;
            case 'n':
                //sin tan or ln
                break;
            case 'g':
                //log
                break;
            case '^':
                //exp
                break;
            default:
                //arithmatic
                break;
    }
}

double eval(char *str){
    int j = 0, i = 0;
    char operand_a[100], operand_b[100], operator;
    while(isdigit(str[i]) || str[i] != '.'){
        operand_a[i] = str[i] - 48;
        i++;
    }
    operand_a[i] = '\0';
    operator = str[i];
    i++;
    while(isdigit(str[i]) || str[i] != '.'){
        operand_b[j] = str[i];
        i++; j++;
    }
    operand_b[j] = '\0';    
}

double str2dbl(char *str){
    int j = 0, i, n = firstIndexOf(str, '.', 0);
    double result = 0;
    if(n == -1){
        n = strlen(str);
        for(i = n - 1; i >= 0; i--){
            result += pow(10, i) * (str[j] - 48);
            j++;
        }
    }else{
        j = n - 1;
        for(i = 0; i < n; i++){
            result += pow(10, j) * (str[i] - 48);
            printf("ln56[%d]: %lf\n", i, result);
            j--;
        }
        i++; j = -1;
        for(i = i; i < strlen(str); i++){
            result += pow(10, j) * (str[i] - 48);
            printf("ln56[%d]: %lf\n", i, result);
            j--;
        }
    }
    return result;
}

int lastIndexOf(char *str, char target){
    int i = strlen(str);
    for(i = i - 1; i >= 0; i--)
        if(str[i] == target) return i;
    return -1;
}
int firstIndexOf(char * str, char target, int max){
    int i;
    for(i = max; i < strlen(str); i++)
        if(str[i] == target) return i;
    return -1;
}
