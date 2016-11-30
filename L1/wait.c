//              wait.c file:

//YOUR ksleep(), kwakeup(), kwait() functions


int ksleep(int event)
{
running->event = event;
running->status = SLEEP;

put_proc(&sleepList, running);
printf("sleep proc %d\n", running->pid);
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
  printf("wakeup proc %d\n", p->pid);
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

	while (1) { // search PROCs for a child

				// Traverse all the processes, look for a zombie child of the running process
		for (i = 1; i<NPROC; i++)
		{ // exclude P0
			p = &proc[i];
			
			// The runining proc is a daddy. Time to pay child support
			if ((p->status != FREE) && (p->ppid == running->pid))
			{
				hasChild = 1;

				// If the running proc's child is dead
				if (p->status == ZOMBIE)
				{ // lay the dead child to rest.This shit is morbid
					
					*status = p->exitCode; // collect its exitCode
					p->status = FREE; // free its PROC
					put_proc(&freeList, p); // to freeList
					return(p->pid); // return its pid
				}

			}
		}

		if (!hasChild)
		{
			return -1;
			// no child, return ERROR
		}

		ksleep(running); // still has kids alive: sleep on PROC address
		return 1;
	}



}
