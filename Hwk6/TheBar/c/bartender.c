/*
 * bartender.c
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
#include "bartender.h"

/**
 * Code for bartender thread.
 * Do not touch.
 */
void* bartender(void* args)
{
	int i;
	for (i = 0; i<num_threads; i++)
	{
		waitForCustomer();
		makeDrink();
		receivePayment();
	}
	return NULL;
}

/**
 * Waits in a closet until a customer enters.
 */
void waitForCustomer()
{
	//wait for customer in bar
	sem_wait(cust_here);
	//bar is not empty
	sem_post(bar_empty);


	printf("\t\t\t\t\t\t\t\t\t\t\t| Bartender\n");
}


/**
 * When a customer places an order it takes the Bartender
 * a random amount of time between 5 ms and 1000 ms to make the drink.
 */
void makeDrink()
{
	//customer places order
	sem_post(cust_order);

	//wait time for drink
	usleep(5 + (rand() % 1000));

	//finish drink
	sem_wait(make_drink);

	printf("\t\t\t\t\t\t\t\t\t\t\t| \t\tBartender\n");
}


/**
 * Gets payment from the correct customer
 */
void receivePayment()
{
	//wait for customer at register
	sem_wait(at_register);

	//at the register waiting for customer to pay
	printf("\t\t\t\t\t\t\t\t\t\t\t| \t\t\t\tBartender\n");

	//customer has payed bartender
	sem_post(payment);

	//got the payment from the right customer!
	printf("\t\t\t\t\t\t\t\t\t\t\t| \t\t\t\t\t\tBartender\n");
}
