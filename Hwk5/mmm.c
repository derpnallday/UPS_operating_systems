
// CPP Program to multiply two matrix using pthreads 
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rtclock.h"
#include <math.h>
#include <ctype.h>

// maximum size of matrix 
#define SIZE 10000

int matA[SIZE][SIZE]; 
int matB[SIZE][SIZE]; 
int matC[SIZE][SIZE]; 
int matD[SIZE][SIZE]; 

/*
* data to be passed into thread
* @i row in matrix A
* @j column in matrix B
* @work amount of work for thread
* @size of matrix
*/
struct data
{
	size_t i;
	size_t j;
	size_t work;
	size_t size;
};


/*
 * rtclock.c
 *
 *  Created on: Jan 21, 2016
 *      Author: dchiu
 */
double rtclock() {
	struct timezone Tzp;
	struct timeval Tp;
	int stat;
	stat = gettimeofday (&Tp, &Tzp);
	if (stat != 0)
		printf("Error return from gettimeofday: %d",stat);
	return(Tp.tv_sec + Tp.tv_usec*1.0e-6);
}




/**
* multiplies matrix A and B without threads and stores in matrix C
* @size the size of the matrix
*/
void multi(int size){
	//row in a
	for (int i=0; i<size; i++){
		int sum;
		//column  in b
		for(int j=0; j<size; j++){
			sum=0;
			//multiply row a and column b
			for(int k=0; k<size; k++){
				sum += matA[i][k] * matB[k][j];
			}
			//store in output
			matC[i][j]=sum;
		}
	}
}



/**
* matrix multiplication function for threads
* @arg rows, columns, and job number for thread
*/
static void * multiplication(void *arg){
	struct data **args = (struct data **)arg;
	size_t n,m,w,l;
	
	w =	args[0][0].work;
	l =	args[0][0].size;


	for(n=0; n<w; n++)
	{
		for(m=0; m < l; m++)
		{
    	//grab row and column
			size_t i=(args[n][m]).i;
			size_t j=(args[n][m]).j;
			double sum=0;
			size_t d;

			//preform opperation
			for (d = 0; d < l; d++)
			{
				sum = sum + matA[i][d]* matB[d][j];
			}

			matD[i][j] = sum;
			sum = 0;
		}

	}
	return 0;
}

/*
* matrix multiplication with threads
* @numThreads number of threads
* @l length of matrix
*/
void tmulti(int numThreads, int l) 
{ 

	size_t threadCount = 0;
	//dont need more threads than matrix size
	if (numThreads > l){
		threadCount = l;
	}else{
		threadCount = numThreads;
	}

	pthread_t threads[threadCount];
	size_t i, j, k, w, curi, curj;

    //store row and column
	struct data ***args;
	args = (struct data ***)malloc(l * sizeof(struct data**));

	//calculate avg work per thread
	int avgWork = round((float)l/threadCount);
	int avgThreads = threadCount-1;
	//work for last thread
	int lastWork = l-(avgWork*avgThreads) ;
	if (lastWork==0) lastWork++;

	curi=0;
	curj=0;

	//format jobs for worker threads
	for (k=0; k<threadCount; k++)
	{
		args[k] = (struct data **)malloc(l * sizeof(struct data*));
		//work for this thread
		w = avgWork;
		if(k == threadCount-1){
			w = lastWork;
		}

		//row in matrix A
		for (i=0; i < w; i++)
		{
			args[k][i] = (struct data *)malloc(l * sizeof(struct data));
			curj=0;

			//column in matrix B
			for(j = 0; j < l; j++)
			{
				//set row and column
				args[k][i][j].i = curi;
				args[k][i][j].j = curj;
				args[k][i][j].work = w;
				args[k][i][j].size = l;

				//printf("thread=%ld  row=%ld column=%ld\n",k,i,j);

				curj++;
			}
			curi++;
		}

	}

	//feed threads jobs
	for(k=0; k<threadCount; k++){
		pthread_create(&threads[k], NULL, multiplication, args[k]);
	}

 	//join
	for(k = 0; k < threadCount; k++)
	{
		pthread_join(threads[k], NULL);
	}

    //clear mem
	for (k = 0; k < threadCount; k++)
	{

		free(args[k]);
	}
	free(args);
} 

