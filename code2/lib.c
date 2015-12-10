#include "head.h"
#define NULL_DOUBLE -99999.
#define NULL_CHAR '#'
#define NULL_STR "\0"

double eval(Expression *target, int startcol, int endcol){
    int i, n, j=-1, k; char c;
    double result;
    //printf("\nlib.c:12 startcol:%d, endcol:%d, parts:%d\n", startcol, endcol, target->parts);
    //printAsGrid(target, "lib.c", 11);
    updateParts(target);
    //check 1: is there only one number left?
    if((isNullCol(target, 1) && target->nums[0] != NULL_DOUBLE) || target->parts <= 1){
        return target->nums[0];
    }
    //next: evaluate parens
    n = hasparens(target, startcol, endcol);
    if(n > 0){
        j = lastParen(target, '(');
        k = firstParen(target, ')');
        c = highestOp(target, j, k);
        //printf("lib.c:22 n:%d, c:%c, j:%d, k:%d\n", n, c, j, k);
        if(c != NULL_CHAR){
            //printf("lib.c:24 n:%d, j:%d, k:%d\n", i, j, k);
            eval(target, j + 1, k - 1);
        }else{
            //printf("lib.c:27 n:%d, j:%d, k:%d\n", i, j, k);
            nullifycol(target, j);
            nullifycol(target, k);
            //printAsGrid(target, "lib.c", 30);
            shiftLeft(target);
            //printAsGrid(target, "lib.c", 32);
        }
    }
    if(target->parts <= 1) return target->nums[0];
    //next: order of operations
    j = lastParen(target, '(');
    k = firstParen(target, ')');
    c = highestOp(target, j==-1?startcol:j, k==-1?target->parts:k);
    n = firstOpInRange(target, c, j==-1?startcol:j, k==-1?target->parts:k);
    //printf("lib.c:37 j=%d, i, k=%d, n=%d, c=%c\n", j, k, n, c);
    updateParts(target);
    if(j == -1 && countOps(target, 0, target->parts) == 1) return evalOp(target, 1);
    if(n != -1 && c != NULL_CHAR){
        target->nums[n - 1] = evalOp(target, n);
        target->ops[n] = NULL_CHAR;
        target->nums[n + 1] = NULL_DOUBLE;
        shiftLeft(target);
        eval(target, startcol, target->parts);
    }else if (c == NULL_CHAR && countOps(target, 0, target->parts) <= 1){
        nullifycol(target, j);
        nullifycol(target, k);
        shiftLeft(target);
        eval(target, startcol, target->parts);
    }else{
        return evalOp(target, 1);
    }
    //next: functions
}

