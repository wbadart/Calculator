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

void parse(char *);
int lastIndexOf(char *, char);
int firstIndexOf(char *, char, int);
double eval(char str[200]);
double str2dbl(char *);
int isexpr(char *, int, int);
int isvalidchar(char);
