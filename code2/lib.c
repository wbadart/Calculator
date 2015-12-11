#include "head.h"
#define NULL_DOUBLE -99999.
#define NULL_CHAR '#'
#define NULL_STR "\0"

double eval(Expression *ex, int startcol, int endcol){
    int i, n, j=-1, k; char c;
    double result;
    updateParts(ex);
    //base case: is there only one number left?
    if(ex->parts <= 1) return ex->nums[0];

    //next: evaluate parens
    n = hasparens(ex, startcol, endcol);
    if(n > 0){
        j = lastParen(ex, '(');
        k = firstParen(ex, ')');
        c = highestOp(ex, j, k);
        if(c != NULL_CHAR){         //if there's an expression inside the parens...
            eval(ex, j + 1, k - 1); //evaluate it
        }else{
            if(isa(ex, j - 1) == 'f'){  //if the parens are preceded by a function...
                evalFunc(ex, j - 1);    //evaluate the function
            }else{                      //otherwise clear the parens
                nullifycol(ex, j);
                nullifycol(ex, k);
                shiftLeft(ex);
            }
        }
    }

    //re-evaluate base case
    if(ex->parts <= 1) return ex->nums[0];

    //next: order of operations
    c = highestOp(ex, startcol, ex->parts);         //find the highest order operator
    n = firstOpInRange(ex, c, startcol, ex->parts); //find the first occurence of this operator
    evalOp(ex, n);                                  //evaluate it
    eval(ex, 0, ex->parts);                         //rinse and repeat
}

