//Nikita Zala and Paris Downing

//isCSV and getColumn both work. The code is breaking after the check for -c -o -d. it is time to sort every file in the directory
//after this, you should probably change the output.csv. it should be sorted something or other. change outputDir
#include "scannerCSVsorter.h"
#include "mergesort.c"
#include <dirent.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

//stores the header of the document
char **array_tokens;

//stores the number of attributes in one line
int attributes;

//stores the number of lines the document has
int f_size;

//tells which col in the doc will be sorted
int sortCol = -1;

//holds all the pid values
int *pidHolder;

int pidCounter;

//Function declaration
void trimTrail(char * str);
struct movies* getData(FILE *file, char *outputStr, char *sortBy);
int getSortBy(struct movies* data, char* sortBy);
void printOutput(struct movies *mo, char *outputDir, char *sortBy);
int getF_size(FILE *file);
int getCurrDirectory(char *directory, char *outputDir, char *sortBy);
int isCSV(char *file);
int getColumn(char *file);

//receives a sortBy word, the starting dir, and the dir where all the final entries will be stored.
int main(int argc, const char * argv[]) 
{
	//TESTING CODE
	FILE *file = fopen("movie_metadata.csv","r+");
	getF_size(file);
	printf("attributes: %d\n", attributes);
	printf("f_size: %d\n", f_size);
	




	int j;

	//figures out what to do with the user's inputs. If 0, put file in curr directory
	char *outputDir;
	char *directory;
	//the location in the input of the string that the documents will be sorted by
	int cLoc;

	if(argc == 3)
	{
		if(strcmp(argv[1], "-c") != 0)
		{
			printf("-c necessary to sort\n");
			return -1;
		}
		else
		{
			cLoc = 2;
			outputDir = ".";
			directory = ".";
		}
	}
	else if(argc == 5)
	{
		if((strcmp(argv[1], "-c") != 0) && (strcmp(argv[3], "-c") != 0))
		{
			printf("-c necessary to sort\n");
			return -1;
		}
		else
		{
			if(strcmp(argv[1], "-c") == 0)
			{
				cLoc = 2;

				if(strcmp(argv[3], "-d") == 0)
				{
					outputDir = ".";
					directory = argv[4];
				}
				else if(strcmp(argv[3], "-o") == 0)
				{
					outputDir = argv[4];
					directory = ".";
				}
			}
			else if(strcmp(argv[3], "-c") == 0)
			{
				cLoc = 4;

				if(strcmp(argv[1], "-o") == 0)
				{
					outputDir = argv[2];
					directory = ".";
				}
				else if(strcmp(argv[1], "-d") == 0)
				{
					outputDir = ".";
					directory = argv[2];
				}
			}
		}
	}
	else if(argc == 7)
	{
		if((strcmp(argv[1], "-c") != 0) && (strcmp(argv[3], "-c") != 0) && (strcmp(argv[5], "-c") != 0))
		{
			printf("-c necessary to sort\n");
			return -1;
		}
		else
		{
			if(strcmp(argv[1], "-c") == 0)
			{
				cLoc = 2;

				if((strcmp(argv[3], "-d") == 0) && (strcmp(argv[5], "-o") == 0))
				{
					directory = argv[4];
					outputDir = argv[6];
				}
				else if((strcmp(argv[3], "-o") == 0) && (strcmp(argv[5], "-d") == 0))
				{
					outputDir = argv[4];
					directory = argv[6];
				}
			}
			else if(strcmp(argv[3], "-c" == 0))
			{
				cLoc = 4;

				if((strcmp(argv[1], "-d") == 0) && (strcmp(argv[5], "-o") == 0))
				{
					outputDir = argv[6];
					directory = argv[2];
				}
				else if((strcmp(argv[1], "-o") == 0) && (strcmp(argv[5], "-d") == 0))
				{
					outputDir = argv[2];
					directory = argv[6];
				}
			}
			else if(strcmp(argv[5], "-c" == 0))
			{
				cLoc = 6;

				if((strcmp(argv[1], "-d") == 0) && (strcmp(argv[3], "-o") == 0))
				{
					outputDir = argv[4];
					directory = argv[2];
				}
				else if((strcmp(argv[1], "-o") == 0) && (strcmp(argv[3], "-d") == 0))
				{
					outputDir = argv[2];
					directory = argv[4];
				}
			}
		}
	}
	else
	{
		printf("incorrect amount of arguments given\n");
		return -1;
	}
	if(strcmp(outputDir, ".") == 0)
	{
		char cwd[1024];
		chdir("/path/to/change/directory/to");
                getcwd(cwd, sizeof(cwd));
                printf("Current working dir: %s\n", cwd);
                outputDir = malloc(sizeof(char) * sizeof(cwd));
                outputDir = cwd;
	}
	printf("cLoc: %d\n", cLoc);
	printf("inDir: %s\n", directory);
	printf("outDir: %s\n", outputDir);

	array_tokens = calloc(attributes,sizeof(char*));
    	struct movies* data = getData(file, outputDir, argv[cLoc]);
	int sortBy = getSortBy(data, argv[cLoc]);
	if(sortBy == -1)
	{
		printf("This is not a valid sorting index\n");
		fclose(file);
		return -1;
	}
	printf("sortBy: %d\n", sortBy);
    	mergesort(data, 1, f_size - 1, sortBy);
	getCurrDirectory(directory, outputDir, argv[cLoc]);
	//print out the sorted doc 
	/*
	int pid = getpid();
	printf("Initial pid: %d\n", pid);
	*/
	//printOutput(data, "getOut.csv");
	fclose(file);
/*
	int pid = getpid();
	pidHolder = calloc(255, sizeof(int));
	pidCounter = 0;
	printf("Initial pid: %d\n", pid);
	getCurrDirectory(directory, outputDir, argv[cLoc]);
	
	printf("PIDS of all child processes: ");
	for(j = 1; pidHolder[j] != 0; j++)
	{
		if(j == 1)
			printf("%d", pidHolder[j]);	
		else
			printf(", %d", pidHolder[j]);
	}
	printf("\n");
	printf("Total number of processes: %d\n", pidCounter);
*/
/*
	//creates the structure that will be used to hold all the data
	array_tokens = calloc(attributes,sizeof(char*));
    	struct movies* data = getData(file, outputDir);
    	fclose(file);
	
	//finds sortBy index
    	int sortBy = 1;
	int isSortable = 0;
	for(j = 0; j < attributes; j++)
	{
		if(strcmp(argv[cLoc], data[0].array[j]) == 0)
		{
			sortBy = j;
			isSortable = 1;
			break;
		}
	}
	if(isSortable == 0)
	{
		printf("Your sorting input does not match up to what is in the document\n");
		free(data);
		return 0;
	}
	//printf("cloc: %d\n", cLoc);
	//printf("sortBy: %d\n", sortBy);
	
	//directory = "/ilab/users/prd45/Systems";
	printf("dir: %s\n", directory);
	int pid = getpid();
	printf("Initial pid: %d\n", pid);
	getCurrDirectory(directory);

	//calls mergesort to sort the data
    	mergesort(data, 1, f_size - 1, sortBy);
*/
	/*
	for(j = 0;j < f_size-1;j++)
		printf("%s\n",data[j].array[sortBy]);
	*/
	//we were having difficulties printing the header, so the header is stored separetly in the document 
/*
	int structure_array_variable;
	for(j = 0;j < attributes;j++)
	{
		data[0].array[j] = array_tokens[j]; 
	}
	printOutput(data, outputDir);
*/
    return 0;
}

