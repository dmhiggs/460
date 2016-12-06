//login.c : upon entry, argv[0] = login, argv[1] = /dev/ttyX
//login		// for login processes

#include "ucode.c"
int in, out, err, fd, r;
char *tty, *ttyS0, *ttyS1;

char buf[1024], user[64], password[64];
char line[64], *lines[8];

main(int argc, char * argv[])
{
	int i = 0, j = 0, n = 0, m = 0, uid, gid;
	char *cp;

	tty = argv[1];
	ttyS0 = "/dev/ttyS0";
	ttyS1 = "/dev/ttyS1";

	//1. close file descriptors 0,1 inherited from INIT
	close(0);
	close(1);
	close(2);

	//2. open argv[1] 3 times as in(0), out(1), err(2)
	in = open(argv[1], O_RDONLY);
	out = open(argv[1], O_WRONLY);
	err = open(argv[1], O_WRONLY);

	//3. settty(argv[1]); //set tty name string in PROC.tty
	settty(tty); //store tty string in PROC.tty[] for putf()

	//NOW can use printf, which calls putc() to tty
	printf("DARALOGIN : open %s as stdin, stdout, stderr\n", tty);

	signal(2,1);	//ignore Control-C interruts so that
			//Control-C KILLS other procs on this tty but not the main sh


	while(1)
	{
		printf("~~~~~~~~~~~~~~~~~~~~~~:C~~~~~~~~~~~~~~~~~~~~~~\n");
		printf("LOGIN: ");
		gets(user);
		printf("PASSWORD: ");
		gets(password);
		printf("~~~~~~~~~~~~~~~~~~~~~~:C~~~~~~~~~~~~~~~~~~~~~~\n");

		//4. open /etc/passwd file for READ;
		fd = open("/etc/passwd", O_RDONLY);

		//check if file good???
		if (fd < 0) //open failed
		{
			printf("no password file... :C\n");
			exit(1);
		}

		printf("opened password file\n");

		//read file...into a buffer...
		r = read(fd, buf, 1024); //fd, buf, blksize...should buf be same size as blksize???
		buf[r] = 0;

		printf("read password file\n");
		cp = buf; //char point at start of buf

		//   for each line in passwd file do{
		//   	tokenize account line;

		//separate into lines
			// '\n'
		//separate lines into 0user,1password, 2gid, 3uid, 4name?, 5name?, 6folder?, 7file?
			// ':', ' ',


		//get line...get parts...compare...repeat
		line[0] = 0;
		i = 0;
		//go through each char in buf to find '\n' and separate
		while (i < r) //r is num chars...
		{
			j = 0;
			//if the char is not a '\n', increment
			while (buf[i] != '\n')
			{
				line[j] = buf[i];
//cp = &buf[i];
//printf("%s\n", cp);
				j++;
				i++; //increment to find end of line...
			}
			//found end of line...make the place null
			line[j] = 0;

			//go through line and separate by ':' and ' '
			n = 0; //index of line
			m = 0; //index of lines
			cp = &line[n]; //point to start of word in line

//			printf("%s\n", cp);
//getc();

			while(n < j) //start of line to end of line
			{
				if (line[n] == ' ') line[n] = '-';
				if (line[n] == ':')
				{
					//make end null
					line[n] = 0;
					
					//add word to lines
					lines[m] = cp;
					//increment to next index in lines
					m++;
					//increment n to next start of word
					n++;
					cp = &line[n];
				}
				else n++;
			}

			printf("user %s %s psswd %s %s\n", user, lines[0], password, lines[1]);
//getc();

			//if user/password and first two in line same
			if (strcmp(user,lines[0]) == 0 && strcmp(password, lines[1]) == 0)
			{
				//then correct username and password
				//print shit
				printf("user = %s\n", user);
				printf("home = %s\n", lines[5]);
				//change uid, gid to user's -- chuid()
				uid = atoi(lines[3]);
				gid = atoi(lines[2]);
				chuid(uid, 0);
				//change cwd to user's home -- chdir()
				chdir(lines[5], 0, 0);
				close(fd);
printf("worked\n");
getc();
				exec("sh");
			}

			//not correct user/pass so check against next line

			//increment end of line to start of next
			i ++;//= j; //i = start of line
		}

		printf("login failed, try again \n");
		close(fd);//close file
	}
}
