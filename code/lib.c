#include "head.h"

char *valids = "+-*/^.";

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

double eval(char str[200]){
    int j = 0, i = 0;
    char operand_a[100], operand_b[100], op;
    double a, b, result;
    while((isdigit(str[i]) || str[i] == '.' || str[i] == '-') && i < 200){
        operand_a[i] = str[i];
        i++;
    }
    operand_a[i] = '\0';
    a = str2dbl(operand_a);
    op = str[i];
    i++;
    while((isdigit(str[i]) || str[i] == '.' || str[i] == '-') && i < 200){
        operand_b[j] = str[i];
        i++; j++;
    }
    operand_b[j] = '\0';
    b = str2dbl(operand_b);
    switch(op){
        case '+':
            result = a + b;
            break;
        case '-':
            result = a - b;
            break;
        case '*':
            result = a * b;
            break;
        case '/':
            result = a / b;
            break;
        case '^':
            result = pow(a, b);
            break;
    }
    return result;
}

double str2dbl(char *str){
    int j = 0, i, n = firstIndexOf(str, '.', 0), neg = 0;
    double result = 0;
    if(str[0] == '-'){
        neg = 1; j = 1;
    }
    if(n == -1){
        n = strlen(str);
        for(i = n - 1 - neg; i >= 0; i--){
            result += pow(10, i) * (str[j] - 48);
            j++;
        }
    }else{
        j = n - 1 - neg;
        for(i = neg?1:0; i < n; i++){
            result += pow(10, j) * (str[i] - 48);
            j--;
        }
        i++; j = -1;
        for(i = i; i < strlen(str); i++){
            result += pow(10, j) * (str[i] - 48);
            j--;
        }
    }
    if(neg) result *= -1;
    return result;
}

int isexpr(char *str, int start, int end){
    int i = start, result = 1, decs = 0;
    while((isdigit(str[i]) || str[i] == '.') && i < end){
        if(str[i] == '.') decs++;
        if(!isdigit(str[i + 1] && !isvalidchar(str[i + 1]))) result = 0;
        i++;
    }
    i++;
    while((isdigit(str[i]) || str[i] == '.') && i < end){
        if(str[i] == '.') decs++;
        if(!isdigit(str[i + 1] && !isvalidchar(str[i + 1]))) result = 0;
        i++;
    }
    if(decs > 2) result = 0;
    return result;
}

int isvalidchar(char c){
    int i, match = 0;
    for(i = 0; i < strlen(valids); i++)
        if(c == valids[i]) match = 1;
    return match;
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