//tells which row the data will be sorted by
int getSortBy(struct movies* data, char* sortBy)
{
	int index = -1;
	int i;
	
	for(i = 0; i < attributes; i++)
	{
		if(strcmp(data[0].array[i], sortBy) == 0)
			index = i; 
	}

	return index;
}

//sets F_size, as well as attributes at the beginning of the program
int getF_size(FILE *file)
{
	printf("in getF_size\n");
	f_size = 0;
	attributes = 0;
	char ch = fgetc(file);

	//finds the number of lines that the code will scan in
	while(!feof(file))
	{
		if(ch == '\n')
		{
			f_size++;
		}
		else if((ch == ',') && (f_size == 0))
			attributes++;
		ch = fgetc(file);
	}
	attributes++;
	close(file);
	return f_size;
}

//fills up a data structure with the information from one document
struct movies* getData(FILE *file, char *outputStr, char *sortBy)
{
	int j;
    
    	fseek(file, 0, SEEK_SET);
 
    	char buff[1024];
	char title[1024]; 

    	//columns
    	fgets(title,1024,file);
    	char* token = strtok(title, ",");

	//sets everything in mo to 0
    	movies *mo = malloc(sizeof(movies) * f_size);
	for(j = 0; j < f_size; j++)
		mo[j].array = calloc(attributes, sizeof(char*)); 

