/*
 * globals.h
 *
 *  Created on: Dec 26, 2015
 *      Author: dchiu
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

int num_threads;	// number of customer threads
int now_serving;	// customer's ID who is being served

// declare some semaphores
sem_t* cust_here; 	//customer arrives
sem_t* bar_empty; 	//customer check if bar is occupied
sem_t* cust_order; 	//customer places order
sem_t* make_drink; 	//bartender makes drink
sem_t* at_register; //customer at register
sem_t* payment;		 //customer pays bartender

#endif /* GLOBALS_H_ */
