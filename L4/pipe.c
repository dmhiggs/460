/* FUNCTIONS:
	show_pipe(PIPE *p)
	pfd()
	read_pipe(int fd, char *buf, int n)
	write_pipe(int fd, char *buf, int n)
	kpipe(int *y, int *z)
	close_pipe(int fd)
*/


char *MODE[ ]={"READ      ","WRITE     ","RDWR      ","APPEND    ",
	       "READ_PIPE ","WRITE_PIPE"};



//got help with this one from Brian Soto
show_pipe(PIPE *p)
{
   	int i; //counter variables
	int j;
   	
	printf("------------ PIPE STUFF ------------\n");     

	
	printf("nreader=%d  nwriter=%d  ", p->nreader, p->nwriter);
   	printf("data=%d room=%d\n",p->data, p->room);
   	prints("contents=");
   	if (p->data)
	{
      		j = p->tail;
      		for (i=0; i<p->data; i++)
		{
	          	putc(p->buf[j++]);
          		j %= PSIZE;
      		}
   	}
   	printf("\n----------------------------------------\n");
}

//????? pipe file descriptors...shows them...shows oft and mode?
int pfd()
{
 	int i=0;
	int j=0; 
	
	printf("...........file descriptors.........\n");

	//open file descriptors
 	for (i=0; i<NFD; i++)
	{
     		if (running->fd[i]) 
		{
			j++;
			if (running->fd[i]!=0)
			{
				printf("OFT:%d    MODE:%s\n",i, MODE[running->fd[i]->mode]);
				
			}
		}
 	}
	printf(".....................................\n");
	
 	if (j==0)
	{
    		printf("no opened files\n");
    		return(-1);
 	}
}


//pg 201
int read_pipe(int fd, char *buf, int n)
{
   	int r = 0;
   	OFT *op;  
	PIPE *p;
   	char c; 

   	if (n<=0) 
	{
		return 0;
	}

	//validate fd; from fd, get oft and pipe pointer p;
	//fd less than 0 in array doesn't exist, can't have more fds than number of fds, if arrary at fd is 0
   	if (fd < 0 || fd >= NFD || running->fd[fd] == 0)
	{
      		printf("fd %d is no good\n", fd);
      		return -1;
   	}
   	op = running->fd[fd];

	//get pipe pointer
	//as long as op isn't null and it's mode isn't read_pipe then it's not for reading and it should return
   	if (op->mode != READ_PIPE)
	{
	      	printf("fd %d is not for reading\n", fd);
      		return -1;
   	}
	//set pointer
   	p  = op->pipe_ptr;

	//i guess you want to see the pipe to make sure it's right?
	show_pipe(p);
 

	//while n (given...nbytes?) while data (pipe?)	
   	while(n)
	{
      		while(p->data)
		{
			//read byte from pipe to buf

			//get byte from pipe
         		c = p->buf[p->tail++];
			//put into buff
         		put_byte(c, running->uss, buf);
         		//tail = tail % 10...
			p->tail %= PSIZE;
         
			n--;
			r++;
			p->data--;
			p->room++;

         		if (n == 0) 
				break;
      		}
		//if(r) ----- has read some data
      		if (r)
		{
 	 		kwakeup(&p->room);
         		printf("show read pipe\n");
         		show_pipe(p);
         		return r;
      		}

      		// pipe has no data
		//if pipe still has a writer
      		if (p->nwriter)
		{                 
	 		printf("pipe has no data, show pipe\n");
         		show_pipe(p);
			//wakeup writers
         		kwakeup(&p->room);
         		//sleep for data
			ksleep(&p->data);
         		continue;
       		}
       // pipe has no writer and no data
       return 0;
   }
}



//page 201
int write_pipe(int fd, char *buf, int n)
{
	int r=0;
	OFT *op;
  	PIPE *p;  
  	char c;   

	if (n<=0)
	{
		return 0;
	}

	//validate fd, from fd, get oft and pipe pinter p
	//fd less than 0 in array doesn't exist, can't have more fds than number of fds, if arrary at fd is 0
   	if (fd < 0 || fd >= NFD || running->fd[fd] == 0)
	{
      		printf("fd %d is no good\n", fd);
      		return -1;
   	}
   	op = running->fd[fd];

	//get pipe pointer
	//as long as op isn't null and it's mode isn't write_pipe then it's not for reading and it should return
   	if (op->mode != WRITE_PIPE)
	{
	      	printf("fd %d is not for writing\n", fd);
      		return -1;
   	}
	//set pointer
   	p  = op->pipe_ptr;

	//i guess you want to see the pipe to make sure it's right?
	show_pipe(p);

  	while(n)
	{
		//no more readers
    		if (!p->nreader)
		{
			printf("proc %d BROKEN_PIPE\n", running->pid);
			//broken pipe error
       			kexit(0x0D); //simulate sigpipe=13
     		}
		
		//while there's still room and there's bytes to write
		while(p->room)
		{
			//write byte from but to pipe
	        	p->buf[p->head++] = get_byte(running->uss, buf); 
			//adjust head size...
        		p->head %= PSIZE;

			r++;
			p->data++;
			p->room--;
			n--;

			//no more bytes to write
			if (n==0)
			{
				break;
			}
     		}
		//wake up readers
     		kwakeup(&p->data);          
		
		//no more bytes to write
		if (n==0)
		{
        		printf("pipe after finished writing\n");
        		show_pipe(p);
			//finished writing n bytes
        		return r;
     		}
     		// still has data to write but pipe has no room
     		printf("no room can't write, sleep the pipe\n");
     		show_pipe(p);
		//sleep for room
     		ksleep(&p->room);
	}


	return r;
}


