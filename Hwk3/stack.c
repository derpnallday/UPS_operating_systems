/*
 * stack.c
 *
 *  Created on: Jun 25, 2015
 *      Author: dchiu
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "stack.h"

void initStack(Stack *s)
{
	s->top = NULL;
	s->size = 0;
}

int isEmpty(Stack *s)
{
	return s->size == 0;
}

double peek(Stack *s)
{
	return s->top->val;
}

double pop(Stack *s)
{
	if (s->top != NULL)
	{
		double retval = s->top->val;
		StackNode *tmp = s->top;	//don't lose the pointer
		s->top = s->top->next;		//set new top
		s->size--;
		free(tmp);					//free up the node
		return retval;
	}
	return NAN;
}

void push(Stack *s, double newval)
{
	//create new node
	StackNode *newNode = (StackNode*) malloc(sizeof(StackNode));
	newNode->val = newval;
	newNode->next = s->top;

	//becomes new top
	s->top = newNode;
	s->size++;
}

int size(Stack *s)
{
	return s->size;
}


void printStack(Stack *s)
{
	printf("--BEGIN STACK--\n");
	StackNode *node = s->top;
	while (node != NULL)
	{
		printf("%.2f\n", node->val);
		node = node->next;
	}
	printf("--END STACK--\n");
}
