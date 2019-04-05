/*
 ============================================================================
 Name        : PolishSol.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <math.h>
#include <string.h>
#include "stack.h"
#include "polish.h"

#define MAX_EQ_LEN 100
#define RESP_LEN 3

int main()
{
	char eqn[MAX_EQ_LEN];
	char resp[RESP_LEN];
	double ans;
	do {
		printf("Enter an equation (in RPN):\n");
		fgets(eqn, MAX_EQ_LEN, stdin);	//reads and includes trailing '\n' in string
		ans = evaluate(eqn);
		if (!isnan(ans)) {
			printf("%.3f\n", ans);
		}

		printf("Evaluate another? (y/n): ");
		fgets(resp, RESP_LEN, stdin);
		resp[1] = '\0';	//truncate so that we only keep the first character entered
	} while (!strcmp(resp, "y"));

	printf("Exiting...\n");
	return 0;
}
