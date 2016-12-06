/* init.c file */
//init		//for process P1


//*************************************************************************
//                      Logic of init.c 
// NOTE: this init.c creates only ONE login process on console=/dev/tty0
// YOUR init.c must also create login processes on serial ports /dev/ttyS0
// and /dev/ttyS1.. 
//************************************************************************

int pid, status;
int in, out;	//might need to rename these

#include "ucode.c"
int console;

//from pg 362
main(int argc, char *argv[])
{
	//int in, out;

	//1. open dev/tty0 as 0 (READ) and 1 (WRITE) to display messages
	in = open("/dev/tty0", O_RDONLY);
	out = open("/dev/tty0", O_WRONLY);
	
	//2. now can use printf, which calls putc, which writes to stdout
	printf("DARAINIT : fork a login proc on console\n");
	console = fork(); //460 page has child=fork() instead

	if (console) parent(); //460 page has child instead console
	//else exec("login /dev/tty0");
	//460 page has 
	else login(); //login task

	return 0;
}

//login task
int login()
{
	exec("login /dev/tty0");
}

int parent()
{
	//int pid, status;

	while (1)
	{
		printf("DARAINIT : wait for ZOMBIE child\n");
		pid = wait(&status);

		if (pid == console)
		{
			printf("DARAINIT : forks a new console login\n");
			console = fork();

			if (console) continue;
			else exec("login /dev/tty0");
		}
		else
			printf("DARAINIT : I just buried an orphan child proc %d\n", pid);
getc();
	}

}
