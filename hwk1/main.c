#include "menu.h"


char * queryUser() {
	int i = 0;

	static char buf[MAX_INPUT_LEN]; 
	fgets(buf, MAX_INPUT_LEN, stdin); 

	while (buf[i])
		i++;

    buf[i] = '\0';    //null-terminate buf

    buf[i-1] = '\0'; //chop of /n character


    return buf;
}



int main() {
	char str[MAX_INPUT_LEN];
	int exit = 1;

	while(1){
		printf("Enter strings (# to stop):\n");
		WordStats w;
		w = setHisto(w);

		//first loop grab words from user
		while(1){
			int i = 0;

			if(i ==0){
			}


			//copy user result input string
			strncpy(str, queryUser(), MAX_INPUT_LEN);

			//check for hashtag
			if(strcmp(str, "#") == 0){
				i = 0;
				break;
			}

			//update the stats
			w = update(str,w);

			i++;
		}


		//now process the results given the user input
		exit = processMenu(w);

		//if menu saw user wants to exit
		if(exit==0){
			printf("Exiting");
			break;
		}

	}
	return 0;
}



