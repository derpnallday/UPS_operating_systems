#include "vmm.h"

int main(int argc, char *argv[]){

	if (argc == 1){
		printf("enter input in format: [-p num_pages] [-f num_frames] [-b block_size] [-r rep_policy] <inst_file>\n");
		printf("available policies are [-r random] [-f fifo] [-l lru] [-c clock]\n");
		return 0;
	}

	instruction_count = 0;
	read_rate = 0;
	write_rate = 0;
	pagefault_rate = 0;
	replace_rate = 0;
	hand = 0;

	input(argc, argv);


	//allocate vars
	pagetable = (uint *) malloc (PAGES * sizeof(uint));
	frametable = (uint *) malloc (FRAMES * sizeof(uint));
	randomList = (uint *) malloc (FRAMES * sizeof(uint));

	age = (int *) malloc (FRAMES * sizeof(int));
	clock_tab = (int *) malloc (FRAMES * sizeof(int));

	FQueue = (struct queue *) malloc (sizeof(struct queue));
	FQueue->head = NULL;
	FQueue->tail = NULL;
	FQueue->size = 0;


	//init tables
	int i;
	for (i=0; i<PAGES; i++){
		pagetable[i] = 0x00000000;
	}
	for (i=0; i<FRAMES; i++){
		frametable[i] = 0x80000000;
		randomList[i] = -1;
		age[i] = -1;
		clock_tab[i]=0;
	}



	instruction_file();

	printf("\ninst_count=%.0f ",instruction_count);
	printf("(read=%.2f%%, write=%.2f%%), ", (read_rate/instruction_count)*100, (write_rate/instruction_count)*100);
	printf("page_faults=%.0f (%.2f%%), replacements=%.0f (%.2f%%)\n", pagefault_rate, (((pagefault_rate)/instruction_count)*100), replace_rate, ((replace_rate/instruction_count)*100));

	free(frametable);
	free(pagetable);
	free(FQueue);
	free(age);

	return 0;
}
