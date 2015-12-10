#include "head.h"
#define NULL_DOUBLE -9999999.
#define NULL_CHAR '#'
#define NULL_STR "\0"

char operators[] = "^*/+-%";

double eval(Expression *target, int startcol, int endcol){
    int i, n, j, k;
    double result;
    //check 1: is there only one number left?
    if(isNullCol(target, 1) && target->nums[0] != NULL_DOUBLE) return target->nums[0];
    //next: evaluate parens
    printf("this expression has %d parens\n", hasparens(target, startcol, endcol));
    if(hasparens(target, startcol, endcol) > 0){
        j = lastParen(target, '(');
        k = firstParen(target, ')');
        if(highestOp(target, j, k) != NULL_CHAR) eval(target, j + 1, k - 1);
        printf("j: %d, k: %d\n", j, k);
        printf("before column nulls\n");
        printAsGrid(target);
        getchar();
        nullifycol(target, j);
        nullifycol(target, k);
        printf("before shiftLeft()\n");
        printAsGrid(target);
        getchar();
        shiftLeft(target);
        printf("after shiftLeft()\n");
        printAsGrid(target);
        getchar();
    }
    //next: order of operations
    if((n = firstOpInRange(target, highestOp(target, startcol, endcol), startcol, endcol)) != -1){
        target->nums[n - 1] = evalOp(target, n);
        target->ops[n] = NULL_CHAR;
        target->nums[n + 1] = NULL_DOUBLE;
        shiftLeft(target);
        eval(target, startcol, target->parts);
    }
    //next: functions
}

double evalOp(Expression *target, int col){
    switch(target->ops[col]){
        case '^':
            return pow(target->nums[col - 1], target->nums[col + 1]);
            break;
        case '*':
            return target->nums[col - 1] * target->nums[col + 1];
            break;
        case '/':
            return target->nums[col - 1] / target->nums[col + 1];
            break;
        case '+':
            return target->nums[col - 1] + target->nums[col + 1];
            break;
        case '-':
            return target->nums[col - 1] - target->nums[col + 1];
            break;
        case '%':
            return (int)target->nums[col - 1] % (int)target->nums[col + 1];
            break;
        default:
            return NULL_DOUBLE;
            break;
    }
}

void shiftLeft(Expression *ex){
    int i, nullfound = 0;
    if(isNullCol(ex, ex->parts)) ex->parts--;
    for(i = ex->parts; i >= 0; i--){
        if(isNullCol(ex, i)){
            nullfound = 1;
            break;
        }
    }
    if(nullfound){
        if(i != ex->parts){
            cpycol(ex, i, i + 1);
            nullifycol(ex, i + 1);
        }
        shiftLeft(ex);
    }else return;
}

void parse(char *source, Expression *target){
    int i = 0, k = 0, j = 0;
    char tmpstr[256];
    while(source[i] != '\0'){
        if(isalpha(source[i])){
            strncpy(target->funcs[k], &source[i], 3);
            target->funcs[k][3] = '\0';
            target->nums[k] = NULL_DOUBLE;
            target->ops[k] = NULL_CHAR;
            target->parens[k] = NULL_CHAR;
            target->parts = k;
            k++;
            i += 3;
            continue;
        }
        if(source[i] == '(' || source[i] == ')'){
            target->parens[k] = source[i];
            strcpy(target->funcs[k], NULL_STR);
            target->nums[k] = NULL_DOUBLE;
            target->ops[k] = NULL_CHAR;
            target->parts = k;
            k++;
            i++;
            continue;
        }
        if(isoperator(source[i]) && (isdigit(source[i - 1]) || source[i - 1] == ')')){
            target->ops[k] = source[i];
            strcpy(target->funcs[k], NULL_STR);
            target->nums[k] = NULL_DOUBLE;
            target->parens[k] = NULL_CHAR;
            target->parts = k;
            k++;
            i++;
            continue;
        }
        if(isdigit(source[i]) || source[i] == '-'){
            j = 0;
            if(source[i] == '-') tmpstr[j++] = '-';
            while(isdigit(source[i + j]) || source[i + j] == '.'){
                tmpstr[j] = source[i + j];
                j++;
            }
            tmpstr[j] = '\0';
            target->nums[k] = str2dbl(tmpstr);
            strcpy(target->funcs[k], NULL_STR);
            target->ops[k] = NULL_CHAR;
            target->parens[k] = NULL_CHAR;
            target->parts = k;
            k++;
            i += j;
            continue;
        }
    }
}

