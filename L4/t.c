/* FUNCTIONS:
	init()
	scheduler()
	int80h()
	set_vector(u16 vector, u16 handler)
	main()
*/


/**************************************************
  bio.o, queue.o loader.o are in mtxlib
**************************************************/
/* #include "bio.c" */
/* #include "queue.c" */
/* #include "loader.c" */


#include "type.h"


PIPE pipe[NPIPE];
OFT oft[NOFT];
PROC proc[NPROC], *running, *freeList, *readyQueue, *sleepList;
int procSize = sizeof(PROC);
int nproc = 0;
int color;
int body();
char *pname[]={"Sun", "Mercury", "Venus", "Earth",  "Mars", "Jupiter", "Saturn", "Uranus", "Neptune" };

#include "wait.c"             // YOUR wait.c   file
#include "kernel.c"           // YOUR kernel.c file
#include "int.c"              // YOUR int.c    file
#include "fe.c"
#include "pipe.c"
int init()
{
    	PROC *p; 
	int i,j;
	color = 0x0C;
    	printf("init ....\n");

    	for (i=0; i<NPROC; i++)
	{   // initialize all procs
	        p = &proc[i];
	        p->pid = i;
	        p->status = FREE;
	        p->priority = 0;  
	        strcpy(proc[i].name, pname[i]);
	        p->next = &proc[i+1];

		for(j = 0; j<NFD;j++)
		{
			p->fd[j]=0;
		}
    	}

    	freeList = &proc[0];      // all procs are in freeList
    	proc[NPROC-1].next = 0;
    	readyQueue = sleepList = 0;

	for(i=0;i<NOFT;i++)
	{
		oft[i].refCount=0;
	}
	for(i=0;i<NPIPE;i++)
	{
		pipe[i].busy=0;
	}

    	/**** create P0 as running ******/
    	p = get_proc(&freeList);
    

	p->status = RUNNING;
    	p->ppid   = 0;
    	p->parent = p;
    	running = p;
    	nproc = 1;
    	printf("done\n");
} 

int scheduler()
{

    if (running->status == RUNNING){
	running->status = READY;
        enqueue(&readyQueue, RUNNING);
	}

     running = dequeue(&readyQueue);
     running->status = RUNNING;
     color = running->pid + 0x0A;
}


int int80h();
int set_vector(u16 vector, u16 handler)
{
     // put_word(word, segment, offset)
     put_word(handler, 0, vector<<2);
     put_word(0x1000,  0,(vector<<2) + 2);
}
            
main()
{
    printf("MTX starts in main()\n");
    init();      // initialize and create P0 as running
    set_vector(80, int80h);

    kfork("/bin/u1");     // P0 kfork() P1, makes it go into user folder?

    while(1){
      printf("P0 running\n");

      if (nproc==2 && proc[1].status != READY)
	{
		printf("no runable process\n");
	}

      while(!readyQueue);
      printf("P0 switch process\n");
      tswitch();         // P0 switch to run P1
   }
}
