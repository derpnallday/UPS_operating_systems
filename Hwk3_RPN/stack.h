
//node for stack
typedef struct sNode
{
	double val; //stored value
	struct sNode *next; //next node
} sNode;


typedef struct stack
{
	struct sNode *head;	//head
	int size; //stack size
} stack;


//prototypes
double pop(stack *s);
void push(stack *s, double newval);
int size(stack *s);
void printstack(stack *s);
void initstack(stack *s);