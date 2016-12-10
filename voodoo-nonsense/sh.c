/*
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
*/

/*==========================================================
                   sh Program:
YOUR sh must support
	(1). logout/exit :  terminate
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

/*//example of cmdLine = "cat < infile | lower2upper | grep print > outfile"*/
/*
			 cat   [filename]         // cat filename or stdin to stdout
             more  [filename]         // more as in Unix                
             grep  pattern [filename] // print lines containing pattern 
             l2u   [f1 f2]            // convert lower case to upper case
             cp    src dest           // src may be a file or DIR

		     NOTE: [filename] means OPTIONAL. 
    		       If no filenames, use stdin for IN and stdout for OUT
*/
/*
//cmds
	//cat > file
	//cat file
	//more file -- display file one page at time, spacebar for next page, enter for line
	//l2u 
	//grep pattern files -- search for pattern in files
	//cp file1 file2 -- copy file1 to file2
	//cp -r dir1 dir2 -- copy dir1 to dir2, create dir2 if doesn't exist
	//cp file /home/dirname -- copy the filename called file to directory
*/

#include "ucode.c"

int aops[16], aaops[16], laops[16], pipeops[16];
int index = 0, ai = 0, aai = 0, lai= 0, pipeindex = 0;

char *tokens[16], *files[16];
char cmd_line[128];
char *tty;
int pid, status, uid;
int hasPipe;
//char *head, *tail;
int lpd[2];


int cmdmenu()
{

   	printf("====================== MTX System Call Functions ======================\n");
    printf("Number  Name          Usage                         Function  \n");
   	printf("-----------------------------------------------------------------------\n");
/*
    printf(" 0    getpid       getpid()                    get process pid  \n");
    printf(" 1    getppid      getppid()                   get parent pid\n");
    printf(" 2    getpri       getpri()                    get priority\n");
    printf(" 3    setpri       setpri(pri)                 set priority\n");
    printf(" 4    getuid       getuid()                    get uid\n");
    printf(" 5    chuid        chuid(uid,gid)              set uid,gid\n");
    printf(" 6    yield        yield()                     switch process\n");
    printf(" 9    exit         exit(value)                 terminate process\n");
    printf("10    fork         fork()                      fork child process\n");
    printf("11    exec         exec(cmd_line)              change image to a file\n");
    printf("12    wait         wait(&status)               wait for child to die\n");
    printf("13    vfork        vfork()                     fork child process\n");
    printf("14    thread       thread(fd,stack,flag,prt)   create thread\n");
    printf("15    mutex_creat   mutex_creat()\n");
    printf("16    mutex_lock    mutex_lock(&mutex)\n");
    printf("17    mutex_unlock  mutex_unlock(&mutex)\n");
    printf("18    mutex_destroy mutex_destroy(&mutex)\n");
   	printf("-----------------------------------------------------------------------\n");
*/
/*
    printf("20    mkdir        mkdir(pathname)             make directory\n");
    printf("21    rmdir        rmdir(pathname)             rm   directory\n");
    printf("22    creat        creat(pathname)             creat file\n");
    printf("23    link         link(oldname, newname)      hard link to file\n");
    printf("24    unlink       unlink(pathname)            unlink\n");
    printf("25    symlink      symlink(oldname,newname)    create symbolic link\n");
    printf("26    readlink     readlink(name, buf[ ])      read symlink \n");
    printf("27    chdir        chdir(pathname)             change dir\n");
    printf("28    getcwd       getcwd(buf[ ])              get cwd pathname\n");
    printf("29    stat         stat(filename, &stat_buf)    stat file\n");
    printf("30    fstat        fstat(fd, &stat_buf)        stat file descriptor\n");
    printf("31    open         open(filename, flag)        open file for R|W|APP\n");
    printf("32    close        close(fd)                   close file descriptor \n");
   	printf("33    lseek        lseek(fd, position)         lseek \n");
    printf("34    read         read(fd, buf[ ], nbytes)    read file\n");
    printf("35    write        write(fd,buf[ ], nbytes)    write to file\n");
    printf("36    pipe         pipe(pd[ ])                 carete pipe\n");
    printf("37    chmod        chmod(filename, mode)       change permission\n");
    printf("38    chown        chown(filname, uid)         change file owner\n");
    printf("39    touch        touch(filename)             change file time\n");
*/
/*
    printf("40    settty       settty(tty_name)            set proc.tty name\n");
    printf("41    gettty       gettty(buf[ ])              get proc.tty name\n");
*/
    printf("42    dup          dup(fd)                     dup file descriptor\n");
    printf("43    dup2         dup2(fd1, fd2)              dup fd1 to fd2\n");
    printf("44    ps           ps()                        ps in kernel\n");
    printf("45    mount        mount(FS, mountPoint)       mount file system\n");
    printf("46    umount       umount(mountPoint)          umount file system\n");
    printf("47    getSector    getSector(sector, buf[ ])   read CDROM sector\n");
    printf("48    cd_cmd       cd_cmd(cmd)                 issue cmd to CD driver\n");
    printf("-----------------------------------------------  ----------------------\n");
    printf("50    kill         kill(sig#, pid)             send signal to pid\n");
    printf("51    signal       signal(sig#, catcher)       install siganl handler\n");
    printf("52    pause        pause(t)                    pause for t seconds\n");
    printf("53    itimer       itimer(sec, action)         set timer request\n");
    printf("54    send         send(msg, pid)              send msg to pid\n");
    printf("55    recv         sender=recv(msg)            receive msg \n");
    printf("56    tjoin        tjoin(n)                    threads join\n");
    printf("57    texit        texit(value)                tthread exit\n");
    printf("58    hits         hits()                      I/O buffer hit ratio\n");
    printf("59    color        color(v)                    change display color\n");
    printf("60    sync         sync()                      sync file system\n");	
	printf("-----------------------------------------------------------------------\n");
	printf("mkdir  rmdir  creat  link  unlink  symlink  readlink  stat  fstat  open\n");
	printf("close  chmod  chown  read   touch   write  pipe   ls  cd    pwd    ps\n");
	printf("               cat  more  l2u  grep  cp  logout  help\n");
	printf("=======================================================================\n\n");	
}

