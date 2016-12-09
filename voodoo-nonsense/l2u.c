//l2u.c
//l2u [f1 f2]		// convert lower case to upper case

#include "ucode.c"

int main(int argc, char *argv[])
{
	int f1, f2, temp;
	char buffer[1028], outbuf[1028]; //no idea how big to make it
	char c, *cp;

	cp = buffer;

	//signal(2,0);

	if (argc == 1) //type in screen, output to screen
	{
		printf("input a string....\n");

		//end at control-d
		while (getc() != EOF)
		{
			//write into buffer
			gets(cp);

			//upper buffer
			while (*cp != 0)
			{
				if (*cp >= 'a' && *cp <= 'z')
				{	
					*cp = *cp + 'A' - 'a';
				}
				if (*cp == '\r')
				{
					*cp = '\n';
					printf("%s", buffer);
					*cp++;
					*cp = '\r';
					printf("%s", buffer);
				}
				*cp++;
			}
			*cp-- = '\n';
			*cp++ = '\r';
			*cp++;
		}

		printf("\nUppercase string... \n%s\n", buffer);

		return 1;
	}

	if (argc == 2)
	{
		//open f1, open tempfile
		//while f1 good
			//read line into buffer
			//upper buffer
			//append tempfile
			//clear buffer
		//copy tempfile into f1
		//remove tempfile
		printf("%s %s: need 2 files\n", argv[0], argv[1]);
		return 1;
	}

	if(argc == 3) //f1
	{
		//open f1, open f2
		f1 = open(argv[1], O_RDONLY);
		f2 = open(argv[2], O_WRONLY);

		if (!f2)
		{
			f2 = open(argv[2], O_CREAT);
		}

		//while f1 good
		while(read(f1, buffer, 1027))
		{
			//read line into buffer
			//buffer = read(f1, )
			//upper buffer
			cp = buffer;
			while (*cp != 0)
			{
				if (*cp >= 'a' && *cp <= 'z')
				{	
					*cp = *cp + 'A' - 'a';
				}

				write(f2, cp, 1);
				*cp++;
			}
			write(f2, cp, 1);
			//write to f2
			//etc

			return 0;
		}
	}
}