/*
 * stack.h
 *
 *  Created on: Jun 25, 2015
 *      Author: dchiu
 */

#ifndef STACK_H_
#define STACK_H_

//structs
typedef struct StackNode
{
	double val;
	struct StackNode* next;	//node below me
} StackNode;


typedef struct Stack
{
	StackNode* top;	//top of the stack
	int size;
} Stack;


//prototypes
void initStack(Stack *s);
int isEmpty(Stack *s);
double peek(Stack *s);
double pop(Stack *s);
void push(Stack *s, double newval);
void printStack(Stack *s);
int size(Stack *s);

#endif /* STACK_H_ */