//pg200
//closes pipe based on fd index and process
int close_pipe(int fd)
{
	OFT *op;
	PIPE *p;

	
  	printf("proc %d close pipe fd=%d\n", running->pid, fd);

	//validate fd
	//fd less than 0 in array doesn't exist, can't have more fds than number of fds, if arrary at fd is 0
   	if (fd < 0 || fd >= NFD || running->fd[fd] == 0)
	{
      		printf("fd %d is no good\n", fd);
      		return -1;
   	}

	//follow proc.fd[fd] to oft. decrement oft.refcount--
  	op = running->fd[fd]; //procfdfd to oft
  	op->refCount--;

	p = op->pipe_ptr;
	//if write pipe, nwriter--
	if(op->mode == WRITE_PIPE)
	{
		p->nwriter--;
		
		//last writer on pipe
		//if nwriter =0, if nreader =0, deallocate pipe, deallocate writeoff
		if (p->nwriter ==0 && p->nreader==0)
		{
			p->busy = 0;
			op->pipe_ptr = 0;
		}	

		//wakeup data -- wake up readers blocked
		kwakeup(&p->data);
	}


	//else read pipe, nreader--
  	else if (op->mode == READ_PIPE)
	{
	      	p->nreader--;

		//last reader on pipe
		//if nreader=0,ifnwriter =0, deallocate pipe, deallocate readoft
		if (p->nreader==0 && p->nwriter ==0)
		{
			p->busy = 0;
			op->pipe_ptr=0;
		}
      	
		//wakeuproom --- wakeup blockedwrites
		kwakeup(&p->room);
	}
      
	//clear caller's fd[fd] to 0, return OK 
	running->fd[fd]=0; //???      	

	return 1;
}




//page 199
//pipe creation
//int kpipe(int pd[2]) ???
int kpipe(int *b)
{
	//allocate pipe object
  	PIPE *p; 
	OFT *op1, *op2; 
	int i,j;
	int pd[2];
  

	//add file descriptor array, fd[nfd], to every proc structure
	//oft *fd[nfd];
	//int kstack[ssize];
	
	//allocate pipe object, initialize pipe object with head=tail=0; data=0; room=psize; nreaders=nwriters=1

	//allocate pipe
	for (i=0; i<NPIPE; i++)
	{
      		if (pipe[i].busy == 0)
		{
			p=&pipe[i];
			pipe[i].busy=1;

		  	p->head = 0;
			p->tail = 0;
			p->data = 0;
  			p->room = PSIZE;
			p->nreader = 1;
			p->nwriter = 1;

          		break;
		}  	
	}
	if (i>= NPIPE)
	{
		printf("no free pipes\n");
		return -1;
	}


	//allocate 2 ofts, initialize ofts as refcount =1 and pipeptr to same pipe object
	j=0;
	i=0;
	while(j<2)
	{
	  	for (i; i<NOFT; i++)
		{
		      	if (oft[i].refCount == 0)
			{	
				if (j==0)
				{
					op1= &oft[i];
					op1->refCount = 1;
					op1->mode = READ_PIPE;
					op1->pipe_ptr = p;
					j++;
				}
				else
				{
					op2= &oft[i];
					op2->refCount = 1;
					op2->mode = WRITE_PIPE;
					op2->pipe_ptr = p;
					j++;
					break;
				}
			}
  		}
		break;		
	}
	if (j<2)
	{
		p->busy=0;
		printf("not enough ofts\n");
		return -1;
	}

	//allocate 2 free entries in proc.fd[] array, eg fd[i] fd[j]
	//let fd[i] pint to readoft and jd[j] point to write oft
	j=0;
	
  	for (i=0; i<NFD && j<2; i++)
	{ 
      		if (running->fd[i] == 0)
		{
			if (j==0)
			{
				running->fd[i] = op1; //reader
				pd[0] = i;
				j++;
			}
			else
			{
				running->fd[i] = op2; //writer
				pd[1] = i;
				j++;
				break;
			}
      		}
  	}

	if(j<2)
	{
		p->busy=0;
		printf("not enough fds\n");
		return -1;
	}


	//write index i to pd0 and j to pd1, both in uspace
  	put_word(pd[0], running->uss, b); 
	put_word(pd[1], running->uss, b+1);
  	printf("do_pipe : file descriptors = [%d %d]\n", pd[0], pd[1]); 
	//retrun 0 for ok
  	return 0;
}




