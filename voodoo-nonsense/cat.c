//cat.c
//cat [filename]	//cat filename or stdin to stdout
	//[filename] is OPTIONAL
	//if no filenames, use stdin for IN and stdout for OUT

//cat
//cat > file
//cat file

//cat...
	//1. display contains of file (contains???)
		//# cat /etc/passwd
	//2. view contains of multiple files in terminal
		//test = hello
		//test1= hi
		//then
			//# cat test test1
			//hello
			//hi
	//3. create a file with cat command
		//# cat >test2


#include "ucode.c"


//argc = num args
//argv = probably files
int main(int argc, char *argv[])
{
	int f1, f2;
	int i = 0;
	char inbuf[1028], outbut[1028], temp[128];
	char *cp;
	//char *tty = gettty();

	//printf("%d %s %s\n", argc, argv[1], argv[2]);

	if (argc == 1) //no file argument, print to screen
	{
		//show input lines from stdin, ctrl-c to escape?
		//i don't see why i should write it anywhere except the screen
		while (1)
		{
			if ((i = getc()) != EOF)
			{
				if (i == '\r') i = '\n';
				putc(i);
			}
			else
			{
				break;
			}
		}
		putc('\n');
		return 0;
	}

	else if (argc == 2) //one file
	{
		//print file to screen
		if ((f1 = open(argv[1], O_RDONLY)) >= 0)
		{
			i = read(f1, inbuf, 1027);

			write(1, inbuf, i);
			putc('\n');

			return 0;
		}
		printf("file doesn't exist\n");
		return 1;
	}

	else if (argc == 3) //possibly 2 files or 1 file & operator
	{
		//2 files -- prints both to console
		if (  ((strcmp(argv[1], ">") != 0)  && (strcmp(argv[1], ">>") != 0) && (strcmp(argv[1], "<") != 0) ))
		{
			if ((f1 = open(argv[1], O_RDONLY)) >= 0)
			{
				i = read(f1, inbuf, 1027);

				write(1, inbuf, i);
				putc('\n');

				//close(f1);
				if ((f2 = open(argv[2], O_RDONLY)) >= 0)
				{
					i = read(f2, outbut, 1027);

					write(1, outbut, i);
					putc('\n');

					//close(f2);

					return 0;
				}
			}
			printf("files doesn't exist\n");
			return 1;
		}
		//operator & file -- >  puts input into file
		//				  -- <  uses file as input for command, print to screen
		//				  -- >> appends input into file
		else if (strcmp(argv[1], ">") == 0)
		{
			if ((f1 = open(argv[2], O_WRONLY)) >= 0)
			{
				while (1)
				{
					if ((i = getc()) != EOF)
					{
						if (i == '\r') i = '\n';
						outbut[0] = i;
						putc(i);
						write(f1, &outbut[0], 1);
					}
					else
					{
						break;
					}
				}
				putc('\n');

				return 0;			
			}

			printf("file did not open\n");
			return 1;
		}
		else if (strcmp(argv[1], ">>") == 0)
		{
			if ((f1 = open(argv[2], O_WRONLY)) >= 0)
			{
				while (1)
				{
					if ((i = getc()) != EOF)
					{
						if (i == '\r') i = '\n';
						outbut[0] = i;
						putc(i);
						write(f1, &outbut[0], 1);
					}
					else
					{
						break;
					}
				}
				putc('\n');

				return 0;			
			}
			else if ((f1 = open(argv[2], O_CREAT)) >= 0)
			{
				while (1)
				{
					if ((i = getc()) != EOF)
					{
						if (i == '\r') i = '\n';
						outbut[0] = i;
						putc(i);
						write(f1, &outbut[0], 1);
					}
					else
					{
						break;
					}
				}
				putc('\n');

				return 0;			
			}

			printf("file did not open\n");
			return 1;
		}
		else if (strcmp(argv[1], "<") == 0) //write to screen...
		{
			cp = inbuf;
			//print file to screen
			if ((f1 = open(argv[2], O_RDONLY)) >= 0)
			{
				i = read(f1, inbuf, 1027);

				write(1, inbuf, i);
				putc('\n');

				return 0;
			}
			printf("file doesn't exist\n");
			return 1;
		}
	}

	else if (argc == 4) //2 files and an operator
	{

		//f1 > f2 --- write f1 into f2, f2 overwritten
		//f1 >> f2 -- append f1 into f2
		//f1 < f2 --- write f2 into f1, overwrite

	}
}