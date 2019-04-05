#include "menu.h"

char * queryMenu() {
	int i = 0;

	static char buf[MAX_INPUT_LEN]; 
	fgets(buf, MAX_INPUT_LEN, stdin); 

	while (buf[i])
		i++;

    buf[i] = '\0';    //null-terminate buf

    buf[i-1] = '\0'; //chop of /n character


    return buf;
}


int processMenu(WordStats w) {
	char str[MAX_INPUT_LEN];
	//whether to exit or add more lines (op 5 or 4)
	int exit = 1;


	while(1){

		printf("*** WORD STATS MENU ***\n");
		printf("Enter 1 to print vowel and consonant frequency.\n");
		printf("Enter 2 to print word count.\n");
		printf("Enter 3 to print histogram.\n");
		printf("Enter 4 to return to inputting more strings.\n");
		printf("Enter 5 to quit.\n");
		printf("\n");

		strncpy(str, queryMenu(), MAX_INPUT_LEN);

	//OPTION 1 get stats
		if(strcmp(str, "1") == 0){
			getStats(w);
		}

	//OPTION 2 word count
		else if(strcmp(str, "2") == 0){
			getWordCount(w);

		}
	//OPTION 3 histogram
		else if(strcmp(str, "3") == 0){
			histogram(w);
		}

	//OPTION 4 input more strings
		else if(strcmp(str, "4") == 0){
			break;
		}
	//OPTION 5 quit
		else if(strcmp(str, "5") == 0){
			exit = 0;
			break;

		}else{
			printf("invalid input\n");
		}

	}
	return exit;
}