int logout()
{
	exit(0);
}

//make tokens
int tokenize(char *line)
{
	char *cp2;
	int i = 0;

	cp2 = line;
	index = 0; ai = -1; aai = -1; lai= -1; pipeindex = 0;
	aops[0] = -1; aaops[0] = -1; laops[0] = -1; pipeops[0] = -1;

	while (*cp2 != 0 && *cp2 != '\0') //while line is good
	{
		while (*cp2 != ' ' && *cp2 != '\0') //not space and not end
		{
			*cp2++; //go to next letter
		}

		if (*cp2 == ' ' || *cp2 == '\0') //if it is a space or the end
		{
			*cp2 = 0; //make that place 0
			tokens[i] = line; //put the line start into tokens
			*cp2++; //go to next letter/start of next token
			line = cp2; //make the line start there too

			//cmd, file/dir, <, >, >>, |

			//save indexes of operators in tokens
			if (strcmp(tokens[i], "<") == 0) //backwards alligator
			{
				lai++;
				laops[lai] = i;
			}
			if (strcmp(tokens[i], ">") == 0) //alligator
			{
				ai++;
				aops[ai] = i;
			}
			if (strcmp(tokens[i], ">>") == 0) //double alligators
			{
				aai++;
				aaops[aai] = i;
			}
			if (strcmp(tokens[i], "|") == 0) //it's a pipe
			{
				pipeops[pipeindex] = i;
				pipeindex++;
			}



			i++; //increment index of tokens
		}

		while (*cp2 == ' ') //skip through multiple spaces
		{
			*cp2++;
		}
	}
}

int main(int argc, char *argv[])
{
	char *head, *tail;
	char temp[128];

	//printf("log out");
	//getc();
	//logout();

	//uid, pid, tty
	uid = getuid();
	pid = getpid();
	gettty(tty); //stdin, stdout...

	signal(2,1);


	while(1)
	{
		//cmdmenu(); //too big to be useful...
		//get a command line; eg cmdLine = cmd | cmd2 | ... | cmdn & 
		*cmd_line = 0; /*doing this so won't get an error when checking if nothing entered for string*/
		printf("enter a command: ");
		gets(cmd_line);

		//doesn't account for white spaces at the end of cmdline but i already wrote it here
		if (strcmp(cmd_line, "logout") == 0) logout();
		if (strcmp(cmd_line, "help") == 0) cmdmenu();
		if (strcmp(cmd_line, "") == 0) continue;
		if (strcmp(cmd_line, "pwd") == 0) pwd();
		else
		{
			strcpy(temp, cmd_line);
			tokenize(cmd_line);
			strcpy(cmd_line, temp);

			if (strcmp(tokens[0], "cd") == 0) 
			{
				if (tokens[1] == 0) chdir("/");
				else chdir(tokens[1]);
				continue;
			}
			else
			{
				//exec("pwd /dev/tty0"); //doesn't work
				//exec("creat hello");


				//PAGE 365
				//for binary executable command
				pid = fork();		//for a child sh process
	
				if (pid)		//parent sh
				{
					//if (no & status) ???????????
					pid = wait(&status);
				}
				else 
				{
					do_pipe(cmd_line,0);
					continue;
				}
			}
		}
	}
}


int do_pipe(char *cmdLine, int *pd)
{
	char *head = 0, *tail = 0;

	if (pd)		//if has a pipe passed in, as WRITER on pipe pd:
	{
		close(pd[0]);
		dup2(pd[1],1);
		close(pd[1]);
	}

	//divide cmdLine into head, tail by reightmost pipe symbol
	hasPipe = scan(cmdLine, &head, &tail);
	if (hasPipe) //cmdLine = head...tail =
	{
		//create a pipe lpd;

		pid = fork();
		if (pid)	//parent as reader
		{
			//as READER on lpd; left pipe descriptor
			close(lpd[1]);
			dup2(lpd[0],0);
			close(lpd[0]);
			do_command(tail);
		}
		else
		{
			index = 0;
			do_pipe(head, lpd); //left pipe is right pipe...?
		}
	}
	else
	{
		index = 0;
		do_command(cmdLine); //no pipe is left
	}
}


