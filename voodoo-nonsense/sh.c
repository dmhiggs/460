//sh.c
//Write YOUR OWN sh.c to support I/O redirections and pipes:
//
//     Examples:  cat [filename]        // NOTE: cat may NOT need a filenmae
//                cat [filename] >  newfile 
//                cat [filename] >> appendFile                           
//                a.out < inFile        // read inputs from inFile 
//                cat filename | more
//                cat filename | grep test
//                cat filename | l2u | grep PRINT
//
//                cat filename > /dev/lp0   // print filename
//                cp  filename /dev/lp0     // print filename

/*==========================================================
                   sh Program:
YOUR sh must support
(1). logout/exit :  terminate
		login
		logout
(2). simple command exceution, e.g. ls, cat, ....
		cat
		grep
		more
		cp
		l2u
(3). I/O redirection:  e.g. cat < infile;  cat > outfile, etc.
(4). (MULTIPLE) PIPEs: e.g. cat file | l2u | grep LINE 
===========================================================*/

//example of cmdLine = "cat < infile | lower2upper | grep print > outfile"
#include "ucode.c"



main(int argc, char *argv[])
{
	int pid, status;

	//uid, pid

	while(1)
	{
	//get a command line; eg cmdLine = cmd | cmd2 | ... | cmdn & 
	//get cmd token from command line

		if (cmd == cd || logout || su)	//built-in commands
		{
		//do the cmd directly;
			continue;
		}

	//for binary executable command
		pid = fork();		//for a child sh process
	
		if (pid)		//parent sh
		{
			if (no & symbol)//assume at most one & for main sh
				pid = wait(&status);
			continue;	//main sh does not wait if &
		}
		else			//child sh
			do_pipe(cmd_line,0);
	}
}


int do_pipe(char *cmdLine, int *pd)
{
	if (pd)		//if has a pipe passed in, as WRITER on pipe pd:
	{
		close(pd[0]);
		dup2(pd[1],1);
		close(pd[1]);
	}

	//divide cmdLine into head, tail by reightmost pipe symbol
	hasPipe = scan(cmdLine, head, tail);
	if (hasPipe)
	{
		//create a pipe lpd;

		pid = fork();
		if (pid)	//parent
		{
			//as READER on lpd;
				close(lpd[1]);
				dup2(lpd[0],0);
				close(lpd[0]);
			do_command(tail);
		}
		else
			do_pipe(head, lpd);
	}
	else
		do_command(cmdLine);
}


int do_command(char *cmdLine)
{
	//scand cmdLine for I/O redirection symbols;
	//do I/O redirections;
	//head = cmdLine BEFORE redirections
	//exec(head);
}


int scan(char *cmdLine, char *head, char *tail)
{
	//divide cmdLine into head and tail by rightmost | symbol
	//cmdLine = cmd1 | cmd2 | ... | cmdn-1 | cmdn
	//	|<------- head ------->| tail |; return 1;
	//cmdLine = cmd1 ==> head = cmd1, tail = null; return 0;
}
