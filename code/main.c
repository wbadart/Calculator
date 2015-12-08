#include "head.h"

int main(void){
    printf("----SCIENTIFIC CALCULATOR----\n\n");
    printf("Enter QUIT to stop\n");
    char str[20];
    double answer;
    while(1)
    {
    	printf("Enter a command: ");
    	scanf("%s", &str);
    	if(strcmp(str, "QUIT") == 0)
    		break;
    	else if(strcmp(str, "Quit") == 0)
    		break;
    	else if(strcmp(str, "quit") == 0)
    		break;
    	//else
    	//	parse(str);
       	printf("%lf\n", answer);
    }
    //test();
    return 1;
}
