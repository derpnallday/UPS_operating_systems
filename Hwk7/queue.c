//queue taken and modified from previous hwk
#include "vmm.h"

//check if queue is empty
int	isempty(struct queue *q){

	if (q && q->size == 0)
		return TRUE;
	else{
		return FALSE;
	}

}

//check if queue if fully populate
int	isfull(struct queue *q){
	if (q->size >= FRAMES){
		return TRUE;
	}else{
		return FALSE;
	} 
}

//add page to queue
uint enqueue(uint p, struct queue *q){
	// allocate mem for new node
	struct qentry *newTail = (struct qentry*) malloc(sizeof(struct qentry));
	newTail->pg = p;
	newTail->next = NULL;

	//if empty
	if (isempty(q)) {
		q->head = newTail;
	}
	//if not empty
	else{
		struct qentry* oldTail = q->tail;
		newTail->prev = oldTail;
		oldTail->next = newTail;
	}

	// update queue
	q->tail = newTail;
	q->size ++;

	//return pg
	return p;
}


//remove head from queue
uint dequeue(struct queue *q){
  //cant remove if queue is empty
	if (isempty(q)) {
		return -1;
	}

	// get the head entry of the queue
	struct qentry *oldhead = q->head;
	uint p = oldhead->pg;

	//one node in queue set head and tail to null
	if (q->size == 1) {
		q->head = NULL;
		q->tail = NULL;
	} 
	//more than one node
	else {
		struct qentry *newhead = oldhead->next;
		newhead->prev = NULL;
		q->head=newhead;
	}

	//update size
	q->size --;
	//free mem
	free(oldhead);

	//return pg
	return p;
}

//remove last entry
uint getlast(struct queue *q){

	// get ttail to remove
	struct qentry *oldtail = q->tail;
	uint p = oldtail->pg;

	//if 1 node in queue
	if (q->size == 1) {
		q->head = NULL;
		q->tail = NULL;
	} 
	//more than one in queue
	else {
		struct qentry *newtail = oldtail->prev;
		newtail->next = NULL;
		q->tail=newtail;
	}

	//update size
	q->size --;

	//free mem
	free(oldtail);

	// return pg
	return p;
}


//print queue
void printq(struct queue *q){
	struct qentry *tmp;
	int i=0;

	for(tmp = q->head; tmp != NULL; tmp = tmp->next){
		printf("%d - %i\n", i, tmp->pg);
		i++;
	}

	printf("\n\n");
}