int do_command(char *cmdLine)
{
	char line[128];
	int i = 1, j = 0;

	//printf("tokens index = %s\n", tokens[index]);
	strcpy(line, tokens[index]);
	while (tokens[index + i] != 0)
	{
		if (*tokens[index + i] == '<' || *tokens[index + i] == '>')
		{
			j++;
		}

		i++;
	}

	i = -1;

//
	//scand cmdLine for I/O redirection symbols;
	//scan(cmdLine, &head, &tail);

	//do I/O redirections;
		//first can be cmd or file/dir
		//| needs commands on either side


	if (lai - j >= index) i = laops[lai-j];
	//< needs command to left, needs file to right
	//	gives input to a command
	if (i > index) //< exists and isn't first after start or |
	{
		lai--;
		//redirect input from next index value in tokens
		if (tokens[i + 1] == 0)
		{
			printf("invalid command...< has no file to right\n");
			exit(1);
		}
		close(0); //close the reader...
		//check if the file exists and open it...to read?
		if (open(tokens[i + 1], 0) < 0) //think this means invalid
		{
			printf("invalid file...doesn't exist...\n");
			exit(1);
		}
	}
	else if (i == index) //< exists but is first so is invalid
	{
		lai--;
		printf("invalid command... < has no left side\n");
		exit(1);
	}

	i = -1;
	if (ai - j >= index) i = aops[ai-j];
	//> needs file to the right, command or file to left
	//	directs the output of a command into a file
	if (i > index) //< exists and isn't first
	{
		ai--;
		if (tokens[i + 1] == 0)
		{
			printf("invalid command...> has no file to write to\n");
			exit(1);
		}
		close(1); //close writer....
		//write or create...
		if (open(tokens[i + 1], O_WRONLY|O_CREAT) >= 0) //file was able to be created...
		{
			close(1);
			if (open(tokens[i + 1], O_WRONLY) < 0) //can't write for some reason
			{
				printf("> didn't write to new file\n");
				exit(1);
			}
		}
		//close(1);
		else if (open(tokens[i + 1], O_WRONLY) < 0) //file didn't write
		{
				printf("failed to write to file\n");
				exit(1);
		}
	}
	else if (i == index) //< exists but is first so is invalid
	{
		ai--;
		printf("invalid command... > has no left\n");
		exit(1);
	}

	i = -1;
	if (aai - j >= 0) i = aaops[aai - j];
	//>> needs file to the right, command or file to left
	if (i > index) //>> exists and isn't first
	{
		aai--;
		if (tokens[i + 1] == 0)
		{
			printf("invalid command...>> has no file to write to\n");
			exit(1);
		}
		close(1);
		//write, append, or create....apparently defined in type.h...
		//O_WRONLY
		//O_APPEND
		//O_CREAT
		if (open(tokens[i + 1], O_WRONLY|O_CREAT) >= 0) //was able to make file...
		{
			close(1);
			if (open(tokens[i + 1],O_WRONLY) < 0)//but couldn't write to it
			{
				printf("couldn't write to new file\n");
				exit(1);
			}
		}
		else if (open(tokens[i + 1], O_WRONLY|O_APPEND) < 0)
		{
			printf("couldn't append to file\n");
			exit(1);
		}
	}
	else if (i == 0) //> exists but is first so is invalid
	{
		aai--;
		printf("invalid command... >> has no left\n");
		exit(1);
	}
//*/

	//head = cmdLine BEFORE redirections
	//exec(head);
	//printf("exec(%s):\n", line);
	exec(line);
}


int scan(char *cmdLine, char *head, char *tail)
{
	char line[128];
	int i = 0;

	if (pipeindex <= 0)
	{
		//printf("tokens index = %s\n", tokens[index]);
		strcpy(line, tokens[index]);
		//i++;
		//while (tokens[index + i] != 0)
		//{
		//	strcat(line, " ");
		//	strcat(line, tokens[index + i]);
		//	i++;
		//}

		strcpy(cmdLine, line);
		return 0; //no pipes
	}

	//divide cmdLine into head and tail by rightmost | symbol
	//cmdLine = cmd1 | cmd2 | ... | cmdn-1 | cmdn

	//make head
	for (i = 0; i < pipeops[pipeindex-1]; i++)
	{
		strcat(head, tokens[i]);
	}

	//make tail
	i = pipeops[pipeindex-1] + 1;
	index = i;
	tokens[pipeops[pipeindex-1]] = 0;
	pipeindex--;
	if (tokens[i] == 0) return 0; //nothing after last pipe

	while (tokens[i] != 0)
	{
		strcat(tail, tokens[i]);
		//tokens[i] = 0;
		i++;
	}

	//	|<------- head ------->| tail |; return 1;
	//cmdLine = cmd1 ==> head = cmd1, tail = null; return 0;

	return 1; //has pipe
}
