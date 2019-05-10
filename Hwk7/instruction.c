#include "vmm.h"

void instruction_file(){
	// get cwd
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));

	strcat(cwd, "/"); 

	// path to file
	char *filePath = strcat(cwd,FILENAME);
	char buff[255];
	FILE *f; 

	//open path if file is found
	f = fopen(filePath, "r");

	int c =0;
	if (f){
		while (fgets(buff, 255, (FILE*) f) != NULL){
			c++;
  			//read in instructions
  			parse_file(buff);
  		}
  		fclose(f);
	} 
	else {
		printf("File not found\n");
	}
}

void parse_file(char *line){
	char op;
	uint addr;
	char *token;
	int i = 0;

	//break up line input
  	token = strtok (line," \n");

  	while (token != NULL){
  		//if instruction is none of these ops handle bits
    	if ((token[0] != READ) && (token[0] != WRITE) && (token[0] != PRINT)){
    		if (MAX_STR_LEN == strlen(token)){
    			addr = handle_address(token);
    		} 
    	}
    	//evaluate instruction
    	else {
    		op = chooseOp(token);
    	}

    	i++;
    	token = strtok (NULL, " \n");
  	}

  	//read/write ops
  	if (op == READ || op == WRITE) {
  		instruction_count++;
  		readWrite(op, addr);
  		//printq(LQueue);	
  	}
  	//print ops
  	else if (op == PRINT_FRAME_TABLE || op == PRINT_PAGE_TABLE) {
  		//printOps(op);
  	}
  	
  	printf("-------------\n\n");
  	printOps('F');
  		printf("\n");
  	printOps('P');
  	printf("-------------\n\n");
  	
}

uint handle_address(char *addr){
	//convert addrbinto numeric
	uint num = (uint) strtol(addr,NULL, 16);
	return num;
}

//handles R/W/PF/PP
char chooseOp(char *op){
	// check first char if R/W
	if (op[0] == 'R'){
		return 'R';
	}
	else if (op[0] == 'W'){
		return 'W';
	}

	// if neither check pp or pf on second char
	else if (op[1] != '\0'){
		if (op[1] == 'P'){
			return 'P';
		}
		else if (op[1] == 'F'){
			return 'F';
		} 
	}
	printf("bad OP\n");
	return '\0';
}