    	for(j = 0; token; j++) 
	{
        	array_tokens[j] = token;
		token = strtok(NULL, ",");
     	}

	//scans in the header
    	int w;
    	for(w = 0; w < attributes; w++)
	{
		mo[0].array[w] = array_tokens[w];
		//printf("%s\n", array_tokens[w]);
	}

	//scans rest of document and trims whitespace
    	int c;
    	for(c = 0; c<f_size-1; c++)
	{
		//main rows
    		fgets(buff,1024,file);
    		char *str = strdup(buff);
    		char *temp = strsep(&str, ",");
		int i;
    		for(i=0;i<attributes;i++)
		{
      			while(isspace((unsigned char)*temp)) 
				temp++;
      			trimTrail(temp);
      			mo[c+1].array[i]=temp;
      			temp = strsep(&str, ",");
    		}
 	}     

	/*
	for(c = 0; c < f_size; c++)
	{
		for(j = 0; j < attributes; j++)
		{
			printf("mo[%d][%d]: %s,", c, j, mo[c].array[j]);
		}
		printf("\n");
	}
	*/
	printOutput(mo, outputStr, sortBy);
  	return mo;
}

//helps with whitespace
void trimTrail(char *str){
    int index, i;
    index = -1;
    
    //Find last index of non-white space character
    i = 0;
    while(str[i] != '\0')
    {
        if(str[i] != ' ' && str[i] != '\t' && str[i] != '\n')
        {
            index= i;
        }
        i++;
    }
    //Mark next character to last non-white space character as NULL
    str[index + 1] = '\0';
}

//prints the information out in a csv file
void printOutput(struct movies *mo, char *outputDir, char *sortBy)
{
	chdir(outputDir);
	char outputStr[80];
	strcpy(outputStr, "AllFiles-sorted-");
	strcat(outputStr, sortBy);
	strcat(outputStr, ".csv");
	printf("outputStr: %s\n", outputStr);
	
        FILE *file = fopen(outputStr, "a");
        int i;
        int j;
        for(i = 0; i < f_size; i++)
        {
                for(j = 0; j < attributes; j++)
                {
                        fprintf(file, "%s", mo[i].array[j]);
	
			if(j != attributes - 1)
				fprintf(file, ",");
                }
                fprintf(file, "\n");
        }
	fclose(file);
}

//add extras documents to the original mo/data
int addRow()
{
}

