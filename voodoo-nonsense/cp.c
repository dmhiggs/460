//cp.c
//cp src dest		// src may be a file or DIR

/*
     NOTE: [filename] means OPTIONAL. 
           If no filenames, use stdin for IN and stdout for OUT
*/

//cp file1 file2 -- copy file1 to file2
//cp dir1 dir2 -- copy dir1 to dir2, create dir2 if doesn't exist
//cp file /home/dirname -- copy the filename called file to directory

#include "ucode.c"

int main(int argc, char *argv[])
{
	int f1, f2;
	char temp[128];

	//must have 3 arguments
	if (argc != 3)
	{
		printf("wrong amount of arguments\n");
		exit(0);
	}

	f1 = open(argv[1], O_RDONLY);
	f2 = open(argv[2], O_WRONLY);

	if (f1 >= 0 && f2 >= 0)
	{	
		close(f1);
		close(f2);
		strcpy(temp, "cat ");
		strcat(temp, argv[1]);
		strcat(temp, " > ");
		strcat(temp, argv[2]);
	}
	if (f1 >= 0 && f2 < 0)
	{	
		close(f1);
		close(f2);
		strcpy(temp, "cat ");
		strcat(temp, argv[1]);
		strcat(temp, " >> ");
		strcat(temp, argv[2]);
	}

	//check for directory or file

	//copy file1 to file2
	exec(temp);

	//copy file1 to directory
	//cat file1 > directory/file1

	//copy directory1 to directory2
	//ls into temp file
	//chdir(directory2)
	//mkdir(directory1)
	//chdir(directory1)
	//while loop through temp calling:
	//		cat current > directory2/directory1/current	

}