// LIBRARIES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <getopt.h>
#include <math.h>
#include <stdbool.h>
#include <tgmath.h>
#include<time.h> 

// Definition for policy
#define RANDOM 'r'
#define FIFO 'f'
#define LRU 'l'
#define CLOCK 'c' 

//Definition for op
#define READ 'R'
#define WRITE 'W'
#define PRINT 'P'
#define PRINT_PAGE_TABLE 'P'
#define PRINT_FRAME_TABLE 'F'

#define SEGFAULT "ERROR: Segmentation Fault.\n"
#define BITMASK 0x0000FFFF
#define FRAME_SIG_BIT 0x80000000

//max string length
#define MAX_STR_LEN strlen("0x00000000")

//data structs for policies
uint *randomList;
struct queue *FQueue;
int *age;
int *clock_tab;
int hand;

// global tables
uint *pagetable;
uint *frametable;

// global vars
int PAGES;
int FRAMES;
int BLK_SIZE;
char R_POLICY;
char *FILENAME;

uint LOG2_PAGES;
uint LOG2_FRAMES;
uint LOG2_BLKSIZE;

// vmm.c
void readWrite(char op, uint addr);
void printOps(char op);
void convert_addr(uint addr);
uint page_replace(char policy);
void printInstruct(char op, uint vaddr, uint paddr, uint segfault, uint page_number, uint pagefault, uint evicted_page, uint evicted_frame);
uint bitMask(uint a, uint b);
void age_table();
int get_oldest();
void print_ages();
uint get_rbit(uint addr);
uint get_mbit(uint addr);
uint get_vbit(uint addr);
void update_hand();
uint frame_to_page(uint frame_addr);

// input.c
void input(int argc, char *argv[]);

// instruction.c
void instruction_file();
void parse_file(char *line);
char chooseOp(char *op);
uint handle_address(char *addr);


// amounts
double instruction_count;
double read_rate;
double write_rate;
double replace_rate;
double pagefault_rate;

//for queue.c
#define	EMPTY NULL	
#define TRUE 1
#define FALSE 0

// define queue's data members
struct queue
{
	struct qentry *head;
	struct qentry *tail;
	int size;
};


// entrtries for
struct qentry
{
	uint pg;
	struct qentry *next;
	struct qentry *prev;
};


//queue.c
void    printq(struct queue *q);

int	isempty(struct queue *q);
int	nonempty(struct queue *q);
int	isfull(struct queue *q);

uint	getlast(struct queue *q);
uint	dequeue(struct queue *q);
uint 	enqueue(uint pg, struct queue *q);
struct queue *newqueue();

uint bitMask(uint a, uint b);
uint removePage(uint pg, struct queue *q);