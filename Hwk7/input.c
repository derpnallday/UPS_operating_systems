#include "vmm.h"

//read in command line args

void input(int argc, char *argv[]){

	FILENAME = argv[argc-1];

	//init
	PAGES = -1; 
	FRAMES = -1; 
	BLK_SIZE = -1; 
	R_POLICY = RANDOM;


	//parse args until empty
	int opt = 0;

	for (int i=0; i<argc; i++){
		


		
	}


	while ((opt = getopt(argc, argv, "p:f:b:r:")) != -1){
		//input from user
		switch(opt){
		//num pages
			case 'p':
			PAGES = atoi(optarg);
		//ensure mult of 2 
			if(!(PAGES % 2 == 0))
				PAGES = 8; //default value
			break;
		//num frames
			case 'f':
			FRAMES = atoi(optarg);
		//mult of 2
			/*if(!(FRAMES % 2 == 0))
				FRAMES = 4; // default value*/
			break;
			//block size
			case 'b':
			BLK_SIZE = atoi(optarg);

			if (!(BLK_SIZE % 2 == 0))
				BLK_SIZE = 1024; // default to 1024
			break;
			//check policy
			case'r':
			R_POLICY = optarg[0];

			//set default  to random if r is not an option
			if(R_POLICY != FIFO && R_POLICY != LRU && R_POLICY != CLOCK && R_POLICY != RANDOM)
				R_POLICY = RANDOM;

			break;

			//invalid input/ just exit
			default:
			exit(EXIT_FAILURE);
		} 

	}


	printf("PAGES: %d\n", PAGES);
	printf("FRAMES: %d\n", FRAMES);
	printf("BLK_SIZE: %d\n", BLK_SIZE);
	printf("R_POLICY: %c\n", R_POLICY);
	printf("FILENAME: %s\n", FILENAME);


	//grab log of values
	LOG2_PAGES =  log2(PAGES);
	LOG2_FRAMES = log2(FRAMES);
	LOG2_BLKSIZE = log2(BLK_SIZE);

		//check for invalid values and size
	if (!((LOG2_PAGES != -1) && (LOG2_BLKSIZE != -1) && ((LOG2_PAGES+LOG2_BLKSIZE) <= 32))){
		exit(EXIT_FAILURE);
	}
}