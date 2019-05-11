#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "stack.h"

//remove head of stack and return val
double pop(stack *s){
	//check if null otherwize pop stack
	if (s->head == NULL){
		return NAN;
	}else{
		//set head to next sNode
		sNode *tmp = s->head;
		s->head = s->head->next;		
		double r = tmp->val;
		//cleanup
		free(tmp);
		//update size
		s->size--;
		//return removed value				
		return r;
	}
}

//push a new sNode on stack
void push(stack *s, double newval){
	//allocate memory for new sNode
	sNode *new = (sNode*) malloc(sizeof(sNode));
	
	//init sNode values
	new->val = newval;
	new->next = s->head;

	//set head and size
	s->head = new;
	s->size++;
}

//print stack size
int size(stack *s){
	return s->size;
}


//print stack
void printstack(stack *s){
	printf("___________________________\n");
	
	sNode *cur = s->head;
	while (cur != NULL)
	{
		printf("%.4f\n", cur->val);
		cur = cur->next;
	}
	printf("___________________________\n");
}


//create a new stack object
void initstack(stack *s){
	s->head = NULL;
	s->size = 0;
}