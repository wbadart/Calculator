#include "head.h"
#define NULL_DOUBLE -99999.
#define NULL_CHAR '#'
#define NULL_STR "\0"

char operators[] = "^*/+-%";

/*=====Evaluation========================*/

void evalFunc(Expression *ex, int col){
    double result = NULL_DOUBLE;
    if(strcmp(ex->funcs[col], "sin") == 0)
        result = sin(ex->nums[col + 2]);
    else if(strcmp(ex->funcs[col], "cos") == 0)
        result = cos(ex->nums[col + 2]);
    else if(strcmp(ex->funcs[col], "tan") == 0)
        result = tan(ex->nums[col + 2]);
    else if(strcmp(ex->funcs[col], "log") == 0)
        result = log10(ex->nums[col + 2]);
    nullifycol(ex, col - 1);
    nullifycol(ex, col);
    nullifycol(ex, col + 1);
    nullifycol(ex, col + 2);
    ex->nums[col] = result;
    shiftLeft(ex);
}

void evalOp(Expression *target, int col){
    double result = NULL_DOUBLE;
    switch(target->ops[col]){
        case '^':
            result = pow(target->nums[col - 1], target->nums[col + 1]);
            break;
        case '*':
            result = target->nums[col - 1] * target->nums[col + 1];
            break;
        case '/':
            result = target->nums[col - 1] / target->nums[col + 1];
            break;
        case '+':
            result = target->nums[col - 1] + target->nums[col + 1];
            break;
        case '-':
            result = target->nums[col - 1] - target->nums[col + 1];
            break;
        case '%':
            result = (int)target->nums[col - 1] % (int)target->nums[col + 1];
            break;
        default:
            result = NULL_DOUBLE;
            break;
    }
    nullifycol(target, col - 1);
    nullifycol(target, col);
    nullifycol(target, col + 1);
    target->nums[col - 1] = result;
    shiftLeft(target);
}

/*========================================*/

/*=====Indexing and Location==============*/

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

int countOps(Expression *target, int start, int end){
    int i, result = 0;
    for(i = start; i <= end; i++){
        if(target->ops[i] != NULL_CHAR) result++;
    }
    return result;
}

int countFuncs(Expression *ex, int start, int end){
    int i, result = 0;
    for(i = start; i <= end; i++){
        if(strcmp(ex->funcs[i], NULL_STR) != 0)
            result++;
    }
    return result;
}

char highestOp(Expression *ex, int start, int end){
    int i, j;
    //printf("lib.c:83 searching (%d, %d)\n", start, end);
    for(i = 0; i < strlen(operators); i++){ //operators is in order of op. so we start @ 0
        for(j = start; j <= end; j++){
            if(operators[i] == ex->ops[j]) return ex->ops[j];
        }
    }
    return NULL_CHAR;   //if not found, return null_char
}

/*==========================================*/

/*=====Boolean functions====================*/

int isoperator(char c){
    int i = strlen(operators);
    for(i = i - 1; i >= 0; i--){
        if(c == operators[i]) return 1;
    }
    return 0;
}

int hasparens(Expression *target, int start, int end){
    int i, result = 0;
    for(i = start; i <= end; i++){
        if(target->parens[i] != NULL_CHAR){
            result++;    //count parens found in expression
        }
    }
    return result;
}

char isa(Expression *ex, int col){
    if(strcmp(ex->funcs[col], NULL_STR) != 0)
        return 'f';
    if(ex->ops[col] != NULL_CHAR)
        return 'o';
    if(ex->parens[col] != NULL_CHAR)
        return 'p';
    if(ex->nums[col] != NULL_DOUBLE)
        return 'n';
    return NULL_CHAR;
}
int isNullCol(Expression *target, int col){
    if(strcmp(target->funcs[col], NULL_STR) == 0 && //compare all rows to respective null val
       target->ops[col] == NULL_CHAR &&
       target->parens[col] == NULL_CHAR &&
       target->nums[col] == NULL_DOUBLE
    ) return 1;
    return 0;
}

/*===========================================*/

/*=====Expression struct Utilities===========*/

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

void updateParts(Expression *ex){
    int i = 0;
    while(!isNullCol(ex, i)) i++;
    ex->parts = i - 1;
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

/*==============================================*/

/*=====String parsing===========================*/

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
    nullifycol(target, k);
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

/*============================================*/

/*=====I/O Functions==========================*/

void getInput(char *target){
    printf(">> ");
    fgets(target, 256, stdin);
    target[strlen(target) - 1] = '\0';  //remove the newline from the input
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

void printAsGrid(Expression *ex, char *prog, int ln){
    int i;
    printf("==>called from [%s:%d]=========================\n==>", prog, ln); printEx(ex);
    printf("   |Num:\t| Func:\t| Op:\t| Paren:\n");
    for(i = 0; i <= ex->parts; i++){
        printf("%2d | %lf\t| %s\t| %c\t| %c\n", i,
                ex->nums[i]==NULL_DOUBLE?.1:ex->nums[i],
                ex->funcs[i],
                ex->ops[i]==NULL_CHAR?:ex->ops[i],
                ex->parens[i]==NULL_CHAR?:ex->parens[i]
        );
    }
    printf("==================================================\n\n");
}

/*============================================*/
