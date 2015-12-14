#include "eval.h"
#define NULL_DOUBLE -99999.
#define NULL_CHAR '#'
#define NULL_STR "\0"

char operators[] = "^*/+-%";
int verbose = 0, logBase = 10;
char settingsFile[] = "settings.txt";

/*=====Evaluation========================*/

double eval(Expression *ex, int startcol, int endcol){
    int i, n, j=-1, k; char c;
    double result;
    updateParts(ex);
    if(verbose) printAsGrid(ex, "eval", 16);
    //base case: is there only one number left?
    if(ex->parts <= 1) return ex->nums[0];

    //next: evaluate parens
    n = hasparens(ex, startcol, endcol);
    if(n > 0){
        if(verbose) printf("[eval:23]\texpression has parens\n");
        j = lastParen(ex, '(');
        k = firstParen(ex, ')');
        c = highestOp(ex, j, k);
        if(c != NULL_CHAR){         //if there's an expression inside the parens...
            if(verbose) printf("[eval:28]\tevaluating inside parens\n");
            eval(ex, j + 1, k - 1); //evaluate it
        }else{
            if(j >= 1 && isa(ex, j - 1) == 'f'){    //if the parens are preceded by a function...
                if(verbose){
                    printf("[eval:33]\tevaluating function: %s\n", ex->funcs[j - 1]);
                    printf("\t\tpress return to continue...\n");
                    getchar();
                }
                evalFunc(ex, j - 1);                //evaluate the function
                updateParts(ex);
                eval(ex, 0, ex->parts);
            }else{                                  //otherwise clear the parens
                nullifycol(ex, j);
                nullifycol(ex, k);
                shiftLeft(ex);
            }
        }
    }

    //re-evaluate base case
    if(ex->parts <= 1) return ex->nums[0];

    //next: order of operations
    c = highestOp(ex, startcol, endcol);            //find the highest order operator
    n = firstOpInRange(ex, c, startcol, endcol);    //find the first occurence of this operator
    evalOp(ex, n);                                  //evaluate it
    eval(ex, 0, ex->parts);                         //rinse and repeat
}

