#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include "rpn.h"
#include "stack.h"

//solves a given equation
double evaluate(char *eq){

	//init data structure
	stack *s = (stack*) malloc(sizeof(stack));
	initstack(s);

	//tokenize eq
	char *tok = strtok(eq," \t\n");

	//loop and store data in stack
	while(tok != NULL){
		//if num push to stack
		if (isdigit(tok[0])){
			push(s, atof(tok));
		}

		//is operator
		else if(ispunct(tok[0])){
			//not enough operands for fxn
			if(s->size < 2){
				printf("ERROR: PROVIDE MORE OPERANDS\n");
				return NAN;
			}

			//grab stack values
			double a = pop(s);
			double b = pop(s);

			//printf("%s ",tok);
			//preform operation and push result to stack
			//printf("tok symbol: %c\n",tok[0]);
			switch(tok[0]){
				case'+':
				push (s, b+a);
				break;

				case'-':
				push (s, b-a);
				break;

				case'/':
				//check divide by 0
				if (a==0){
					printf("ERROR: CANNOT DIVIDE BY ZERO\n");	
					return NAN;				
				}
				push (s, b/a);
				break;

				case'*':
				push (s, b*a);
				break;

				default:
				//not supported operation
				printf("ERROR: OPERATOR %s NOT SUPPORTED\n",tok);
				return NAN;
			}
		}
		else{
			//value neither num or symbol
			printf("ERROR: OPERATOR %s UNRECOGNIZED\n",tok);
			return NAN;
		}
		//next tok
		tok = strtok(NULL, " \t\n");
		//printstack(s);
	}
	//last entry should be result/ otherwize need more ops
	if (s->size == 1)
		return pop(s);
	else{
		printf("ERROR: NEED MORE OPERADS\n");
		return NAN;
	}
}




















































/*

double evaluate(char *equation)
{
	stack *s = (stack*) malloc(sizeof(stack));
	initstack(s);

	//tokenize the equation into operands and operators
	char *tok = strtok(equation, " \t\n");
	while (tok != NULL)
	{
		if (strlen(tok) == 1 && ispunct(tok[0]))
		{
			if (size(s) >= 2)
			{
				//tok is an operator: pop two operands, apply operator, push back
				double b = pop(s);
				double a = pop(s);
				if (!strcmp("+", tok))
					push(s, a + b);
				else if (!strcmp("-", tok))
					push(s, a - b);
				else if (!strcmp("*", tok))
					push(s, a * b);
				else if (!strcmp("/", tok))
				{
					if (b == 0)
					{
						printf("Error: divide-by-zero detected.\n");
						return NAN;
					}
					push(s, a / b);
				}
				else
				{
					printf("Error: operator %s unrecognized.\n", tok);
					return NAN;
				}
			}
			else
			{
				printf("Error: insufficient operands.\n");
				return NAN;
			}
		}
		else
		{
			//tok is an operand: push it
			push(s, atof(tok));
		}

		//get next token from string
		tok = strtok(NULL, " \t\n");
	}
	if (size(s) == 1)
		return pop(s);
	else
	{
		printf("Error: too many operands entered.\n");
		return NAN;
	}
}
*/
