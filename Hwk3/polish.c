/*
 * polish.c
 *
 *  Created on: Jun 25, 2015
 *      Author: dchiu
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include "polish.h"
#include "stack.h"

double evaluate(char *equation)
{
	Stack *s = (Stack*) malloc(sizeof(Stack));
	initStack(s);

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