void cpycol(Expression *ex, int dest, int source){
    ex->nums[dest] = ex->nums[source];
    ex->ops[dest] = ex->ops[source];
    ex->parens[dest] = ex->parens[source];
    strcpy(ex->funcs[dest], ex->funcs[source]);
}

void nullifycol(Expression *ex, int i){
    ex->nums[i] = NULL_DOUBLE;
    ex->ops[i] = NULL_CHAR;
    ex->parens[i] = NULL_CHAR;;
    strcpy(ex->funcs[i], NULL_STR);
}

void printEx(Expression *target){
    int i;
    for(i = 0; i <= target->parts; i++){
        if(strcmp(target->funcs[i], NULL_STR) != 0){
            printf("%s", target->funcs[i]);
        }else if(target->parens[i] != NULL_CHAR){
            printf("%c", target->parens[i]);
        }else if(target->nums[i] != NULL_DOUBLE){
            printf("%lf", target->nums[i]);
        }else if(target->ops[i] != NULL_CHAR){
            printf("%c", target->ops[i]);
        }
    }
    printf("\n");
}

void printAsGrid(Expression *ex){
    int i;
    printf("=======expression=========\n");
    printf("-> ");
    printEx(ex);
    printf("\nNum:\t\tFunc:\tOp:\tParen:\n");
    for(i = 0; i <= ex->parts; i++){
        printf("%lf\t%s\t%c\t%c\n", ex->nums[i], ex->funcs[i], ex->ops[i], ex->parens[i]);
    }
}

void setupEx(Expression *target){
    strcpy(target->funcs[0], NULL_STR);
    strcpy(target->funcs[1], NULL_STR);
    target->ops[0] = NULL_CHAR;
    target->ops[1] = NULL_CHAR;
    target->parens[0] = NULL_CHAR;
    target->parens[1] = NULL_CHAR;
    target->nums[0] = NULL_DOUBLE;
    target->nums[1] = NULL_DOUBLE;
}

void getInput(char *target){
    printf(">> ");
    fgets(target, 256, stdin);
    //remove newline from input string:
    target[strlen(target) - 1] = '\0';
}

double str2dbl(char *str){
    int j = 0, i, n = firstIndexOf('.', str), neg = 0;
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

char highestOp(Expression *ex, int start, int end){
    int i, j;
    for(i = 0; i < strlen(operators); i++){
        for(j = start; j <= end; j++){
            if(operators[i] == ex->ops[j]) return ex->ops[j];
        }
    }
    return NULL_CHAR;
}

int hasparens(Expression *target, int start, int end){
    int i, result = 0;
    for(i = start; i <= end; i++){
        if(target->parens[i] != NULL_CHAR) result++;
    }
    return result;
}

int isNullCol(Expression *target, int col){
    if(strcmp(target->funcs[col], NULL_STR) == 0 &&
       target->ops[col] == NULL_CHAR &&
       target->parens[col] == NULL_CHAR &&
       target->nums[col] == NULL_DOUBLE
    ) return 1;
    return 0;
}

int firstOpInRange(Expression *target, char op, int start, int end){
    int i; char alt;
    if(op == '*' || op == '/') alt = op=='*'?'/':'*';
    else if(op == '+' || op == '-') alt = op=='+'?'-':'+';
    for(i = start; i <= end; i++){
        if(target->ops[i] == op || target->ops[i] == alt) return i;
    }
    return -1;
}

int firstIndexOf(char c, char *str){
    int i, max = strlen(str);
    for(i = 0; i < max; i++){
        if(str[i] == c) return i;
    }
    return -1;
}

int firstParen(Expression *target, char c){
    int i;
    for(i = 0; i <= target->parts; i++){
        if(target->parens[i] == c) return i;
    }
    return -1;
}

int lastParen(Expression *target, char c){
    int i = target->parts;
    for(i = i; i >= 0; i--){
        if(target->parens[i] == c) return i;
    }
    return -1;
}

int isoperator(char c){
    int i = strlen(operators);
    for(i = i - 1; i >= 0; i--){
        if(c == operators[i]) return 1;
    }
    return 0;
}