//recurses through all the directories looking for csv files
int getCurrDirectory(char *directory, char *outputDir, char *sortBy)
{
	printf("***directory: %s\n", directory);
	DIR *currDirectory = opendir(directory);
	struct dirent *dir = readdir(currDirectory);
	int pidCounter = 1;
	if(currDirectory)
    	{
       		while (dir != NULL)
        	{
			if(isCSV(dir->d_name) == 0)
			{
				printf("%s is CSV\n", dir->d_name);
			//put code here that will add a csv file to the output document 
			/*
				FILE *file = fopen(dir->d_name, "r+");
				array_tokens = calloc(attributes,sizeof(char*));
				//getF_size(file);
    				struct movies* data = getData(file, outputDir, sortBy);
    				mergesort(data, 1, f_size - 1, sortBy);
			*/
			}

			//tempDir gets directory path, dir->d_name gets various files/folders in directory
			char* tempDir = calloc((sizeof(directory) + sizeof(dir->d_name) + 1), sizeof(char));
			strcat(tempDir, directory);
			strcat(tempDir, "/");
			strcat(tempDir, dir->d_name);

			//most of the comments lines are about processes, so threads should probably be added here
			if((dir->d_name != NULL ) && (isDirectory(tempDir)) && (strcmp(dir->d_name, ".") != 0) && (strcmp(dir->d_name, "..") != 0))
			{
				int pid1;
				if((pid1 = fork()) > 0)
				{ 
					printf("%s is directory\n", dir->d_name);
					printf("pid: %d\n", pid1);
				/*
					char spid1[5];
					printf("in fork\n");
					pidHolder[pidCounter] = pid1;
				*/
					/*
					if(pidCounter == 1)
					{
						printf("if\n");
						//itoa(pid1, spid1, 10);
						//printf("%s\n", spid1);
						//strcat(pidHolder, pid1);
						printf("%s\n", pidHolder);
					}
					else
					{
						printf("else\n");
						//strcat(pidHolder, ", ");
						//strcat(pidHolder, pid1);
					}
					*/
				/*
					pidCounter++;
					printf("piDCOUNTER: %d\n", pidCounter);
					printf("about to recurse\n");
					if(strcmp(outputDir, ".") == 0)
					{
						outputDir = tempDir;
					}
				*/
					//printf("outputDir: %s\n", outputDir);
					getCurrDirectory(tempDir, outputDir, sortBy);
				//	directory = tempDir;
				}
			}
			dir = readdir(currDirectory);
        	}
        	closedir(currDirectory);
    	}
		
	//printf("Total number of processes: %d\n", pidCounter + 1);
	//make it return pidCounter
    return 0;
}

//makes sure the file in question does not have the word "sorted" in the title, and ends with .csv
int isCSV(char *file)
{
	char *lastOccur = strrchr(file, '.');
	//int w = getColumn(file);
	//printf("%s %d\n", file, w);
	if((strstr(file,"-sorted-") == NULL) && (lastOccur && !strcmp(lastOccur, ".csv")))// && (getColumn(file) != -1))
		return 0;
	else
		return -1;
}

//checks if it is a normal directory 
int isDirectory(const char *path)
{
	//printf("isDir: %s\n", path);
	struct stat statbuf;
	if (stat(path, &statbuf) != 0)
	{
		printf("%d\n", statbuf);
       		return 0;
	}
   	return S_ISDIR(statbuf.st_mode);
}

//checks to make sure every row has the same amount of commas
int getColumn(char *file)
{
	printf("in getColumn\n");
	FILE *myFile = fopen(file, "r");
	printf("%s\n", file);
   	int currChar = fgetc(myFile);
	int commaCounter = 0;
	int lineCounter;
  
	for(lineCounter = 1; lineCounter < f_size;lineCounter++)
	{
		printf("Line: %d\n", lineCounter);
		printf("f_size: %d\n", f_size);
		printf("attribute: %d\n", attributes);
		while(currChar != '\n')
		{
			if(currChar == ',')
			{
				commaCounter++;
				printf("in if\n");
			}
			/*
			if(lineCounter == 65)
				if(currChar != ',')
				printf("%c", currChar);
				else
				printf(" ||||  ");
			*/
			currChar = fgetc(myFile);
		}
		//if one of the lines has the wrong amount of commas, return -1
		if((currChar == '\n') && (commaCounter + 1 != attributes))
			return -1;
		else if(currChar == '\n')
		{
			commaCounter = 0;
			currChar = fgetc(myFile);
		}
	}
   	fclose(myFile);
	return 0;
}
