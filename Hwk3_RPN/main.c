#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include "stack.h"
#include "rpn.h"

//global max for input
#define EQ_LEN 128
#define RSP_LEN 16

int main()
{
	//init values
	char eqn[EQ_LEN];
	char rsp[RSP_LEN];
	double sol;

	//user response loop
	do {
		printf("Enter an equation (in RPN):\n");
		//parse user input
		fgets(eqn, EQ_LEN, stdin);	
		eqn[strlen(eqn)-1] = '\0';
		
		//printf("%s\n",eqn);

		//solve equation
		sol = evaluate(eqn);

		//check sol validity
		if (!isnan(sol))
			printf("%f\n",sol);

		//continue if user enters yes
		printf("Evaluate another? (y/n): ");
		fgets(rsp, RSP_LEN, stdin);

	} while (rsp[0] == 'y');
	
	printf("Exiting...\n");
	return 0;
}
