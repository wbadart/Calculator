#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

typedef struct Expression_s{
    double a;
    double b;
    char op;
} Expression;

extern char *valids;
extern char *ops;

void parse(char *);
int lastIndexOf(char *, char, int);
int firstIndexOf(char *, char, int);
double eval(char str[200]);
double evalChunk(char str[200], int start, int end);
double str2dbl(char *);
double evalMexp(char str[200]);
int isexpr(char *, int, int);
int iscompexpr(char *, int, int);
int isvalidchar(char);
int countops(char *, int);
int countchar(char *, char);
int isnumchar(char);
int leftNumIndex(char *, int);
int rightNumIndex(char *, int);
double evalF(char *);
