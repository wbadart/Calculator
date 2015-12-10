#include "head.h"
#define NULL_DOUBLE -99999.
#define NULL_CHAR '#'
#define NULL_STR "\0"

double eval(Expression *ex, int startcol, int endcol){
    int i, n, j=-1, k; char c;
    double result;
    //printf("\nlib.c:12 startcol:%d, endcol:%d, parts:%d\n", startcol, endcol, ex->parts);
    //printAsGrid(ex, "lib.c", 11);
    updateParts(ex);
    //check 1: is there only one number left?
    if((isNullCol(ex, 1) && ex->nums[0] != NULL_DOUBLE) || ex->parts <= 1){
        return ex->nums[0];
    }
    //next: evaluate parens
    n = hasparens(ex, startcol, endcol);
    if(n > 0){
        j = lastParen(ex, '(');
        k = firstParen(ex, ')');
        c = highestOp(ex, j, k);
        //printf("lib.c:22 n:%d, c:%c, j:%d, k:%d\n", n, c, j, k);
        if(c != NULL_CHAR){
            //printf("lib.c:24 n:%d, j:%d, k:%d\n", i, j, k);
            eval(ex, j + 1, k - 1);
        }else{
            //printf("lib.c:27 n:%d, j:%d, k:%d\n", i, j, k);
            nullifycol(ex, j);
            nullifycol(ex, k);
            //printAsGrid(ex, "lib.c", 30);
            shiftLeft(ex);
            //printAsGrid(ex, "lib.c", 32);
        }
    }
    if(ex->parts <= 1) return ex->nums[0];
    //next: order of operations
    j = lastParen(ex, '(');
    k = firstParen(ex, ')');
    c = highestOp(ex, j==-1?startcol:j, k==-1?ex->parts:k);
    n = firstOpInRange(ex, c, j==-1?startcol:j, k==-1?ex->parts:k);
    //printf("lib.c:37 j=%d, i, k=%d, n=%d, c=%c\n", j, k, n, c);
    updateParts(ex);
    if(j == -1 && countOps(ex, 0, ex->parts) == 1) return evalOp(ex, 1);
    if(n != -1 && c != NULL_CHAR){
        ex->nums[n - 1] = evalOp(ex, n);
        ex->ops[n] = NULL_CHAR;
        ex->nums[n + 1] = NULL_DOUBLE;
        shiftLeft(ex);
        eval(ex, startcol, ex->parts);
    }else if (c == NULL_CHAR && countOps(ex, 0, ex->parts) <= 1){
        nullifycol(ex, j);
        nullifycol(ex, k);
        shiftLeft(ex);
        eval(ex, startcol, ex->parts);
    }else{
        return evalOp(ex, 1);
    }
    //next: functions
    updateParts(ex);
    if(countFuncs(ex, 0, ex->parts)){
        j = lastParen(ex, '(');
        evalFunc(ex, j - 1);
        nullifycol(ex, j);
        nullifycol(ex, j + 1);
        shiftLeft(ex);
    }
}

