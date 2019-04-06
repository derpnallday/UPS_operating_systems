/*
 * customer.c
 *
 *  Created on: Dec 26, 2015
 *      Author: dchiu
 */


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include "globals.h"
#include "customer.h"

/**
 * This is what the thread will call.
 * Do not touch.
 */
void* customer(void* args)
{
	unsigned int *custID = (unsigned int*) args;
	custTravelToBar(*custID);
	custArriveAtBar(*custID);
	custPlaceOrder();
	custBrowseArt();
	custAtRegister();
	custLeaveBar();
	return NULL;
}


/**
 * Each customer takes a random amount of time between 20 ms and 5000 ms to travel to the bar.
 */
void custTravelToBar(unsigned int custID)
{
	//sleep time
	usleep(20 + (rand() % 5000));


	printf("Cust %u\t\t\t\t\t\t\t\t\t\t\t|\n", custID);
}


/**
 * If there is already another customer in the bar the current customer has
 * to wait until bar is empty before entering.
 */
void custArriveAtBar(unsigned int custID)
{
	//customer arrives outside bar
	sem_post(cust_here);

	//wait for bar to be empty
	sem_wait(bar_empty);

	//update now serving to current cust
	now_serving = custID;

	printf("\t\tCust %u\t\t\t\t\t\t\t\t\t|\n", custID);
}


/**
 * The customer in the bar places an order
 */
void custPlaceOrder()
{
	//place order
	sem_wait(cust_order);

	printf("\t\t\t\tCust %u\t\t\t\t\t\t\t|\n", now_serving);
}


/**
 * The customer in the bar can browse the wall art for a random amount of time between 3ms and 4000ms.
 */
void custBrowseArt()
{
	//time to browse
	usleep(3 + (rand() % 4000));

	//gets drink after browsing
	sem_post(make_drink);

	printf("\t\t\t\t\t\tCust %u\t\t\t\t\t|\n", now_serving);
}


/**
 * If their drink is not ready by the time they are done admiring the art they must wait
 * until the bartender has finished. When the bartender is finished, the customer pays.
 *
 */
void custAtRegister()
{
		//goes to register
		sem_post(at_register);

	printf("\t\t\t\t\t\t\t\tCust %u\t\t\t|\n", now_serving);
}


/**
 * The customer in the bar leaves the bar.
 */
void custLeaveBar()
{
	//wait to pay before leaving
	sem_wait(payment);

	printf("\t\t\t\t\t\t\t\t\t\tCust %u\t|\n", now_serving);
}
