	#include "vmm.h"

	//grab significant bits
uint bitMask(uint a, uint b){
	uint r = 0;
	for (uint i=a; i<=b; i++)
		r |= 1 << i;
	return r;
}

	//manage read/write
void readWrite(char op, uint v_addr){

		// vars 
	uint frame, page_num, p_addr, fr_evict_ind, pg_evict_ind, segfault, pagefault;
		//init values
	segfault = 0;
	pagefault = 0;
	pg_evict_ind = -1;

		// get page num
	page_num = v_addr >> LOG2_BLKSIZE;

		//pte bit num
	uint offset = v_addr << (32-(LOG2_BLKSIZE));
	offset >>= (32-(LOG2_BLKSIZE));
		//for rand list loop
	int c;
		// pg does not exist
	if (!(page_num >= 0 && page_num < PAGES)){
		segfault = 1;
	} 

		// check valid bit
	else if ((pagetable[page_num] >> 31) == 1){

			// grab frame
		frame = pagetable[page_num] & bitMask(0,28);
		frame <<= LOG2_BLKSIZE;
		p_addr = frame; 

			//age for LRU
		if(R_POLICY == LRU){
			age[pagetable[page_num] & bitMask(0,28)] = 0;
			age_table();
		}

		//set current process to cold and update clock hand
		if(R_POLICY == CLOCK){
			;
			uint r=0;
			//set r-bit to 0
			r = (pagetable[page_num] & bitMask(29,29));
			pagetable[page_num] = r^pagetable[page_num];
			update_hand();
		}
	} 
		// valid bit invalid
	else {
		//page faults
		pagefault = 1;
			// find empty frame
		int i;
		int frame_found = 0;
		for (i=0; i<FRAMES; i++){

			if (frametable[i] == FRAME_SIG_BIT){

				frametable[i] = 0x00000000; 

					frametable[i] |= page_num; // set frame entry

					pagetable[page_num] |= 0x80000000; // 100

					//bit for write
					if (op == WRITE) 
						pagetable[page_num] = 0xE0000000; //111

					//bit for read
					else if (op == READ) 
						pagetable[page_num] = 0xA0000000; //101
					
					
					pagetable[page_num] |= i; // adds frame number to PTE

					printf("table %d\n", pagetable[page_num]& bitMask(0,28)  );

					frame_found = 1; // let things be set to found

					p_addr = i << LOG2_BLKSIZE;
					p_addr |= offset;

					printf ("policy choose\n");

					switch (R_POLICY){
						case RANDOM:
							//find first empty frame to populate
						for (c=0; c<FRAMES; c++){
							if (randomList[c] == -1){
								randomList[c] = page_num;
								break;
							} 
						}
						break;
						case FIFO:
							//add to queue
						enqueue(i,FQueue);
						break;

						case LRU:
							//init age of frame
						age[pagetable[page_num]& bitMask(0,28)] = 0;
						age_table();
						break;

						case CLOCK:
						update_hand();
						break;
					}

					// break from loop
					break;
				} 
			}

			// if not in table
			if (!frame_found){
				// pagefault
				pagefault = 1;
				bool isEvictable = false;
				switch (R_POLICY){
					case RANDOM:
					// random entry to throw
					//set seed
					srand(time(0));
					fr_evict_ind = (rand() % (FRAMES-1));
					pg_evict_ind = frametable[fr_evict_ind];

					//loop thorugh page list for page to evict
					for (c=0; c<FRAMES; c++){					
							//handle page
						if ((randomList[c] == pg_evict_ind)){
							randomList[c] = page_num;
							isEvictable = true;
							break;	
						} 

							//random assignment not in list
						if ((c == FRAMES-1 && !isEvictable) || page_num == pg_evict_ind){
							fr_evict_ind = (rand() % (FRAMES-1));
							pg_evict_ind = frametable[fr_evict_ind];
							c--;
						}
					}
					break;

					case FIFO:
					//trhow out head
					pg_evict_ind = FQueue->head->pg;
					fr_evict_ind = pagetable[pg_evict_ind] & bitMask(0,28);
					//reorder queue
					dequeue(FQueue);
					enqueue(page_num, FQueue);
					isEvictable = true;
					break;

					case LRU:
					//oldest entry
					pg_evict_ind = get_oldest();
					fr_evict_ind = pagetable[pg_evict_ind] & bitMask(0,28);
					break;

					case CLOCK:

					//set r bits and stop get index if r=0
					while (get_rbit(pagetable[frame_to_page(hand)]) > 0){
						int pageInd = frame_to_page(hand);
						uint r = 0;
						r = (pagetable[pageInd] & bitMask(29,29));
						pagetable[pageInd] = r^pagetable[pageInd];
						update_hand();
					}

					//get evicted page and frame
					fr_evict_ind = hand;
					pg_evict_ind = (frametable[fr_evict_ind] << 1) >> 1;

					break;
				}

				//evict the page
				pagetable[pg_evict_ind] = 0x0000000;

				//table with ops
				if (op == WRITE)
					pagetable[page_num] = 0xE0000000;
				else if (op == READ)
					pagetable[page_num] = 0xA0000000;
				

				//update pgtable /frametable
				pagetable[page_num] |= fr_evict_ind;
				frametable[fr_evict_ind] = page_num;	

				//grab pyhsical address
				p_addr = fr_evict_ind << LOG2_BLKSIZE;
				p_addr |= offset;
			} 

		}

		printf("\n");
		printInstruct(op, v_addr, p_addr, segfault, page_num, pagefault, pg_evict_ind, fr_evict_ind);
	}


	//handle print table/ print frames
	void printOps(char op){

		int i=0,c=0;
		//printing page table
		if (op == PRINT_PAGE_TABLE){

			//header
			printf("Page\t|V,M,R,Frame\n");

			//loop thoruhg page entries
			for (i=0; i<PAGES; i++){
				
				printf("%i\t|",i); 

				//check bit shift
				if ((pagetable[i] >> 31) == 1){
					c = 1;
					printf("%d,",c);

					//m and r bit
					printf("%i,%i",get_mbit(pagetable[i]),get_rbit(pagetable[i]));
					
					// frame 
					printf(",%i", pagetable[i] & bitMask(0,28));
				} else {
					c=0;
					printf("%d,",c);
					printf("-,");
					printf("-,");
					printf("-");
				}
				printf("\n");
			}

		} 
		//for printing frames
		else if (op == PRINT_FRAME_TABLE){
			//header
			printf("Frame\t|F,Page\n");
			//print frames
			for (i=0;i<FRAMES;i++){
				printf("%i\t|",i);
				if (frametable[i] != 0x80000000){
					printf("%i,%i\n",((frametable[i] & bitMask(30,31)) == 1 ? 1 : 0), frametable[i] & bitMask(0,5));
				} else {
					printf("1,-\n");
				}
			}
			printf("\n");
		}
	}

	//print out each instruction from file
	void printInstruct(char op, uint v_addr, uint p_addr, uint segfault, uint page_num, uint pagefault, uint pg_evict_ind, uint fr_evict_ind){

		//check op type first
		if (op == READ){
			printf("START_READ\n");
			read_rate++;
		}
		else {
			printf("START_WRITE\n");
			write_rate++;
		}

		//virtual addr
		printf("\tvirt_addr=0x%.8x\n",v_addr);
		
		//print faults
		if (segfault){
			printf("\tSegmentation Fault: illegal_page=%i\n",page_num);
		} else {
			if (pagefault){
				printf("\tPage fault: page=%i\n", page_num);
				pagefault_rate++;
			}

			//chec for page eviction
			if (pg_evict_ind != -1){
				// if modified bit is 1 and the current page and you're replacing, then it is a write out
				printf("\tPage replacement: evicted_page=%i (frame=%i), ", pg_evict_ind,fr_evict_ind);
				if(op == WRITE){
					printf("writeout=false\n");
				} else{
					printf("writeout=true\n");
				}
				//increment replace
				replace_rate++;

			}	
			printf("\tphys_addr=0x%.8x\n",p_addr);	
		}

		// indicate end of op
		if (op == READ){
			printf("END_READ\n\n");
		}
		else if (op == WRITE){
			printf("END_WRITE\n\n");
		}
	}


	//age the table if frame is set
	void age_table(){
		for (int i=0; i<FRAMES; i++){
			if (age[i] != -1) age[i]++;
		}
	}

	//grab oldest process for LRU
	int get_oldest(){
		int p = -1;
		int c = -1;
		for (int i=0; i<FRAMES; i++){
			if (age[i] >p -1){
				p = age[i];
				c = i;
			} 
		}

		return c;

	}

	//check ages of processes
	void print_ages(){
		printf("ages :");
		for (int i=0; i<FRAMES; i++){
			printf("%d, ",age[i]);
		}
		printf("\n");
	}


void showbits(uint x){
    int i; 
    printf("\t\tbin: ");
    for(i=(sizeof(int)*8)-1; i>=0; i--){
        (x&(1u<<i))?putchar('1'):putchar('0');
    }
	printf("\n");
}

	//addr conversion to hex and dec
	void convert_addr(uint addr){
		printf("\t\thex: 0x%.8X\n",addr); 
		printf("\t\tdec: %i\n",addr); 
		showbits(addr);
		printf("\n");
	}


	uint get_rbit(uint addr){
		return (addr & bitMask(29,29)) >> 29;
	}

	uint get_mbit(uint addr){
		return (addr & bitMask(30,30)) >> 30;
	}

	uint get_vbit(uint addr){
		return (addr) >> 31;
	}

	//change clock hand
	void update_hand(){
		hand++;
		if (hand >= FRAMES){
			hand=0;
		}
	}

//get page num from frame
uint frame_to_page(uint frame_addr){
		return ((frametable[frame_addr] << 1) >> 1);
}





