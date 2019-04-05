#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#define HISTORY_LEN 100

char* history [HISTORY_LEN*sizeof(char*)];

//prints message of the day
void motd(){
//grab the MOTD file path
	char c;
	FILE *fptr;

	char filename[80];
	strcpy(filename, getenv("HOME"));
	strcat(filename, "/.dsh_motd");

	//if file exists open and print
	if(access( filename, F_OK ) == 0) {

		// Open file 
		fptr = fopen(filename, "r"); 

		//if file is no openable leave
		if (fptr == NULL) { 		
			printf("Cannot open file \n"); 
		} else {
    	// Read contents from file 
			c = fgetc(fptr); 

			while (c != EOF) { 
				printf ("%c", c); 
				c = fgetc(fptr); 
			} 
		}
		fclose(fptr); 
	}
}


//grabs user input
char * queryUser() {
	int i = 0;

	static char buf[100]; 
	fgets(buf, 100, stdin); 

	while (buf[i])
		i++;

    buf[i] = '\0';    //null-terminate buf
    buf[i-1] = '\0'; //chop of /n character

    return buf;
}

//split input bt delim and return array of tokens
char  **parseInput(char *buf, char *delim){

	char temp[256*sizeof(buf)];
	//for 
	strcpy(temp,buf);
	
	int position = 0;

	char **tokens = (char **)malloc(256 * sizeof(char*));
	char *token;

	token = strtok(temp, delim);

	while (token != NULL) {
		tokens[position] = token;
		position++;
		token = strtok(NULL, delim);
	}

	tokens[position]=NULL;

	return tokens;
}

//find length of args
int argLen(char **args){
	int j = 0;
	//get size of arguments
	while(args[j] != NULL){
		j++;
	}
	return j;
}

//check for &
int backgroundCheck(char **args){
	//true if last arguement is amp
	if (*args[argLen(args)-1] == '&'){
		return 1;
	}
	return 0;

}


//check if path exists and execute
int processPath(char **args){
//seperate path from args
	char **tokens = (char **)malloc(256 * sizeof(char*));

	int j = 1;
	//get all args not file path and &
	while(args[j] != NULL){
		tokens[j] = args[j];
		j++;
	}

	//check if path exists
	if (access( args[0], F_OK ) != 0){
		//return false if path does not exist
		printf("could not find path\n");
		return 0;
	}

	//check for &
	if (backgroundCheck(args)){
		//chop of & for args
		args[argLen(args)-1] = NULL;

		//run child in background
		if (!fork()){
			execv(args[0] , tokens);
		}

	}else{
		//run in foreground
		if (!fork()){
			execv(args[0] , tokens);
		}else{
			wait(NULL);
		}
	}
	return 1;
}

//return true if execute path, false if failed
int execPath(char *path, char **args){
//if command found in cwd exec
	if (access( path, F_OK ) == 0){
		//if running in background
		if(backgroundCheck(args)){
			//chop of & for args
			args[argLen(args)-1] = NULL;

			//run child in background
			if (!fork()){
				execv(path , args);
			}
			return 1;

		//run in foreground
		}else{
			//run in foreground
			if (!fork()){
				execv(path , args);
			}else{
				wait(NULL);
			}
			return 1;
		}
	}

	return 0;
}

//process a given command
int processCommand(char **args){
	int status = 0;
	//check if exists in cwd
	char cwd[100];

	getcwd(cwd, 100);
	//append funct to path
	strcat(cwd,"/");
	strcat(cwd,args[0]);

	if(execPath(cwd, args)){
		return 1;
	}

	//check path directories next
	char* path = getenv("PATH");
	char** parsed;
	char pdelim[]= ":";
	//get all path vars
	parsed = parseInput(path,pdelim);

	int i=0;
	char lookUp[256*sizeof(char)];
	char temp[256*sizeof(args[0])];
	strcpy(temp,args[0]);
	
	//check if path exists from lists
	while(parsed[i] != NULL){
		strcpy(lookUp,parsed[i]);
		strcat(lookUp,"/");
		strcat(lookUp,temp);

		status = execPath(lookUp, args);
		//if executed exit
		if(status){
			return 1;
		}
		i++;
	}

	printf("ERROR: %s not found!\n",args[0]);

	return 1;
}






int evaluate(char **args){
	//check for builtin commands
	if (!strcmp(args[0],"exit")){
		return 0;
	}
		if (!strcmp(args[0],"pwd")){
		char cwd[100];
		getcwd(cwd, 100);
		printf("%s\n",cwd);
		return 1;
	}
		if (!strcmp(args[0],"cd")){
			if(args[1]==NULL){
				chdir(getenv("HOME"));
			}else{
				chdir(args[1]);
			}
		return 1;
	}
		if (!strcmp(args[0],"history")){
			int i=0;
			printf("\n");
			while(history[i] != NULL){
				printf("%s\n",history[i]);
				i++;
			}
			printf("\n");
		return 1;
	}


	//if looking for file path
	if (args[0][0] == '.' || args[0][0] == '/'){
		//execute given file path
		int pathExist = processPath(args);

		if (!pathExist){
			printf("File path does not exist\n");
		}
		return 1; //continue loop

	}else{
		return processCommand(args);
	}

	return 1;
}


void loop(void){
	char buf[100];
	char **args;
	int status=0;

	//read commands by user
	//parse commands and args
	//excecute

	int i=0;

	do{
		printf("%s","dsh> " );

		//get input
		strncpy(buf, queryUser(), 100);

		history[i] = (char*)malloc(256 * sizeof(char*));

		strcpy(history[i],buf);

		char delim[] = " ";

		args = parseInput(buf, delim);

		status = evaluate(args);
		i++;
	}while(status);

}


int main(int argc, char const *argv[])
{
	//check for MOTD and print if found
	motd();

	//run REPL loop
	loop();
	//perform cleanup for shutdown

	return 0;
}


