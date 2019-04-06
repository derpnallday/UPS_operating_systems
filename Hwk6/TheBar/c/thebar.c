/*
 * TheBar.c
 *
 *  Created on: Dec 26, 2015
 *      Author: dchiu
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include "globals.h"
#include "customer.h"
#include "bartender.h"


void printBanner();
void init();
void cleanup();

//need to pass thread number to customer thread
typedef struct cust_args{
	int cust_num;
} thread_args;

/**
 * Main function
 */
int main(int argc, char** argv)
{
	printBanner();
	init();		//initialize semaphores


	//read in num threads from arguements
	num_threads = atoi(argv[1]);

	if (num_threads <= 1){
		printf("enter value larger than 1\n");
		return 0;
	}

	//init number of threads specified (+1 for bartender)
	pthread_t threads[num_threads+1];

	//store threads arguments
	struct cust_args args[num_threads];



	//fire off customer threads
	int i = 0;
	for (i = 0; i < num_threads; i++){
		//store thread id for thread args
		args[i].cust_num = i;

		//fire thread
		pthread_create(&threads[i], NULL, customer, &args[i]);

	}


	//fire off bartender thread
	pthread_create(&threads[num_threads], NULL, bartender, NULL);



	// wait for all threads to finish
	for (i = 0; i <= num_threads; i++){
		printf("\n%d",i);
		pthread_join(threads[i], NULL);
		printf(": thread joined");
	}

	printf("\njoined threads\n");

	cleanup();	//cleanup and destroy semaphores
	return 0;
}


/**
 * Prints the activity banner.
 * Do not touch.
 */
void printBanner() {
	printf("Customer:\t\t\t\t\t\t\t\t\t\t| Employee:\n");
	printf("Traveling\tArrived\t\tOrdering\tBrowsing\tAt Register\tLeaving");
	printf("\t| Waiting\tMixing Drinks\tAt Register\tPayment Recv\n");
	printf("----------------------------------------------------------------------------------------+");
	printf("------------------------------------------------------------\n");
}


/**
 * Create and initialize semaphores
 */
void init()
{
	//unlink semaphores
	sem_unlink("/travel");
	sem_unlink("/cust_here");
	sem_unlink("/bar_empty");
	sem_unlink("/cust_order");
	sem_unlink("/browse");
	sem_unlink("/make_drink");
	sem_unlink("/at_register");
	sem_unlink("/payment");


	//create semaphores
	travel = sem_open("/travel", O_CREAT, 0, 0) ; 			//customer travel time
	cust_here = sem_open("/cust_here", O_CREAT, 0, 0) ; 	//customer arrives
	bar_empty = sem_open("/bar_empty", O_CREAT, 0, 0) ; 	//customer check if bar is occupied
	cust_order = sem_open("/cust_order", O_CREAT, 0, 0) ; 	//customer places order
	browse = sem_open("/browse", O_CREAT, 0, 0) ;			//customer browse art gallery
	make_drink = sem_open("/make_drink", O_CREAT, 0, 0) ; 	//bartender makes drink
	at_register = sem_open("/at_register", O_CREAT, 0, 0); 	//customer at register
	payment = sem_open("/payment", O_CREAT, 0, 0) ;			//customer pays bartender
}


/**
 * Cleanup and destroy semaphores
 */
void cleanup()
{
	//close semaphores
	sem_close(travel);
	sem_close(cust_here);
	sem_close(bar_empty);
	sem_close(cust_order);
	sem_close(browse);
	sem_close(make_drink);
	sem_close(at_register);
	sem_close(payment);
}
