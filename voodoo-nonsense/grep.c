//grep.c
//grep pattern [filename]	//print lines containing pattern
	//[filename] is OPTIONAL
	//if no filenames, use stdin for IN and stdout for OUT

//grep pattern files -- search for pattern in files

//grep pattern
//grep pattern f1
//grep pattern f1 f2
//grep pattern > f1
//grep pattern >> f1
//grep pattern f1 > f2
//grep pattern f1 >> f2

#include "ucode.c"
char *tty;

int main(int argc, char *argv[])
{
	int f1, f2, n = 0, i = 0, j = 0, k = 0;
	char pattern[64], cmppat[64], inbuf[1028], outbuf[1028];
	char c, *cp;

	gettty(tty);
	strcpy(pattern, argv[1]);
	//strcpy(cmppat, "");
	cp = inbuf;
	//printf("%s\n", cmppat);

	if(argc == 2) //find pattern and prints lines in previous pipe file?
	{

	}
	if (argc == 3) //one file...print to screen
	{
		f1 = open(argv[2], O_RDONLY);
		n = read(f1, inbuf, 1027);

		//printf("grepping...%s...\n", pattern);

		while (i < n)
		{
			outbuf[j] = inbuf[i];
			//printf("%d", j);

			if (outbuf[j] == pattern[0])
			{
				k = 0;
				while (outbuf[j] == pattern[k])
				{
					//printf("%d", k);
					cmppat[k] = inbuf[i];
					j++;
					i++;
					k++;
					outbuf[j] = inbuf[i];
				}

				cmppat[k] = 0;
				//printf("%s", cmppat);
				if (strcmp(cmppat, pattern) == 0)
				{
					while (inbuf[i] != 0 && inbuf[i] != '\r' && inbuf[i] != '\n')
					{
						outbuf[j] = inbuf[i];
						j++;
						i++;
					}
					printf("%s\n", outbuf);
				}
			}
			else if (outbuf[j] != '\r' && outbuf[j] != '\n')
			{
				i++;
				j++;
				continue;
			}


			for (k = 0; k < j; k++) outbuf[k] = 0;
			i++;
			j=0;
//			printf("i %d n%d\n", i, n);
		}
		//printf("what is going on");
		exit(0);
		//return 0;
	}
	if (argc == 4)
	{

	}
	if (argc == 5) //find pattern in file...prints lines with it
	{

	}
}