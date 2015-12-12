#include "evaluation.h"
#include "gfx5.h"

void settings(char *);
void plot(Expression *ex);
static char *ex2str(Expression *);
float plugin(Expression *, float);
Expression excpy(Expression *);

void help(void);
void drawAxes(int minX, int maxX, int minY, int maxY);
double pix2val(int n, int axis_min, int axis_max, int window_len, int isY);
int val2pix(double n, int axis_min, int axis_max, int window_len, int isY);
