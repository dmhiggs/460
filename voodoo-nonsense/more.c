//more.c
//more [filename]	// more as in Unix
	//[filename] is OPTIONAL
	//if no filenames, use stdin for IN and stdout for OUT

//more file -- display file one page at time, spacebar for next page, enter for line

#include "ucode.c"

int main(int argc, char *argv[])
{
	int f1, f2, i = 0, j = 0;
	char buffer[1028];
	char c, *cp;

	if (argc == 1) //????? apply to previous pipe file?
	{

	}
	else if(argc == 2) //upper f1
	{
		if ((f1 = open(argv[1], O_RDONLY)) >= 0)
		{
			f2 = read(f1, buffer, 1028);

			while (f2 > j)
			{
				if (buffer[j] == 0) break;
				//c = getc();
				if ((c = getc()) != EOF)
				{
					if (c == ' ' || c == 13) //13 is enter key?, 32 is space
					{
						if (c == 13)
						{
							i = 24;
							while (i > 0)
							{
								while (buffer[j] != '\n' && buffer[j] != '\r' && f2 != j)
								{
									c = buffer[j];
									putc(c);
									j++;
									*cp++;
									//f2--;
									//getc();
								}
								if (f2 <= j) break;

								while (buffer[j] == '\n' || buffer[j] == '\r') 
								{
									if (buffer[j] == '\n')
									{
										putc('\n');
										i--;
									}
									//if (*cp == 0) break; 
									j++;
									*cp++;
								}
								//f2--;
							}
						} 
						else
						{
							while (buffer[j] != '\n' && buffer[j] != '\r' && f2 != j)
							{
								putc(buffer[j]);
								j++;
								*cp++;
								//getc();
								//f2--;
							}
							//j++;
							if (f2 <= j) break;
							while (buffer[j] == '\n' || buffer[j] == '\r')
								{
									if (buffer[j] == '\n')
									{
										putc('\n');
										i--;
									}
									//if (*cp == 0) break; 
									j++;
									*cp++;
								}
							//putc('\n');
						}
						//printf("f2 %d j %d\n", f2, j);
						//getc();
						if (f2 <= j) break;
					}
				}
				else
				{
					putc('\n');
					break;
				}
			}
			putc('\n');
		}
	}
}