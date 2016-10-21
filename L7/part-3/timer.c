/******************** timer.c file *************************************/
#define LATCH_COUNT     0x00	   /* cc00xxxx, c = channel, x = any */
#define SQUARE_WAVE     0x36	   /* ccaammmb, a = access, m = mode, b = BCD */

#define TIMER_FREQ   1193182L	   /* clock frequency for timer in PC and AT */
#define TIMER_COUNT  TIMER_FREQ/60 /* initial value for counter*/

#define TIMER0       0x40
#define TIMER_MODE   0x43
#define TIMER_IRQ       0

u16 tick;
int h10,h1,m10,m1,s10,s1;
	char *nums[] = {"0","1","2","3","4","5","6","7","8","9", 0};


int enable_irq(u16 irq_nr)
{
  lock();
    out_byte(0x21, in_byte(0x21) & ~(1 << irq_nr));

}

int timer_init()
{
  /* Initialize channel 0 of the 8253A timer to e.g. 60 Hz. */

  printf("timer init\n");
  tick = 0; 
	h10=0;
	h1=0;
	m10=0;
	m1=0;
	s10=0;
	s1=0;
  out_byte(TIMER_MODE, SQUARE_WAVE);	// set timer to run continuously
  out_byte(TIMER0, TIMER_COUNT);	// timer count low byte
  out_byte(TIMER0, TIMER_COUNT >> 8);	// timer count high byte 
  enable_irq(TIMER_IRQ); 
}

/*===================================================================*
 *		    timer interrupt handler       		     *
 *===================================================================*/
int thandler()
{
	int i, r = row, c = col, prev = 0;
	PROC *p;
	struct tq *temp;
	
  tick++; 
  tick %= 60;
  if (tick == 0){                      // at each second

//increment seconds
	//tick%60 -- s1++
	//tick%(60*10) -- s10++
	//tick%(60*60) -- m1++
	//tick%(60*60*10) -- m10++
	//tick%(60*60*60) -- h1++
	//tick%(60*60*60*10) -- h10++

	s1++;
 	if (s1 > 0 && (s1%=10) == 0) s10++;
	prev = s10;
	if ((s10 % 6) == 0) s10 = 0;
	if (prev > 0 && (prev%=6) == 0) m1++;
	if (m1 > 0 && (m1%=10) == 0) m10++;
	prev = m10;
	if ((m10 % 6) == 0) m10 = 0;
	if (prev > 0 && (prev%=6) == 0) h1++;
	if (h1 > 0 && (h1%=10) == 0) h10++;
	if (h10 == 10) h10 = 0;

  }

	//HH:MM:SS in lower right corner
	time_pos();
	printf("%d\b%d\b:%d\b%d\b:%d\b%d\r\b\b", h10,h1,m10,m1,s10,s1);
	row = r;
	col = c;	
	move_cursor();

	//clear_time();

  out_byte(0x20, 0x20);                // tell 8259 PIC EOI

//if in usermode...do things...
//From Umode, add a command, itimer T, to let the running process request for
    //an INTERVAL timer of T seconds. The process becomes SLEEP unitl its interval
    //timer expires.
//NOTE: You MUST implement the itimer service by a timer queue in the kernel, 
      //as in Chapter 8.7.

	if (tick == 0 && running->inkmode == 1)
	{
		temp = tq;

		//print time queue
		printf("--time queue--> ");
		p = temp->proc;
		while (temp != 0 && temp->time > 0 && p->pid > 0 && p->pid < 10)
		{
			p = temp->proc;
			printf("P%d\b time %d\b --> ", p->pid, temp->time);
			temp = temp->next;
		}
		printf("\n");

		temp = tq;

		//everything tqe[], decrement by 1 second
		while (temp != 0)
		{
			temp->time--;
			temp = temp->next;
		}

		temp = tq->next;
		p = tq->proc;

		//if it equals 0, not in SLEEP anymore...kwakeup()?
		if (tq->time == 0)
		{
			tqe[p->pid].proc = 0;
			tqe[p->pid].next = 0;
			tq = temp;
			kwakeup(p->pid);
		}		
	}
}

int itimer()
{
	char *c; char *d;
	int i;
	struct tq *temp, *prev;

	printf("P%d\b set itimer: ", running->pid);
	gets(c);

	i = 0;   d = nums[0];
	while (d){
	 if (strcmp(d, c)==0)
	    break;
	 i++;  d = nums[i];
	} 	

	i = atoi(c);

	printf("%d\b seconds P%d\b goes to sleep\n", i, running->pid);

	//next, time, proc
	if (tq == &tqe[0])
	{
		tqe[running->pid].time = i;
		tqe[running->pid].proc = running;
		tq = &tqe[running->pid];
	}

	else
	{
		temp = tq;
		prev = 0;
		
		while (temp->time < i && temp->next != 0)
		{
			prev = temp;
			temp = temp->next;
		}

			tqe[running->pid].time = i;
			tqe[running->pid].proc = running;

		if (temp->time < i)
		{
			temp->next = &tqe[running->pid];
		}
		else if (prev != 0)
		{
			tqe[running->pid].next = temp;
			prev->next = &tqe[running->pid];
		}
		else
		{
			tqe[running->pid].next = temp;	
			tq = &tqe[running->pid];		
		}

	}

printf("forking\n");	
	fork();
	ksleep(running->pid);
}
