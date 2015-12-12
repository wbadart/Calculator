#include "head.h"

void plot(Expression *ex){
    gfx_open(400, 300, ex2str(ex));
}

static char *ex2str(Expression *ex){
    static char result[256];
    char tmp[20], c; result[0] = '\0';
    int i = 0, n, len = 0;
    while(!isNullCol(ex, i)){
        c = isa(ex, i);
        if(c == 'f'){
            n = strlen(ex->funcs[i]);
            strcat(result, ex->funcs[i]);
            len += n;
        }else if(c == 'o'){
            tmp[0] = ex->ops[i]; tmp[1] = '\0';
            strcat(result, tmp);
            len++;
        }else if(c == 'p'){
            tmp[0] = ex->parens[i]; tmp[1] = '\0';
            strcat(result, tmp);
            len++;
        }else if(c == 'v'){
            tmp[0] = ex->vars[i]; tmp[1] = '\0';
            strcat(result, tmp);
            len++;
        }else if(c == 'n'){
            n = sprintf(tmp, "%.1lf", ex->nums[i]);
            strcat(result, tmp);
            len += n;
        }
        i++;
    }
    result[len] = '\0';
    return result;
}

float plugin(Expression *ex, float val){
    Expression tmp;
    tmp.parts = ex->parts;
    int i;
    for(i = 0; i <= ex->parts; i++){
        nullifycol(&tmp, i);
        if(isa(ex, i) == 'v'){
            tmp.nums[i] = val;
        }else{
            strcpy(tmp.funcs[i], ex->funcs[i]);
            tmp.nums[i] = ex->nums[i];
            tmp.ops[i] = ex->ops[i];
            tmp.parens[i] = ex->parens[i];
        }
    }
    return eval(&tmp, 0, tmp.parts);
}
