//                     kernel.c file:

  //           YOUR kernel.c file as in LAB3

	//     MODIFY kfork() to

extern int goUmode();
PROC *kfork();

int makeUimage(char *filename, PROC *p)
{
	u16 i, segment;
	segment = (p->pid +1)*0x1000;
	load(filename, segment);

	for (i=1;i<=12;i++)
	{

		put_word(0, segment, -2*i);
	}

	put_word(0x0200, segment, -2*1);
	put_word(segment, segment, -2*2);
	put_word(segment, segment, -2*11);
	put_word(segment, segment, -2*12);

	p->usp = -2*12;
	p->uss = segment;
	return 0;


}
 



PROC *kfork(char *filename)
{
  PROC *p;
  int  i, child;
  u16  segment;

  /*** get a PROC for child process: ***/
  if ( (p = get_proc(&freeList)) == 0){
       printf("no more proc\n");
       return 0;
  }
  /* initialize the new proc and its stack */
  p->status = READY;
  p->ppid = running->pid;
  p->parent = running;
  p->priority  = 1;                 // all of the same priority 1

  // clear all SAVed registers on stack
  for (i=1; i<10; i++)
      p->kstack[SSIZE-i] = 0;
 
  // fill in resume address
  p->kstack[SSIZE-1] = (int)body;
  // save stack TOP address in PROC
  p->ksp = &(p->kstack[SSIZE - 9]);

  enqueue(&readyQueue, p);
  nproc++;
  if (filename){
    /******** create Umode image *******************/
    segment = 0x1000*(p->pid+1);
    makeUimage("/bin/u1", p);
  }
  printf("Proc %d kforked a child %d at segment=%x\n",
          running->pid, p->pid, segment);
  return p;
}





int body()
{

char c;
printf("proc %d starts from body()\n", running->pid);
while(1){
	printList("freelist ", freeList);
	printList("readyqueue ", readyQueue);
	printList("sleeplist ", sleepList);
	printf("proc %d running: parent = %d\n", running->pid, running->ppid);
	printf("[s|f|w|q|u]: ");
	c = getc();
	printf("%c\n", c);
	switch(c){
		case 's': do_tswitch();
			break;
		case 'f': do_kfork();
			break;
		case 'w': do_kwait();
			break;
		case 'u': goUmode();
			break;
		case 'q': do_exit();
			break;
		}
	}

}










int do_kfork()
{

PROC *p = kfork("/bin/u1");
if (p == 0){
	printf("kfork failed\n");
	return -1;
	}

printf("PROC %d kfork child %d\n", running->pid, p->pid);
return p->pid;
}







int do_tswitch()
{

tswitch();
}






int do_kwait()
{
int wait = 0;
int status = 0;
printf("wait for dead child\n");
wait = kwait(status);

if (wait == -1){
	printf("no child\n");
	return -1;
	}

printf("free zombie %d with code %d\n", wait, status);

}


int do_exit()
{
int exitValue;

  if (running->pid == 1 && nproc > 2){
      printf("other procs still exist, P1 can't die yet !%c\n",007);
      return -1;
  }

  printf("enter an exitValue (0-9) : ");
  exitValue = (getc()&0x7F) - '0'; 
  printf("%d\n", exitValue);

  kexit(exitValue);
}

int kexit(int exitValue)
{

int i, wakeupP1 = 0;
PROC *p;


if (running->pid == 1 && get_nproc()>2){
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


kwakeup(running->parent);

if (wakeupP1){
	kwakeup(&proc[1]);
	}

tswitch();
}








int ksp()
{
    //WRITE C code to print PROC information
int count = 1;
printf("running proc stats:\n name: %s, pid: %d, status: %d\n", running->name, running->pid, running->status);
printf("......other procs........\n");



while(count<9){
	printf("proc[%d] = %s, status = %d, pid = %d, ppid = %d\n", count, (&proc[count])->name, (&proc[count])->status, (&proc[count])->pid, (&proc[count])->ppid);
	count++;
	}

return 1;

}









int kchname(char *name)
{
    //WRITE C CODE to change running's name string;
//go to running process at a specific value 
	//get_byte(running->uss, value)
int i=0;
char c;
char newname[32];
c = get_byte(running->uss, *name);

while (c!='\0' && i<32)
{	
	c = get_byte(running->uss, name + i);
	newname[i] = c;
	i++;
}
//return byte into a character
//increment value and place in string until returned byte is null or until 32 characters bc max length is 32
//string copy new name into old name
strcpy(running->name, newname);
}









int kkfork()
{
  //use you kfork() in kernel;
  //return child pid or -1 to Umode!!!

//1. creat a child proc ready to run from body() in Kmode

//2. segment = (child pid +1)*0x1000

//if filename
	//3. load filenam to child segment as its umode image
	//4. set up child's ustack for it to return to umode to execute the loaded image
//5. return child proc pointer

	PROC *p = kfork("/bin/u1");
	if (p==0)
		return -1;
	return (p->pid);
}

int ktswitch()
{
    tswitch();
}

int kkwait(int *ustatus)
{

  //use YOUR kwait() in LAB3;
  //return values to Umode!!!

	int ans = 0;
	int status = 0;
	int pid = 0;
	pid = kwait(&status);
	put_word(status, running->uss, ustatus);

	return pid;
}

int kkexit(int value)
{
    //use your kexit() in LAB3
    //do NOT let P1 die

	kexit(value);
	return value;
}















int kmode()
{
  body();
}
