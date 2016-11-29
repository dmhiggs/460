/* FUNCTIONS:
	ksleep(int event)
	remove_proc(PROC **list, PROC *p)
	get_nproc()
	ready(PROC *p)
	kwakeup(int event)
	kwait(int *status)
	kexit(int exitValue)
*/


//              wait.c file:

//YOUR ksleep(), kwakeup(), kwait() functions


int ksleep(int event)
{
	running->event = event;
	running->status = SLEEP;

	//put_proc(&sleepList, running);
	enqueue(&sleepList, running);
	printf("go to sleep proc %d\n", running->pid);
	tswitch();
}


PROC *remove_proc(PROC **list, PROC *p)
{
PROC *x = list;
PROC *y;
if (x==0){
	return -1;}
if (x ==p){
	(*list) = x->next;
	return p;}
y = x->next;
while(y){
	if (y==p){
		x->next = y->next;
		return y;}
	}
return -1;
}


int get_nproc()
{
	int i = 0;
	int active_procs = 0;
	while (i < NPROC)
	{
		if (proc[i].status == READY)
		{
			active_procs++;
		}
		i++;
	}
	return active_procs;
}

int ready(PROC *p)
{
	p->event = 0;
	p->status = READY;
	enqueue(&readyQueue, p);
	printf("time to wakeup proc %d\n", p->pid);
}

int kwakeup(int event)
{
int i;
PROC *p;

for (i = 1; i<NPROC; i++){
	p = &proc[i];
	if (p->status == SLEEP && p->event == event){
		p->event = 0;
		p->status = READY;
		remove_proc(&sleepList, p);
		enqueue(&readyQueue, p);
		}
	}

}


int kwait(int *status)
{
	PROC *p; 
	int i = 0;
	int hasChild = 0;

	while (1) 
	{ 
		for (i = 1; i<NPROC; i++)
		{ // exclude P0
			p = &proc[i];
			
			if ((p->status != FREE) && (p->ppid == running->pid))
			{
				hasChild = 1;

				if (p->status == ZOMBIE)
				{	
					*status = p->exitCode;
					p->status = FREE; 
					put_proc(&freeList, p);
					nproc--;
					return(p->pid);
				}

			}
		}

		if (!hasChild)
		{
			return -1;
			// no child, return ERROR
		}
		ksleep(running); 
	}
}

int kexit(int exitValue)
{
	int i;
	PROC *p;
	
	//close open file
	for (i=0;i<NFD;i++)
	{
		running->fd[i] != 0;
	}

	for (i = 1; i< NPROC; i++)
	{
		p = &proc[i];
      		if (p->status != FREE && p->ppid == running->pid)
		{
          		p->ppid = 1;
		        p->parent = &proc[1];
      		}
  	}

	running->exitCode=exitValue;
	running->status=ZOMBIE;

	kwakeup(running->parent);
	kwakeup(&proc[1]);
	tswitch();
}



/*int kexit(int exitValue)
{
	int i, wakeupP1 = 0;
	PROC *p;

	if (running->pid == 1 && nproc>2)
	{
		printf("other procs, p1 can't die\n");
		return -1;
	}

	for (i = 1; i < NPROC; i++){
	p = &proc[i];
	if (p->status != FREE && p->ppid == running->pid){
		p->ppid = 1;
		p->parent = &proc[1];
		wakeupP1++;
		}
	}

running->exitCode = exitValue;
running->status = ZOMBIE;


kwakeup(&proc[running->ppid]);

if (wakeupP1){
	kwakeup(&proc[1]);
	}

tswitch();
}*/
