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
	PROC *p;
	int i, r = row, c = col, prev = 0;

	
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


	clear_time();	


	//if in usermode...do things...
		if (p->inkmode)
		{

		}
		//when proc is scheduled to run, set its PROC.time to a time slice value -- like 5 seconds


		//per second, decrement running's time in Umode only
		if (p->time>0)
		{

		}

		//when PROC's time expires, switch process
			//do NOT switch process while in Kmode
				//MTX kernel NOT multi-processor kernel
		if (p->time==0)
		{

		}


/*
      printf("1 second timer interrupt in ");
      running->inkmode > 1 ? putc('K') : putc('U');
      printf("mode\n");
*/
  }

	//HH:MM:SS in lower right corner
	time_pos();
	printf("%d\b%d\b:%d\b%d\b:%d\b%d\r\b\b", h10,h1,m10,m1,s10,s1);
	row = r;
	col = c;	
	move_cursor();

	//clear_time();

  out_byte(0x20, 0x20);                // tell 8259 PIC EOI
}
