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
	//must have 3 arguments
	if (argc != 3)
	{
		printf("wrong amount of arguments\n");
		return 1;
	}

	strcpy(argv[0], "cat ");
	strcat(argv[0], argv[1]);
	strcat(argv[0], " ");
	strcat(argv[0], argv[2]);

	//check for directory or file

	//copy file1 to file2
	exec(argv[0]);

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