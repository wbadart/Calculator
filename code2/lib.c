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
    n = hasparens(target, startcol, endcol);
    printf("n=%d\n", n);
    //for some reason, this check is being passed even when n = 0?
    if(n > 0){
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

void shiftLeft(Expression *ex){ //by shifting content left, this func puts null cols @ end
    int i, nullfound = 0;
    if(isNullCol(ex, ex->parts)) ex->parts--;   //lose a col if the last one's null
    for(i = ex->parts; i >= 0; i--){    //loop until you find null col or get to start
        if(isNullCol(ex, i)){
            nullfound = 1;
            break;
        }
    }
    if(nullfound){      //if you found a null col, move the col to the right into it
        if(i != ex->parts){
            cpycol(ex, i, i + 1);
            nullifycol(ex, i + 1);
        }
        shiftLeft(ex);
    }else return;   //repeat until no null cols
}

void parse(char *source, Expression *target){
    int i = 0, k = 0, j = 0;
    char tmpstr[256];
    while(source[i] != '\0'){       //until you reach the end of the string...
        if(isalpha(source[i])){                     //if it's a letter, then we have a func
            nullifycol(target, k);                      //start with a blank slate
            strncpy(target->funcs[k], &source[i], 3);   //for now, we're dealing w/ 3 letter funcs
            target->funcs[k][3] = '\0';                 //add delimiting character
            target->parts = k;                          //log that you've added a col
            k++;
            i += 3;                                     //we already have these logged
            continue;
        }
        if(source[i] == '(' || source[i] == ')'){   //if it's a paren, then the it's a paren
            nullifycol(target, k);
            target->parens[k] = source[i];              
            target->parts = k;
            k++;
            i++;                                        //here, we only need to advance 1 char
            continue;
        }
        if(isoperator(source[i]) && (isdigit(source[i - 1]) || source[i - 1] == ')')){
            nullifycol(target, k);
            target->ops[k] = source[i];                 //tracking operators
            target->parts = k;
            k++;
            i++;
            continue;
        }
        if(isdigit(source[i]) || source[i] == '-'){ //finally, digit chars mean it's a number
            j = 0;
            if(source[i] == '-') tmpstr[j++] = '-'; //if it's negative, make a note
            while(isdigit(source[i + j]) || source[i + j] == '.'){
                tmpstr[j] = source[i + j];          //add chars to tmpstr until it's non-num
                j++;
            }
            tmpstr[j] = '\0';
            nullifycol(target, k);
            target->nums[k] = str2dbl(tmpstr);
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

void setupEx(Expression *target){       //this is needed for the base case of recusive evaluation
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
    target[strlen(target) - 1] = '\0';  //remove the newline from the input
}

double str2dbl(char *str){
    int j = 0, i, n = firstIndexOf('.', str), neg = 0; //start str iter. @ 0, find decimal
    double result = 0;
    if(str[0] == '-'){  //if num is negative, j=1 to skip the dash
        neg = 1; j = 1;
    }
    if(n == -1){    //if it's an integer...
        n = strlen(str);    //start @ highest place and work to units (i.e. n*(10^0))
        for(i = n - 1 - neg; i >= 0; i--){
            result += pow(10, i) * (str[j] - 48);   //-48 b/c ascii->int
            j++;
        }
    }else{          //if it has a decimal point...
        j = n - 1 - neg;//left-most pwr of 10, eg -23.2: str[3]='.', neg=1->str[3-1-1]=2
        for(i = neg?1:0; i < n; i++){   //if neg, skip dash
            result += pow(10, j) * (str[i] - 48);
            j--;
        }
        i++; j = -1;    //j=-1 b/c now it's decimal (i.e negative powers of 10)
        for(i = i; i < strlen(str); i++){
            result += pow(10, j) * (str[i] - 48);
            j--;
        }
    }
    if(neg) result *= -1;   //up til now, result has been positive
    return result;
}

char highestOp(Expression *ex, int start, int end){
    int i, j;
    for(i = 0; i < strlen(operators); i++){ //operators is in order of op. so we start @ 0
        for(j = start; j <= end; j++){
            if(operators[i] == ex->ops[j]) return ex->ops[j];
        }
    }
    return NULL_CHAR;   //if not found, return null_char
}

int hasparens(Expression *target, int start, int end){
    int i, result = 0;
    for(i = start; i <= end; i++){
        if(target->parens[i] != NULL_CHAR) result++;    //coutn parens found in expression
    }
    return result;
}

int isNullCol(Expression *target, int col){
    if(strcmp(target->funcs[col], NULL_STR) == 0 && //compare all rows to respective null val
       target->ops[col] == NULL_CHAR &&
       target->parens[col] == NULL_CHAR &&
       target->nums[col] == NULL_DOUBLE
    ) return 1;
    return 0;
}

int firstOpInRange(Expression *target, char op, int start, int end){
    int i; char alt;
    if(op == '*' || op == '/') alt = op=='*'?'/':'*';   //set alt b/c mult/div & +/- have same order
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