/*
* processes arguements passed thorugh shell
* @argv arguements
* @size number of arguements
*/
void processArgs(char const *argv[],int size){
	char matSize[100];

	if (size==1) printf ("Usage: ./mmm <mode> [num threads] <size>\n");
	
	//single mode
	if (size>=2 && !strcmp(argv[1],"S")){

		if (size==2){
			printf ("Usage: ./mmm S <size>\n");
			return;
		}
		if (size > 3) {
			printf ("Error: too many Arguements for mode S");
			return;
		}

		strcpy(matSize, argv[2]);
		int isNum = 0;

		int i=0;

		while (matSize[i] != '\0'){
			if(!isdigit(matSize[i])){
				isNum=0;
				break;
			}
			isNum=1;
			i++;
		}

		if(isNum){

			// Generating random values in matA and matB 
			for (int i = 0; i < atoi(matSize); i++) { 
				for (int j = 0; j < atoi(matSize); j++) { 
					matA[i][j] = rand() % 99; 
					matB[i][j] = rand() % 99; 
				} 
			} 

			printf("==========\n");
			printf("mode: sequential\n");
			printf("thread count: 1\n");
			printf("size: %d\n", atoi(matSize));
			printf("==========\n");

			double start = rtclock();
			multi(atoi(matSize));
			printf("sequential time: %f\n",rtclock()-start);
			return;
		}else{
			printf("Error: size must be an integer value\n");
			return;
		}

	}

	//parallel mode
	if (size>=2 && !strcmp(argv[1],"P")){
		if (size==2){
			printf ("Usage: ./mmm P [num threads] <size>\n");
			return;
		}
		if (size==3) {
			printf ("Error: parallel mode requires [num threads]\n");
			return;
		}
		if (size>4) {
			printf ("Error: too many Arguements for mode P\n");
			return;
		}

		char numThread[100];
		strcpy(numThread,argv[2]);
		strcpy(matSize, argv[3]);

		int isNumS = 0;
		int isNumT = 0;
		int i=0;

		while (matSize[i] != '\0'){
			if(!isdigit(matSize[i])){
				isNumS=0;
				break;
			}
			isNumS=1;
			i++;
		}
		i=0;

		while (numThread[i] != '\0'){
			if(!isdigit(numThread[i])){
				isNumT=0;
				break;
			}
			isNumT=1;
			i++;

			if (isNumT && isNumS){	

			// Generating random values in matA and matB 
				for (int i = 0; i < atoi(matSize); i++) { 
					for (int j = 0; j < atoi(matSize); j++) { 
						matA[i][j] = rand() % 99; 
						matB[i][j] = rand() % 99; 
					} 
				} 

				printf("==========\n");
				printf("mode: parallel\n");
				printf("thread count: %d\n",atoi(numThread));
				printf("size: %d\n", atoi(matSize));
				printf("==========\n");


				double start = rtclock();
				multi(atoi(matSize));
				double stopS = rtclock()-start;
				printf("sequential time: %f\n",stopS);

				//throwaway run for cache
				tmulti(atoi(numThread),atoi(matSize));

				double stopP=0.0;
				//get avg time for run 2 and 3
				for (int r=0; r<2; r++){
					start = rtclock();
					tmulti(atoi(numThread),atoi(matSize));
					stopP = stopP + (rtclock()-start);
				}

				stopP = stopP/2.0;

				printf("parallel time: %f\n",stopP);

				printf("speedup: %f\n",stopS/stopP);

				double largestError = 0.0000000;

				for (int n=0; n<atoi(matSize); n++){
					for (int m=0; m<atoi(matSize); m++){
						if (matC[n][m] - matD[n][m] > 0){
							largestError = matC[n][m] - matD[n][m];
						}
					}
				}
				printf("Verifying... largest error between parallel and sequential matrix: %f\n\n",largestError);
				return;
			}else{
				printf("Error: size and number of threads must be an integer value\n");
				return;
			}
		}
	}
	printf("Error: invalid input format\n");
}





int main(int argc, char const *argv[])
{
	int i=0;
	while(argv[i]!=NULL){
		i++;
	}

	processArgs(argv,i);

return 0;

}