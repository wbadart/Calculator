#include "head.h"

int winWid = 800, winHgt = 600;

void settings(char *str)
{
	//printf("%s\n",str);
	//start processing string at i = 4 because we already know the first 4 
	//characters are "set "
	int i; char subs[15], val[20];
	for(i = 0; i < strlen(str)-4; i++)
	{
		subs[i] = str[i+4]; 
	}
	//printf("%s\n",subs);
	if(strncmp(subs, "width", 5) == 0)
	{
		for(i = 0; i < strlen(subs)-6; i++)
		{
			val[i] = subs[i+6];
		}
		winWid = (int)str2dbl(val);
	}
	if(strncmp(subs, "height", 6) == 0)
	{
		for(i = 0; i < strlen(subs)-7; i++)
		{
			val[i] = subs[i+7];
		}
		winHgt = (int)str2dbl(val);
	}
	if(strncmp(subs, "color", 5) == 0)
	{
		for(i = 0; i < strlen(subs)-6; i++)
		{
			val[i] = subs[i+6];
		}
	}
}
void plot(Expression *ex){
    gfx_open(winWid, winHgt, ex2str(ex));
    gfx_clear();
    gfx_color(255, 255, 255);
    drawAxes(-10, 10, -10, 10);
    int i, xpxl, ypxl;float yval, xval;
    for(i = 0; i < winWid; i++){
        xpxl = i;
        xval = pix2val(xpxl, -10, 10, winWid, 0);
        yval = plugin(ex, xval);
        ypxl = val2pix(yval, -10, 10, winHgt, 1);
        gfx_point(xpxl, ypxl);
    }
    gfx_flush();
}

float plugin(Expression *ex, float val){
    Expression tmp = excpy(ex), *pt = &tmp;
    int i;
    for(i = 0; i <= ex->parts; i++){
        if(isa(pt, i) == 'v'){
            nullifycol(pt, i);
            pt->nums[i] = val;
        }
    }
    float x = eval(pt, 0, pt->parts);
    return x;
}

Expression excpy(Expression *ex){
    int i;
    Expression result;
    result.parts = ex->parts;
    for(i = 0; i <= ex->parts; i++){
        nullifycol(&result, i);
        strcpy(result.funcs[i], ex->funcs[i]);
        result.ops[i] = ex->ops[i];
        result.nums[i] = ex->nums[i];
        result.parens[i] = ex->parens[i];
        result.vars[i] = ex->vars[i];
    }
    nullifycol(&result, i);
    return result;
}

double pix2val(int n, int axis_min, int axis_max, int window_len, int isY){
    //takes a value in gfx terms and converts to axis terms
    double axis_len = axis_max - axis_min;
    double window_mid = window_len / 2;
    double prop = window_len / axis_len;
    double val;
    if(isY){
        val = (window_mid - n) / prop;
    }
    else{
        val = (n - window_mid) / prop;
    }
    return val;
}

int val2pix(double n, int axis_min, int axis_max, int window_len, int isY){
    //takes a value in axis terms and returns gfx value
    double axis_len = axis_max - axis_min;
    double window_mid = window_len / 2;
    double prop = window_len / axis_len;
    int val;
    if(isY){
        val = window_mid - (prop * n);
    }
    else{
        val = window_mid + (prop * n);
    }
    return val;
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

void drawAxes(int minX, int maxX, int minY, int maxY){
    //draws x and y axes through the middle of the window
    int numX = maxX - minX, numY = maxY - minY;
    int i, j = 0, xStep = winWid / numX, yStep = winHgt / numY;
    char numstr[4];

    gfx_color(255, 255, 255);
    //x-axis:
    gfx_line(0, winHgt / 2, winWid, winHgt / 2);
    //y-axis:
    gfx_line(winWid / 2, 0, winWid / 2, winHgt);
    //draw tick marks
    for(i = minX; i <= maxX; i++){
        sprintf(numstr, "%d", i);
        gfx_text(xStep * j, winHgt / 2 + 10, numstr);
        j++;
    }
    j = 0;
    for(i = minY; i <= maxY; i++){
        sprintf(numstr, "%d", i);
        gfx_text(winWid / 2 - 10, yStep * j, numstr);
        j++;
    }

    gfx_color(255, 255, 255);
}