void evalFunc(Expression *ex, int col){
    double result = NULL_DOUBLE, arg = ex->nums[col + 2];
    char *function = ex->funcs[col];
    if(strcmp(function, "sin") == 0)
        result = sin(arg);
    else if(strcmp(function, "cos") == 0)
        result = cos(arg);
    else if(strcmp(function, "tan") == 0)
        result = tan(arg);
    else if(strcmp(function, "log") == 0)
        result = log10(arg)/log10(logBase);
    else if(strcmp(function, "ln") == 0)
        result = log(arg);
    else if(strcmp(function, "asin") == 0)
        result = asin(arg);
    else if(strcmp(function, "acos") == 0)
        result = acos(arg);
    else if(strcmp(function, "atan") == 0)
        result = atan(arg);
    else if(strcmp(function, "sqrt") == 0)
        result = sqrt(arg);
    else if(strcmp(function, "ceil") == 0)
        result = ceil(arg);
    else if(strcmp(function, "floor") == 0)
        result = floor(arg);
    else if(strcmp(function, "round") == 0)
        result = round(arg);
    else if(strcmp(function, "abs") == 0)
        result = fabs(arg);
    nullifycol(ex, col);        //clear the function col 
    nullifycol(ex, col + 1);    //clear the open paren col
    nullifycol(ex, col + 2);    //clear the number col
    nullifycol(ex, col + 3);    //clear the close paren col
    ex->nums[col] = result;     //assign result to func col
    shiftLeft(ex);              //remove null cols
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

int firstIndexOf(char c, char *str, int start){
    int i, max = strlen(str);
    for(i = start; i < max; i++){
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

int validstr(char *str){
    int len = strlen(str), openP = 0, closeP = 0, i;
    int nums = 0, chars = 0;
    if(verbose) printf("\tvalidating expression...\n");
    if(strcmp(str, "c") == 0 || strcmp(str, "q") == 0) return 0;
    for(i = 0; i < len; i++){
        if(isspace(str[i])){
            return 1;
        }
        if(isdigit(str[i]) && !isdigit(str[i + 1]) &&
                str[i + 1] != '.' && str[i + 1] != ')' && !isoperator(str[i + 1]) && str[i + 1] != '\0'){
            return 2;
        }
        if(str[i] == ')' && i != len - 1 && !isoperator(str[i + 1]) && str[i + 1] != ')'){
            return 3;
        }
        if(str[i] == '(') openP++;
        if(str[i] == ')') closeP++;
        if(isdigit(str[i]) || str[i] == '.') nums++;
        if(isalpha(str[i])) chars++;
    }
    if(openP != closeP) return 4;
    if(nums + openP + closeP == 0) return 5;
    if(verbose) printf("\tstring valid\n");
    return 0;
}

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

int nvars(Expression *ex){
    int i, j = 0;
    char foundvars[256];
    for(i = 0; i <= ex->parts; i++){
        if(isa(ex, i) == 'v' && !charinstr(foundvars, ex->vars[i], j + 1)){
            foundvars[j] = ex->vars[i];
            j++;
        }
    }
    return j;
}

int charinstr(char str[256], char target, int max){
    int i;
    for(i = 0; i < max; i++)
        if(str[i] == target) return 1;
    return 0;
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
    if(ex->vars[col] != NULL_CHAR)
        return 'v';
    return NULL_CHAR;
}
int isNullCol(Expression *target, int col){
    if(strcmp(target->funcs[col], NULL_STR) == 0 && //compare all rows to respective null val
       target->ops[col] == NULL_CHAR &&
       target->parens[col] == NULL_CHAR &&
       target->nums[col] == NULL_DOUBLE &&
       target->vars[col] == NULL_CHAR
    ) return 1;
    return 0;
}

/*===========================================*/

/*=====Expression struct Utilities===========*/

void cpycol(Expression *ex, int dest, int source){
    ex->nums[dest] = ex->nums[source];
    ex->ops[dest] = ex->ops[source];
    ex->parens[dest] = ex->parens[source];
    ex->vars[dest] = ex->vars[source];
    strcpy(ex->funcs[dest], ex->funcs[source]);
}

void nullifycol(Expression *ex, int i){
    ex->nums[i] = NULL_DOUBLE;
    ex->ops[i] = NULL_CHAR;
    ex->parens[i] = NULL_CHAR;
    ex->vars[i] = NULL_CHAR;
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
    int j = 0, i, n = firstIndexOf('.', str, 0), neg = 0; //start str iter. @ 0, find decimal
    double result = 0;
    for(i = 0; i <= strlen(str); i++) if(isspace(str[i])) str[i] = '\0';
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
    if(verbose){
        printf("\tparsing expression...\n");
        printf("\tpress return to continue...\n");
        getchar();
    }
    while(source[i] != '\0' && i < 256){       //until you reach the end of the string...
        if(verbose) printf("\texamining >%c< @ %d\n", source[i], i);
        if(isalpha(source[i]) && isalpha(source[i + 1])){//if it's a letter followed by letter-> func
            nullifycol(target, k);                      //start with a blank slate
            j = firstIndexOf('(', source, i) - i;       //number of chars in func
            strncpy(target->funcs[k], &source[i], j);   //copy func string to target
            target->funcs[k][j] = '\0';                 //add delimiting character
            target->parts = k;                          //log that you've added a col
            k++;
            i += j;                                     //we already have these logged
            continue;
        }
        if(isalpha(source[i])){     //now we're dealing with a variable
            nullifycol(target, k);
            target->vars[k] = source[i];
            target->parts = k;
            k++;
            i++;
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
        if(isoperator(source[i]) && (
                    isa(target, k - 1) == 'n' ||
                    target->parens[k - 1] == ')' ||
                    isa(target, k - 1) == 'v')){
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
    if(verbose) printAsGrid(target, "parse", 424);
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
    target->vars[0] = NULL_CHAR;
    target->vars[1] = NULL_CHAR;
}

/*============================================*/

/*=====I/O Functions==========================*/

void getInput(char *target){
    int i;
    printf(">> ");
    fgets(target, 256, stdin);
    target[strlen(target) - 1] = '\0';  //remove the newline from the input
    if(!isalnum(target[0]) && target[0] != '('){
        if(verbose) printf("[getInput:443] shifting target\n");
        target++;
    }
}

void printEx(Expression *target){
    int i;
    printf("\t");
    for(i = 0; i <= target->parts; i++){
        if(strcmp(target->funcs[i], NULL_STR) != 0){
            printf("%s", target->funcs[i]);
        }else if(target->parens[i] != NULL_CHAR){
            printf("%c", target->parens[i]);
        }else if(target->nums[i] != NULL_DOUBLE){
            printf("%lf", target->nums[i]);
        }else if(target->ops[i] != NULL_CHAR){
            printf("%c", target->ops[i]);
        }else if(target->vars[i] != NULL_CHAR){
            printf("%c", target->vars[i]);
        }
    }
    printf("\n");
}

void printAsGrid(Expression *ex, char *prog, int ln){
    int i;
    char dblstr[64];
    printf("\t==>called from [%s:%d]===================\n", prog, ln); printEx(ex);
    printf("\t   |Num:\t| Func:\t| Op:\t| Paren:| Var:\n");
    for(i = 0; i <= ex->parts; i++){
        if(ex->nums[i] == NULL_DOUBLE) strcpy(dblstr, "\t");
        else sprintf(dblstr, "%lf", ex->nums[i]);
        printf("\t%2d | %s\t| %s\t| %c\t| %c\t| %c\n", i,
                dblstr,
                ex->funcs[i],
                ex->ops[i]==NULL_CHAR?:ex->ops[i],
                ex->parens[i]==NULL_CHAR?:ex->parens[i],
                ex->vars[i]==NULL_CHAR?:ex->vars[i]
        );
    }
    printf("\t==================================================\n\n");
}

void errmsg(int n){
    switch(n){
        case 1:
            printf("\terr: expression contains space\n");
            break;
        case 2:
            printf("\terr: missing operator after digit\n");
            break;
        case 3:
            printf("\terr: missing operator after parenthasis\n");
            break;
        case 4:
            printf("\terr: unbalanced parenthases\n");
            break;
        case 5:
            printf("\terr: invalid expression\n");
            break;
    }
}

void settings(char *str){
    str += 4;   //remove "set " from the string
    char arg1[256], arg2[256];
    int space1 = firstIndexOf(' ', str, 0);
    if(verbose) printf("[settings:119]\tcommand: %s\n", str);
    if(space1 != -1){
        snprintf(arg1, space1 + 1, "%s", str);
        snprintf(arg2, strlen(str) - space1,  "%s", str + space1 + 1);
    }else strcpy(arg1, str);
    if(verbose) printf("[settings:124]\targ1:%s\targ2:%s\n", arg1, arg2);
    if(strcmp(arg1, "width") == 0){
        if(verbose) printf("\tsetting window width to %s\n", arg2);
        winWid = (int)str2dbl(arg2);
        samewindow = 0;
    }else if(strcmp(arg1, "height") == 0){
        if(verbose) printf("\tsetting window height to %s\n", arg2);
        winHgt = (int)str2dbl(arg2);
        samewindow = 0;
    }else if(strcmp(arg1, "color") == 0){
        char arg3[256], arg4[256]; int space2;
        str += 6;
        space1 = firstIndexOf(' ', str, 0);
        snprintf(arg2, space1 + 1, "%s", str);

        space2 = firstIndexOf(' ', str, space1 + 1);
        snprintf(arg3, space2 - space1, "%s", str + space1 + 1);
        
        snprintf(arg4, strlen(str) - space2, "%s", str + space2 + 1);
        if(verbose) printf("\tsetting color to %s, %s, %s\n", arg2, arg3, arg4);
        globR = (int)str2dbl(arg2); globG = (int)str2dbl(arg3); globB = (int)str2dbl(arg4);
    }else if(strcmp(arg1, "color2") == 0){
        char arg3[256], arg4[256]; int space2;
        str += 7;
        space1 = firstIndexOf(' ', str, 0);
        snprintf(arg2, space1 + 1, "%s", str);

        space2 = firstIndexOf(' ', str, space1 + 1);
        snprintf(arg3, space2 - space1, "%s", str + space1 + 1);
        
        snprintf(arg4, strlen(str) - space2, "%s", str + space2 + 1);
        globR2 = (int)str2dbl(arg2); globG2 = (int)str2dbl(arg3); globB2 = (int)str2dbl(arg4);
    }else if(strcmp(arg1, "window") == 0){
        if(windowopen){
            if((int)str2dbl(arg2) == 1) samewindow = 0;
            else if((int)str2dbl(arg2) == 0) samewindow = 1;
            else samewindow = (samewindow + 1) % 2;
            if(verbose) printf("\twill open plots in %s window\n", samewindow?"same":"new");
        }else{
            printf("\terr: must have at least one window open to disable new window\n");
        }
    } else if(strcmp(arg1, "verbose") == 0){
        if((int)str2dbl(arg2) == 1) verbose = 1;
        else if((int)str2dbl(arg2) == 0) verbose = 0;
        else verbose = (verbose + 1) % 2;
        if(verbose) printf("\tturning verbose evaluation %s\n", verbose?"on":"off");
    }else if(strcmp(arg1, "file") == 0){
        strcpy(settingsFile, arg2);
        getSettings(settingsFile);
    }else if(strcmp(arg1, "base") == 0){
        logBase = (int)str2dbl(arg2);
        if(verbose) printf("\tsetting log base to %d\n", logBase);
    }else if(strcmp(arg1, "refresh") == 0){
        if(verbose) printf("\trefreshing settings from file: %s\n", settingsFile);
        settings(settingsFile);
    }else if(strcmp(arg1, "grad") == 0){
        if((int)str2dbl(arg2) == 1) useGrad = 1;
        else if((int)str2dbl(arg2) == 0) useGrad = 0;
        else useGrad = (useGrad + 1) % 2;
        if(verbose) printf("\tturning gradient %s\n", useGrad?"on":"off");
    }else if(strcmp(arg1, "thick") == 0){
        if((int)str2dbl(arg2) == 1) thick = 1;
        else if((int)str2dbl(arg2) == 0) thick = 0;
        else thick = (thick + 1) % 2;
        if(verbose) printf("\tusing  %s line\n", thick?"thick":"thin");
    }
    /*==Experimental:==
    else if(strcmp(arg1, "xmin") == 0){
        xmin = (int)str2dbl(arg2);
    }else if(strcmp(arg1, "xmax") == 0){
        xmax= (int)str2dbl(arg2);
    }else if(strcmp(arg1, "ymin") == 0){
        ymin = (int)str2dbl(arg2);
    }else if(strcmp(arg1, "ymax") == 0){
        ymax = (int)str2dbl(arg2);
    }
    */
    else{
        printf("\t===Current settings=============\n");
        printf("\twindow width:\t|window height:\n");
        printf("\t%d\t\t|%d\n", winWid, winHgt);
        printf("\t================================\n");
        printf("\tred:\t|green:\t|blue:\n");
        printf("\t%d\t|%d\t|%d\n", globR, globG, globB);
        printf("\tred2:\t|green2:|blue2:\n");
        printf("\t%d\t|%d\t|%d\n", globR2, globG2, globB2);
        printf("\t================================\n");
        printf("\tplot gradient:\n");
        printf("\t%s\n", useGrad?"on":"off");
        printf("\tplot line:\n");
        printf("\t%s\n", thick?"thick":"thin");
        printf("\t================================\n");
        printf("\topen in new window:\n");
        printf("\t%s\n", samewindow?"off":"on");
        printf("\t================================\n");
        printf("\tverbose evaluation:\n");
        printf("\t%s\n", verbose?"on":"off");
        printf("\t================================\n");
        printf("\tlog base:\n");
        printf("\t%d\n", logBase);
        printf("\t================================\n");
    }
}

void getSettings(char *filename){
    FILE *fp; int i;
    if(fp = fopen(filename, "r")){
        char line[64];
        while(!feof(fp)){
            fgets(line, 64, fp);
            for(i = 0; i <= strlen(line); i++){
                if(line[i] == '\n') line[i] = '\0';
            }
            if(line[0] == 's') settings(line);
        }
    }else{
        if(verbose){
            printf("[getSettings:603] failed to read settings file\n");;
            printf("\t\t  using stock defaults\n");
        }
    }
}

void help(void){
    printf("\tdo calculation:         '[expression, no variables]'\n");
    printf("\tplot function:          '[expression, one variable]'\n");
    printf("\tview help menu:         'help'\n");
    printf("\tplot window width:      'set width [pixels]'\n");
    printf("\tplot window height:     'set height [pixels]'\n");
    printf("\tplot color:             'set color [red] [green] [blue]'\n");
    printf("\tplot secondary color:   'set color 2 [red] [green] [blue]\n");
    printf("\t                            must be in range [0, 255]\n");
    printf("\tuse gradient on plot:   'set grad [args]'\n");
    printf("\t                            args: 1 (use gradient), 0 (don't use gradient)), none (toggle)\n");
    printf("\ttoggle new window:      'set window [args]'\n");
    printf("\t                            args: 1 (plot in new window), 0 (plot in same window), none (toggle)\n");
    printf("\ttoggle verbose mode:    'set verbose [args]'\n");
    printf("\t                            args: 1 (on), 0 (off), none (toggle)\n");
    printf("\tplot with thick line:   'set thick [args]'\n");
    printf("\t                            args: 1 (thick line), 0 (thin line), none (toggle)\n");
    printf("\tchange base of 'log':   'set base [new base]'\n"); 
    printf("\tget settings from file: 'set file [filename]'\n");
    printf("\tdisplay settings:       'set'\n");
    printf("\trefresh settings:       'set refresh'\n");
    printf("\tclear screen:           'c'\n");
    printf("\tquit prgram:            'q'\n");
}

/*============================================*/
