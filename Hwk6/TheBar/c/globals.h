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

//TODO - declare some semaphores
sem_t* bar_custWait = sem_open(0);
sem_t* bar_orderWait = sem_open(0);
sem_t* bar_custPay = sem_open(0);






#endif /* GLOBALS_H_ */
