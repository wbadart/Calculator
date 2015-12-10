#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

extern char operators[];

//structure which holds expression data.
//each "column" only contains 1 item, e.g. if nums[2]==3.14, ops[2]==NULL_CHAR
typedef struct Expression_s{
    double nums[256];
    char ops[256];
    char funcs[256][4];
    char parens[256];
    int parts;
} Expression;

//get keyboard input, strip newline
void getInput(char *target);
//prints an expression
void printEx(Expression *target);
//turns a string into an expression
void parse(char *source, Expression *target);
//indicates if a character is a member of "^*/+-%"
int isoperator(char);
//converts a string to a double, e.g. "-3.14" -> -3.140000
double str2dbl(char *);
//returns the first occurence of char in str, -1 if not found
int firstIndexOf(char c, char *str);
//seeds the first two columns of expression w/ NULL
void setupEx(Expression *);
//evaluates passed expression
double eval(Expression *, int startcol, int endcol);
//evaluates the part of an expression on either side of operator at column col
double evalOp(Expression *, int col);
//evaluates the part of an expression to the right of the func at col
double evalFunc(Expression *, int col);
//shifts the columns of an expression by writing over fully null columns
void shiftLeft(Expression *);
//determines if expression has any parens left
int hasparens(Expression *, int, int);
//finds the first or last open or close paren (specified by c: must be '(' or ')')
int firstParen(Expression *, char);
int lastParen(Expression *, char);
//finds the first spcified operator that occues in the given range
int firstOpInRange(Expression *, char op, int start, int end);
//determines if a specified column only has null values
int isNullCol(Expression *, int col);
//returns the highest order operator in an expression in a range of columns
char highestOp(Expression *, int start, int end);
//prints expression as grid. the pass the line called as int
void printAsGrid(Expression *, char *, int);
//copies the data from the source column to the destination within an expression
void cpycol(Expression *, int dest, int source);
//nullifies a column
void nullifycol(Expression *, int);
//updates the number of columns recoreded in "parts"
void updateParts(Expression *);
//count remaining operators
int countOps(Expression *, int start, int end);
//counts the functions present in the given range
int countFuncs(Expression *, int start, int end);
