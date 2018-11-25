#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <assert.h>
#include <dirent.h>
#include "scannerCSVsorter.h"

int rows =  0;
int cols = 0;
int finalRun = 0;
char *header[28]= {"color", "director_name", "num_critic_for_reviews", "duration", "director_facebook_likes", "actor_3_facebook_likes","actor_2_name","actor_1_facebook_likes","gross","genres","actor_1_name","movie_title","num_voted_users","cast_total_facebook_likes","actor_3_name","facenumber_in_poster","plot_keywords","movie_imdb_link","num_user_for_reviews","language","country","content_rating","budget","title_year","actor_2_facebook_likes","imdb_score","aspect_ratio","movie_facebook_likes"};
int process_count = 1;
int main(int argc, char* argv[]){
	
	
	int colFlag = 0;
	int inDirFlag = 0;
	int outDirFlag = 0;
	char* colName = (char*)malloc(100*sizeof(char));
	char* inputDirectory = (char*)malloc(100*sizeof(char));
	char* outputDirectory = (char*)malloc(100*sizeof(char));
	char **a = argv;
	int copied1 = 0;
	int copied2 = 0;
	int copied3 =0;


	//iterate argv to check for flags and arguments

	for (a = argv ; a < argv+argc ; a++) {
	   
	        if((strcmp(*a,"-c")==0) && (colFlag == 0)){
	        	colFlag = 1;

	        	if((a+1) < (argv+argc)){
	        		if( (strcmp( *(a+1), "-d") == 0) || (strcmp( *(a+1), "-o") == 0)){
	        		//printf("error: flag raised but no column name provided\n");
	        		write(STDERROR, "error: flag raised but no column name provided\n", 50);
	        		return 0;
	        		}
	        	}
	        	else if ( (a+1) == (argv+argc)){
		        	//printf("error: flag raised but no column name provided\n");
		        	write(STDERROR, "error: flag raised but no column name provided\n", 50);
		        	return 0;
		        }
	        	
				continue;
	    	}

	    	else if((strcmp(*a,"-d")==0) && (inDirFlag == 0)){
	        	inDirFlag = 1;

	        	if((a+1) < (argv+argc)){
		        	if( (strcmp( *(a+1), "-c") == 0) || (strcmp( *(a+1), "-o") == 0)){
		        		//printf("error: flag raised but no input directory name provided\n");
		        		write(STDERROR, "error: flag raised but no input directory name provided\n", 57);
		        		
		        		return 0;
		        	}
		        }
		        else if ( (a+1) == (argv+argc)){
		        	//printf("error: flag raised but no input directory name provided\n");
		        	write(STDERROR, "error: flag raised but no input directory name provided\n", 57);
		        	
		        	return 0;
		        }

				continue;
	    	}

	    	else if((strcmp(*a,"-o")==0) && (outDirFlag == 0)){
	        	outDirFlag = 1;

	        	if((a+1) < (argv+argc)){
		        	if( (strcmp( *(a+1), "-c") == 0) || (strcmp( *(a+1), "-d") == 0)){
		        		//printf("error: flag raised but no output directory name provided\n");
		        		write(STDERROR, "error: flag raised but no output directory name provided\n" , 58);
		        		return 0;
		        	}
		        }
		        else if ( (a+1) == (argv+argc)){
		        	//printf("error: no output directory name provided\n");
		        	write(STDERROR, "error: flag raised but no output directory name provided\n" , 58);
		        	return 0;
		        }

				continue;
	    	}

	    	// END of checking flags, start of checking arguments

	    	if((colFlag == 1) && (copied1 == 0)){
	    		strcpy(colName, *a);
			copied1 = 1;
			continue;
	    	}

	    	else if((inDirFlag == 1) && (copied2 == 0)){
	    		strcpy(inputDirectory, *a);
			copied2 = 1;
			continue;
	    	}

	    	else if((outDirFlag == 1) && (copied3 == 0)){
	    		strcpy(outputDirectory, *a);
			copied3 = 1;
			continue;
	    	}
		
	}
	if (colFlag == 0){
		char* colFlagerror = "Error: -c argument was not included\n";
		
		write(STDERROR, colFlagerror, 37);
		return 0;
	}  

	if( (strcmp(colName,"color")!=0)&&(strcmp(colName,"director_name")!=0)&&(strcmp(colName,"num_critic_for_reviews")!=0)&&(strcmp(colName,"duration")!=0)&&(strcmp(colName,"director_facebook_likes")!=0)&&(strcmp(colName,"actor_3_facebook_likes")!=0)&&(strcmp(colName,"actor_2_name")!=0)&&(strcmp(colName,"actor_1_facebook_likes")!=0)&&(strcmp(colName,"gross")!=0)&&(strcmp(colName,"genres")!=0)&&(strcmp(colName,"actor_1_name")!=0)&&(strcmp(colName,"movie_title")!=0)&&(strcmp(colName,"num_voted_users")!=0)&&(strcmp(colName,"cast_total_facebook_likes")!=0)&&(strcmp(colName,"actor_3_name")!=0)&&(strcmp(colName,"facenumber_in_poster")!=0)&&(strcmp(colName,"plot_keywords")!=0)&&(strcmp(colName,"movie_imdb_link")!=0)&&(strcmp(colName,"num_user_for_reviews")!=0)&&(strcmp(colName,"language")!=0)&&(strcmp(colName,"country")!=0)&&(strcmp(colName,"content_rating")!=0)&&(strcmp(colName,"budget")!=0)&&(strcmp(colName,"title_year")!=0)&&(strcmp(colName,"actor_2_facebook_likes")!=0)&&(strcmp(colName,"imdb_score")!=0)&&(strcmp(colName,"aspect_ratio")!=0)&&(strcmp(colName,"movie_facebook_likes")!=0) ){


		//printf("error, column name is not a valid column name\n");
		char* hi = "error, column name is not a valid column name\n";
		write(STDERROR, hi, 46);

		return 0;
	}

	//check if input or output directory flags were not raised
	if(inDirFlag == 0 && strcmp(inputDirectory, "") == 0){
		//will use current directory as inputDirectory
		strcpy(inputDirectory, ".");
	}
	if(outDirFlag == 0 && strcmp(outputDirectory, "") == 0){
		//will use current directory as outputDirectory
		strcpy(outputDirectory, ".");
	}


	
	
	
	//printf("colFlag: %d, inDirFlag: %d, outDirFlag: %d\n", colFlag, inDirFlag, outDirFlag);
	//printf("Input column: %s, Input directory: %s, Output directory: %s\n", colName, inputDirectory, outputDirectory);

	pid_t before_pid = getpid();
	
	printf("Initial PID: %d\n", before_pid);
	printf("PIDS of all children processes: ");
	fflush(stdout);
	//printf("different:     asdf");

	listDirectory(inputDirectory, colName, outputDirectory, 0);
	

	pid_t after_pid = getpid();
	if(after_pid == before_pid){

		printf("\n");
		
		printf("Total number of Processes: %d\n", process_count-1);
	}

	free(colName);
	free(inputDirectory);
	free(outputDirectory);
	
	
	
	return 0;
	
}


/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////



void mergeStr(struct Node *arr,int start,int middle,int end, struct Node *temp1, struct Node *temp2){
        int arraySize1 = middle-start+1;
        int arraySize2 = end-middle;

        int i;
        for(i=0; i<arraySize1;i++){
                strcpy(temp1[i].stringData,arr[start+i].stringData);
                strcpy(temp1[i].fullString,arr[start+i].fullString);
        }

        int j;
        for(j=0; j<arraySize2;j++){
                strcpy(temp2[j].stringData,arr[middle+1+j].stringData);
		strcpy(temp2[j].fullString,arr[middle+1+j].fullString);
        }


        i=0; j=0;
        int k=start;

        while(i<arraySize1 && j<arraySize2){
                if(strcmp(temp1[i].stringData,temp2[j].stringData) < 0){
                	strcpy(arr[k].stringData,temp1[i].stringData);
			strcpy(arr[k].fullString,temp1[i].fullString);
	                i++;
                }
                else{
                	strcpy(arr[k].stringData,temp2[j].stringData);
			strcpy(arr[k].fullString,temp2[j].fullString);
                    j++;
                }
                k++;
        }

        while(i<arraySize1){
                strcpy(arr[k].stringData,temp1[i].stringData);
		strcpy(arr[k].fullString,temp1[i].fullString);
                i++;k++;
        }

	while(j<arraySize2){
		strcpy(arr[k].stringData,temp2[j].stringData);
		strcpy(arr[k].fullString,temp2[j].fullString);
		j++;k++;
	}

}

void mergesortStr(struct Node *arr, int start, int end,struct Node *temp1, struct Node *temp2){
        if(start<end){
                int middle = (start+end)/2;

                mergesortStr(arr,start,middle,temp1,temp2);
                mergesortStr(arr,middle+1,end,temp1,temp2);
                mergeStr(arr,start,middle,end,temp1,temp2);
		//printf("HEEEERE\n");
		//printf("%d\n",arr[0].intData);
        }
}



//merge sort for integers
void mergeInt(struct Node *arr,int start,int middle,int end, struct Node *temp1, struct Node *temp2){
        int arraySize1 = middle-start+1;
        int arraySize2 = end-middle;

        int i;
        for(i=0; i<arraySize1;i++){
                temp1[i].intData = arr[start+i].intData;
                strcpy(temp1[i].fullString,arr[start+i].fullString);
        }
        int j;
        for(j=0; j<arraySize2;j++){
                temp2[j].intData = arr[middle+1+j].intData;
		strcpy(temp2[j].fullString,arr[middle+1+j].fullString);
        }


        i=0; j=0;
        int k=start;

        while(i<arraySize1 && j<arraySize2){
                if(temp1[i].intData<temp2[j].intData){
                        arr[k].intData = temp1[i].intData;
			strcpy(arr[k].fullString,temp1[i].fullString);
                        i++;
                }
                else{
                        arr[k].intData = temp2[j].intData;
			strcpy(arr[k].fullString,temp2[j].fullString);
                        j++;
                }
                k++;
        }

        while(i<arraySize1){
                arr[k].intData=temp1[i].intData;
		strcpy(arr[k].fullString,temp1[i].fullString);
                i++;k++;
        }

	while(j<arraySize2){
		arr[k].intData=temp2[j].intData;
		strcpy(arr[k].fullString,temp2[j].fullString);
		j++;k++;
	}

}

void mergesortInt(struct Node *arr, int start, int end,struct Node *temp1, struct Node *temp2){
        if(start<end){
                int middle = (start+end)/2;

                mergesortInt(arr,start,middle,temp1,temp2);
                mergesortInt(arr,middle+1,end,temp1,temp2);
                mergeInt(arr,start,middle,end,temp1,temp2);
        }
}

char* formatOutput(char* inputfile, char* inputColumn, char* outputDir){
	///////formatting output to "<inputfile>-sorted-<inputColumn>.csv"

	char* outputfile = (char*)malloc(1000*sizeof(char));
	char* cat0 = (char*)malloc(100*sizeof(char));
	char* cat1 = (char*)malloc(100*sizeof(char));
	char* cat2 = (char*)malloc(100*sizeof(char));
	char* cat3 = (char*)malloc(100*sizeof(char));
	char* cat4 = (char*)malloc(100*sizeof(char));
	char* cat5 = (char*)malloc(100*sizeof(char));
	char* cat6 = (char*)malloc(100*sizeof(char));

	//making copies of inputfile and inputcolumn
	char* tempinputfile = (char*)malloc(100*sizeof(char));
	char* tempinputColumn = (char*)malloc(100*sizeof(char));
	char* tempoutputDir = (char*)malloc(100*sizeof(char));
	strcpy(tempinputfile, inputfile);
	strcpy(tempinputColumn, inputColumn);
	strcpy(tempoutputDir, outputDir);
	
	//concatenating all the strings together
	//strcpy(cat0, "");
	strcpy(cat1, tempoutputDir);
	//cat1 = strcat(cat0, tempoutputDir);
	cat2 = strcat(cat1, "/");
	
			//trimming ".csv" off the tempinputfile name
			char *ptr;
			ptr = strstr(tempinputfile, ".csv");
			if (ptr != NULL) {
		    	*ptr = '\0';
			}

	cat3 = strcat(cat2, "AllFiles");
	cat4 = strcat(cat3, "-sorted-");
	cat5 = strcat(cat4, tempinputColumn);
	cat6 = strcat(cat5, ".csv");
	strcpy(outputfile, cat6);
	printf("outputFile: %s\n", outputfile);
	
	/*
	int i;
	for(i = 0; i < 28; i++){
		if(i == 27)
			fprintf(outputfile,"%s\n",header[i]);
		else
			fprintf(outputfile,"%s,",header[i]);
	}
	*/
			

	//printf("%s\n", outputfile);
	return outputfile;
}

//this function returns 0 if a file is a valid csv file and returns -1 if invalid file
int checkcsv(char* inputfilepath){

	if(strstr(inputfilepath, ".csv") == NULL){
		fprintf(stderr,"%s is not a csv file, ignore and skip this one\n", inputfilepath);
		return -1;
	}

	else if(strstr(inputfilepath, ".csv") != NULL) {
	//read the file for the first time to check for valid columns

		//case for if you come across a an already sorted <inputfilepath>-sorted-<inputColumn>.csv file
		if((strstr(inputfilepath, "-sorted-") != NULL)){// && ((strstr(inputfilepath,"color")!=NULL)||(strstr(inputfilepath,"director_name")!=NULL)||(strstr(inputfilepath,"num_critic_for_reviews")!=NULL)||(strstr(inputfilepath,"duration")!=NULL)||(strstr(inputfilepath,"director_facebook_likes")!=NULL)||(strstr(inputfilepath,"actor_3_facebook_likes")!=NULL)||(strstr(inputfilepath,"actor_2_name")!=NULL)||(strstr(inputfilepath,"actor_1_facebook_likes")!=NULL)||(strstr(inputfilepath,"gross")!=NULL)||(strstr(inputfilepath,"genres")!=NULL)||(strstr(inputfilepath,"actor_1_name")!=NULL)||(strstr(inputfilepath,"movie_title")!=NULL)||(strstr(inputfilepath,"num_voted_users")!=NULL)||(strstr(inputfilepath,"cast_total_facebook_likes")!=NULL)||(strstr(inputfilepath,"actor_3_name")!=NULL)||(strstr(inputfilepath,"facenumber_in_poster")!=NULL)||(strstr(inputfilepath,"plot_keywords")!=NULL)||(strstr(inputfilepath,"movie_imdb_link")!=NULL)||(strstr(inputfilepath,"num_user_for_reviews")!=NULL)||(strstr(inputfilepath,"language")!=NULL)||(strstr(inputfilepath,"country")!=NULL)||(strstr(inputfilepath,"content_rating")!=NULL)||(strstr(inputfilepath,"budget")!=NULL)||(strstr(inputfilepath,"title_year")!=NULL)||(strstr(inputfilepath,"actor_2_facebook_likes")!=NULL)||(strstr(inputfilepath,"imdb_score")!=NULL)||(strstr(inputfilepath,"aspect_ratio")!=NULL)||(strstr(inputfilepath,"movie_facebook_likes")!=NULL))){
			
			fprintf(stderr, "Error: This file has already been sorted before\n");
			return -1;

		}


		///NEED FULL PATH NAME TO OPEN FILE//////////////////////////////////////////////
		FILE* myFile0 = fopen(inputfilepath, "r");
		if (myFile0 == NULL){
			fprintf(stderr,"ERROR: in checkcsv, myFile0 was NULL");
		}
		int lineNum = 0; ////used to check each line of a file
		char *line = (char*)malloc(100*sizeof(char));
		size_t line_size = 0;
		int i= 0; //for loop increment int
		int j = 0;
		int res1 = 0; //counts number of occurrences of a character in first line
		int res2 = 0; //counts number of occurrences of a character in all the following lines
		char* tmp;
		char* tmp2;
		int ignoreComma = 0; //int for counting number of ' " ' in a line. this is for the case that theres a comma in a field
		//printf("calling the getline function in checkcsv...\n");
		//getline(&line, &line_size, myFile0 ); 

		while( getline(&line, &line_size, myFile0 ) != -1 ){	
			//printf("getline function passed!");	
			if(lineNum == 0){
				
				tmp = line; //temp ptr to line buffer of the very first line
				//printf("%s\n", tmp);

				//checks if the csv file has all the required columns. Otherwise report bad csv file
				/*
				if( (strstr(tmp,"color")==NULL)||(strstr(tmp,"director_name")==NULL)||(strstr(tmp,"num_critic_for_reviews")==NULL)||(strstr(tmp,"duration")==NULL)||(strstr(tmp,"director_facebook_likes")==NULL)||(strstr(tmp,"actor_3_facebook_likes")==NULL)||(strstr(tmp,"actor_2_name")==NULL)||(strstr(tmp,"actor_1_facebook_likes")==NULL)||(strstr(tmp,"gross")==NULL)||(strstr(tmp,"genres")==NULL)||(strstr(tmp,"actor_1_name")==NULL)||(strstr(tmp,"movie_title")==NULL)||(strstr(tmp,"num_voted_users")==NULL)||(strstr(tmp,"cast_total_facebook_likes")==NULL)||(strstr(tmp,"actor_3_name")==NULL)||(strstr(tmp,"facenumber_in_poster")==NULL)||(strstr(tmp,"plot_keywords")==NULL)||(strstr(tmp,"movie_imdb_link")==NULL)||(strstr(tmp,"num_user_for_reviews")==NULL)||(strstr(tmp,"language")==NULL)||(strstr(tmp,"country")==NULL)||(strstr(tmp,"content_rating")==NULL)||(strstr(tmp,"budget")==NULL)||(strstr(tmp,"title_year")==NULL)||(strstr(tmp,"actor_2_facebook_likes")==NULL)||(strstr(tmp,"imdb_score")==NULL)||(strstr(tmp,"aspect_ratio")==NULL)||(strstr(tmp,"movie_facebook_likes")==NULL) ){
					fprintf(stderr,"error, bad csv file, this file does not have all the required columns\n");
					return -1;
				}
				*/
				for(i = 0; i<strlen(tmp); i++){

					if (tmp[i] == ',') {
	        		res1++;         
	        		}
				}
				//printf("comma count for the first line : %d\n", res1);

	  		
	  			lineNum++;
			}

			else if(lineNum > 0){
				tmp2 = line; //temp ptr to line buffer of the 2nd line
				//fprintf(stderr,"%s\n", tmp2);
				for(j = 0; j<strlen(tmp2); j++){

					if(tmp2[j] == '"'){
						ignoreComma++;
					}

					if ((tmp2[j] == ',') && (ignoreComma % 2 == 0)) {
	        		res2++;          	
	        		}

				}
				lineNum++;

				//printf("comma count for line %d: %d\n", lineNum, res2);
	  		
	  			if(res1 == res2){
	  				//printf("same number of commas LETS GET THIS FUKIN BREAD\n");
	  				
	  				res2 = 0;
	  				tmp2 = NULL; 	//reset values of tmp2 and res2

	  				continue;
	  			}
	  			else if ( res1 != res2 ){
	  				fprintf(stderr,"the columns are messed up, don't use this csv file\n");
	  				//printf("res1 : %d, res2 : %d", res1, res2);
	  				pclose(myFile0);
	  				return -1;
	  			}  			  			
			}

			else {
				lineNum++;
			}
		}
		pclose(myFile0);
		return 0;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//works like strtok, but handles null spots
static char *delimetizer(char *str, char const *delims)
{
	static char *src = NULL;
	char *p,*ret = 0;

        if (str != NULL)
        	src = str;

        if (src == NULL || *src == '\0')
		return NULL;

        ret = src;
        if ((p = strpbrk(src, delims)) != NULL)
        {
		*p  = 0;
                ret = src;
                src = ++p;
        }
        else
        	src += strlen(src);

	return ret;
}


//finds the number of columns in the document
int getSize(char* inputfilepath){

	
	FILE *file = fopen(inputfilepath, "r");
        char ch = fgetc(file);
 

        while(!feof(file))
        {
                if(ch == '\n')
                        rows++;
                if((ch == ',') && (rows == 0))
                        cols++;
                ch = fgetc(file);
        }

        cols++;
        close(file);
        rewind(file);
        return 0; 
}


//finds how the columns in currDoc relate to the masterDoc
int findPos(char* toFind1)
{
	//printf("\n");
	//printf("\nIWUZHERE: %s\n", toFind1);
	int output = 27;
	int i;
	for(i = 0; i < 28; i++)
	{
		//printf("header[%d]: %s\n", i, header[i]);
		if(strcmp(header[i],toFind1) == 0)
		{
			//printf("%s at %d\n", toFind, i);
			return i;
		}
	}
	return output;
}

/*
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
*/


//rearranges the document so it always has 28 rows in the right order
int formatcsv(char* inputfilepath, char* inputfile, char* inputColumn, char* outputDir){


	//finds the size of the current document we will be using. Rows and Cols are global variables that change with each document
	getSize(inputfilepath);


	int i = 0;
	int j = 0;
	char buffer[1024];
   	char *record,*line,*small;	
	int positions[cols];	
	char* myStruct[rows][cols];


	//myStruct will hold the individual values of the entire document
	for(i = 0; i < rows; i++)
		for(j = 0; j < cols; j++)
			myStruct[i][j] = malloc(sizeof(char*));
	

	FILE *file = fopen(inputfilepath, "r+");
	i = 0;
	j = 0;



	//stores the entire document in myStruct
	while(i < rows)
   	{
		line=fgets(buffer,sizeof(buffer),file);
     		record = delimetizer(line,"\n");
		//printf("%s\n",record);
		small = delimetizer(record,",");

     		while(j < cols)
     		{
			strcpy(myStruct[i][j], small);
			//printf("myStruct[%d][%d]: {%s}\n", i, j, myStruct[i][j]);
			


			if(i == 0)
			{
				positions[j] = findPos(myStruct[i][j]);
				//printf("pos[%d]: %d\n", j, positions[j]);
			}


     			small = delimetizer(NULL,",");
			j++;
     		}
		//printf("record: %s\n", record);
		j = 0;
     		i++;
   	}


	/*
	printf("MY STRUCT:\n");
	i = 0;
	j = 0;
	while(i < rows)
	{
		while(j < cols)
		{
			printf("myStruct[%d][%d]: {%s}\n", i, j, myStruct[i][j]);
			j++;
		}
		j = 0;
		printf("\n");
		i++;
	}
	*/


	//for storing myStructure in order
	char* myPrinting[rows][28];
	for(i = 0; i < rows; i++)
		for(j = 0; j < 28; j++)
			myPrinting[i][j] = malloc(sizeof(char*));



	//Stores the columns in the same order as the header
	for(i = 0; i < cols; i++)
	{
		int tempPos = positions[i];
		//printf("currpos[%d]: %d\n", i, tempPos);
		for(j = 0; j < rows; j++)
		{
			strcpy(myPrinting[j][tempPos], myStruct[j][i]);
			//myPrinting[j][positions[i]] = myStruct[j][i];
			
			//printf("myStruct[%d][%d]: %s\n", j,i, myStruct[j][i]);
			
		}
	}

	printf("print meeeeeeeeeeeeeeeeee\n");
	//fills in empty columns with empty string
	for(i = 0; i < rows; i++)
	{
		for(j = 0; j < 28; j++)
		{
			//printf("***myPrinting[%d][%d]\n", i, j);
			if(strcmp(myPrinting[i][j],"") == 0)
			{
				strcpy(myPrinting[i][j], "HERE");
			}
			printf("myPrinting[%d][%d]: {%s}\n", i, j, myPrinting[i][j]);
		}
	}
				


	rewind(file);


/*
	//writes over the original document
	for(i = 0; i < rows; i++)
		for(j = 0; j < cols; j++)
			if(i == 27)
				fprintf("%s\n", myPrinting[i][j]);
			else
				fprintf("%s,", myPrinting[i][j]);
	
*/

/*
	for(i = 0; i < rows; i++)
		for(j = 0; j < cols; j++)
		{
			free(myStruct[i][j]);
			free(myPrinting[i][j]);
		}

*/
	
	fclose(file);

	return 0;
}



//handles the sorting of an input file. Returns 0 on sucess, and -1 for failure
int csvfilehandler(char* inputfilepath, char* inputfile, char* inputColumn, char* outputDir){
	

	//printf("%s\n", inputColumn );

	//printf("entering checkcsv function...\n");

	if(finalRun == 0)
	{
		int checkcsv1 = checkcsv(inputfilepath);
		if(checkcsv1 == -1){
			fprintf(stderr, "%s is not a valid csv file\n", inputfile);
			return -1;
		}
		else
			printf("%s is processing...\n", inputfile);

	formatcsv(inputfilepath, inputfile, inputColumn, outputDir);

	}

	
	//must be a valid columnName from the movie_metadata
	if( (strcmp(inputColumn,"color")!=0) && (strcmp(inputColumn,"director_name")!=0)&&(strcmp(inputColumn,"num_critic_for_reviews")!=0)&&(strcmp(inputColumn,"duration")!=0)&&(strcmp(inputColumn,"director_facebook_likes")!=0)&&(strcmp(inputColumn,"actor_3_facebook_likes")!=0)&&(strcmp(inputColumn,"actor_2_name")!=0)&&(strcmp(inputColumn,"actor_1_facebook_likes")!=0)&&(strcmp(inputColumn,"gross")!=0)&&(strcmp(inputColumn,"genres")!=0)&&(strcmp(inputColumn,"actor_1_name")!=0)&&(strcmp(inputColumn,"movie_title")!=0)&&(strcmp(inputColumn,"num_voted_users")!=0)&&(strcmp(inputColumn,"cast_total_facebook_likes")!=0)&&(strcmp(inputColumn,"actor_3_name")!=0)&&(strcmp(inputColumn,"facenumber_in_poster")!=0)&&(strcmp(inputColumn,"plot_keywords")!=0)&&(strcmp(inputColumn,"movie_imdb_link")!=0)&&(strcmp(inputColumn,"num_user_for_reviews")!=0)&&(strcmp(inputColumn,"language")!=0)&&(strcmp(inputColumn,"country")!=0)&&(strcmp(inputColumn,"content_rating")!=0)&&(strcmp(inputColumn,"budget")!=0)&&(strcmp(inputColumn,"title_year")!=0)&&(strcmp(inputColumn,"actor_2_facebook_likes")!=0)&&(strcmp(inputColumn,"imdb_score")!=0)&&(strcmp(inputColumn,"aspect_ratio")!=0)&&(strcmp(inputColumn,"movie_facebook_likes")!=0) ){
		//fprintf(stderr, "printing inputColumn: %s\n", inputColumn );

		fprintf(stderr, "error, your third argument is not a valid &&&&& column name\n");
		return -1;
	}

	//is column we are sorting int? this int is 0 if false, 1 if true
	int isInt=0;

	if((strcmp(inputColumn,"num_critic_for_reviews")==0)||(strcmp(inputColumn,"duration")==0)||(strcmp(inputColumn,"director_facebook_likes")==0)||(strcmp(inputColumn,"actor_3_facebook_likes")==0)||(strcmp(inputColumn,"actor_1_facebook_likes")==0)||(strcmp(inputColumn,"gross")==0)||(strcmp(inputColumn,"num_voted_users")==0)||(strcmp(inputColumn,"cast_total_facebook_likes")==0)||(strcmp(inputColumn,"facenumber_in_poster")==0)||(strcmp(inputColumn,"num_user_for_reviews")==0)||(strcmp(inputColumn,"budget")==0)||(strcmp(inputColumn,"title_year")==0)||(strcmp(inputColumn,"actor_2_facebook_likes")==0)||(strcmp(inputColumn,"imdb_score")==0)||(strcmp(inputColumn,"aspect_ratio")==0)||(strcmp(inputColumn,"movie_facebook_likes")==0)){
		isInt = 1;
	}

	FILE *myFile;
	
	myFile = fopen(inputfilepath, "r");

        if(myFile == NULL){
                fprintf(stderr,"Failed to open and read the file.\n");
		return -1;
        }

	int c;

	//this buf String will hold the whole row
	int bufSize = 64;
	char* buf = (char *)malloc(bufSize*sizeof(char));
	int temp = bufSize;

	//this String only holds the desired data from column being compared
	char* compareC = (char *)malloc(bufSize*sizeof(char));
	//int y is indexing for compareC
	int y=0;	

	int charIndex = 0;

	//counts number of commas in current line
	int commaCount=0;

	//keeps count of line number in original file
	int lineCount=0;
	
	//tells how many commas have passed until it htis the field that wants to be sorted
	int commaColSort=-1;


	//array of Nodes so that list can be sorted
	struct Node* myArray = malloc(1024*1024*sizeof(struct Node));
	

	//this one only exists to stop if statement from running
	int tempbreak = 0;

	//counts total chars to be able to realloc
	int totalChar=0;	

	//this int will be the conditional to tell stringData to take in ',' if its part of the value being compared
	int takeComma = 0;
	

	//read to end of file
	while((c=fgetc(myFile))!=EOF){

		
		if(totalChar<bufSize){
			buf[charIndex] = c;
		}

		if(totalChar == bufSize){
			char *hold = realloc(buf, 2*bufSize);
			if(hold == NULL){
				fprintf(stderr,"realloc() error: memory was not correctly allocated\n");
				return -1;
			}
			buf=hold;
			bufSize*=2;

			buf[charIndex]=c;
		}

		//this set of conditionals finds how many commas passes before the matched column is found
		if((lineCount==0)&&((c==',')||(c=='\n'))){
	
			//columnName found, store # commas we must parse before getting the wanted field
			if((strstr(buf,inputColumn)!=NULL)&&(tempbreak==0)){
				commaColSort = commaCount;
				tempbreak = 1;
			}

			if(c==','){
				++commaCount;
			}
			
			//columnName we are sorting cannot be found
			if((strstr(buf,inputColumn)==NULL)&&(c=='\n')){
				fprintf(stderr,"The indicated column can not be found, please put valid column name\n");
				return -1;
			}
		//	printf("buff[%d]: %d\n", charIndex, buf[charIndex]);	
		}

		//this is for the condition where we have " " for values with commas in them
	        if(c=='"'){
                        if(takeComma==0){
        	                takeComma=1;
                        }
                        else{
                                takeComma=0;
                        }
	        }

		//past the 1st line, check whats in the selected column and store that data
		if(lineCount>0){

			//if we passed correct number of commas, we can start passing in this part of string
			if(commaColSort == commaCount){
				if((c!=' ')&&(c!='\n')){

					if(y==temp){
						char *hold2 = realloc(compareC, 2*temp);
						if(hold2 == NULL){
							fprintf(stderr,"realloc() error: memory was not correctly allocated\n");
							return -1;
						}
						compareC = hold2;
						temp*=2;
					}
					if(c!='"'){
						compareC[y] = c;
						y++;
					}
				}
				//this situation takes care of the last column field
				if(c=='\n'){
					compareC[y]='\0';
				}
			}


			//we have passed the portion where we want to compare, so stop passing string in
			if(commaColSort<commaCount){
				compareC[y-1] = '\0';
			}

			if((c==',')&&(takeComma==0)){
				++commaCount;
			}

		}



		++totalChar;
		++charIndex;

		//end of the line, completes the string with \0 and stores the whole string
		if(c=='\n'){
			--charIndex;
			buf[charIndex]='\0';
		
			myArray[lineCount].fullString = malloc(charIndex*sizeof(struct Node));
			strcpy(myArray[lineCount].fullString,buf);
			//printf("%s\n", buf);
			
			//printf("%s\n", compareC);
			//if column being compared is chars
			if(isInt==0){
				myArray[lineCount].stringData = malloc(charIndex*sizeof(struct Node));
				strcpy(myArray[lineCount].stringData,compareC);
			}

			//if column being compared is int, need to trim whats being compared
			if(isInt==1){
				myArray[lineCount].intData = atoi(compareC);
			}

			charIndex=0;

			//printf("Current line:%i; stored portion:%s; whole line:%s\n",lineCount,compareC,myArray[lineCount].fullString);
			//printf("Current line:%i; stored portion:%i; whole line:%s\n",lineCount,myArray[lineCount].intData,myArray[lineCount].fullString);

			++lineCount;
			commaCount = 0;
			tempbreak=0;
			y=0;
		}

	}
        struct Node* temp1=malloc(lineCount*sizeof(struct Node));
        struct Node* temp2=malloc(lineCount*sizeof(struct Node));
	
	int j;
	for(j=0;j<lineCount;j++){
		temp1[j].fullString = malloc(bufSize*sizeof(struct Node));
		temp2[j].fullString = malloc(bufSize*sizeof(struct Node));
	}


	free(buf);
	free(compareC);

	if(isInt==1){
		mergesortInt(myArray,1,lineCount-1,temp1,temp2);
	}

	if(isInt==0){
		int f;
		for(f=0;f<lineCount;f++){
			temp1[f].stringData = malloc(bufSize*sizeof(struct Node));
			temp2[f].stringData = malloc(bufSize*sizeof(struct Node));
		}
		mergesortStr(myArray,1,lineCount-1,temp1,temp2);
	}

	
	//creating sorted file in given output directory
	FILE* output;
	
	if(finalRun == 0)
		output = fopen(formatOutput(inputfile, inputColumn, outputDir), "a");
	else
		output = fopen(formatOutput(inputfile, inputColumn, outputDir), "w");
	
	if(output == NULL){
		fprintf(stderr, "Error in line 521: could not open file");
		return -2;
	}
	
	fprintf(output,"%s\n",myArray[0].fullString);

	int z;
	for(z=1;z<lineCount;z++)
	{
		//printf("%i\n",myArray[z].intData);
		//printf("%s\n",myArray[z].stringData);
		if(finalRun != 0)
		{
			//printf("finalRun != 0\n");
			//printf("myArray[%d]: %c\n", z, myArray[z].fullString[0]);
			if(myArray[z].fullString[0] == 'c')
			{
				continue;
			}
		}
		fprintf(output,"%s\n",myArray[z].fullString);
	}

	
	free(temp1);
	free(temp2);

	
	pclose(output);
	pclose(myFile);
	
	

	return 0;
}



void listDirectory(char* inputDirectory, char* inputColumn, char* outputDir, int indent){

	DIR *dir;
	DIR *outdir;
	struct dirent *entry;
	int forked_pid;
	int status;
	//int processcount_ToReturn;


	if(!(dir = opendir(inputDirectory))){
		fprintf(stderr, "Error: could not open input directory or it does not exist");
		return;
	}


	if(!(outdir = opendir(outputDir))){
		fprintf(stderr, "Error: could not open output directory or it does not exist\n");
		return;
	}

	else {
		closedir(outdir);
	}



	//printf("currently in listDirectory function...\n");

	while ((entry = readdir(dir)) != NULL){
		if(entry->d_type == DT_DIR) {// found a directory

			//this conditional excludes using the current directory and parent directory
			if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0){	

				forked_pid = fork();

				if(forked_pid == 0){ //I am the child

					
					printf("%d,", getpid());
					//printf("REEEEEEEE Im the child process for handling directories\n");
					fflush(stdout);

					char path[1024];
					snprintf(path, sizeof(path), "%s/%s", inputDirectory, entry->d_name);
					//printf("%s\n", path);

					//printf("%*s[%s]\n", indent, "",entry->d_name);
					process_count = 1;
					listDirectory(path,inputColumn, outputDir, indent + 2);


					
					
					exit(process_count);
				}
		
				else if (forked_pid != 0) {//I am the parent 

					waitpid(forked_pid, &status, 0);
					
					process_count +=WEXITSTATUS(status);

				}
			}

			
		
		}
		else{ //handling a file
			//run csvfilehandler
			//printf("now calling csvfilehandler function...\n");

			
			

			
				forked_pid = fork();
					if(forked_pid == 0){//I am the child process for filehandler

						
						printf("%d,", getpid());
						fflush(stdout);

						//printf("REEEEE Im in the child process to handle files\n");
						//printf("I am the child process to handle files asdfasdf\n");

						
						char path2[1024];
						snprintf(path2, sizeof(path2), "%s/%s", inputDirectory, entry->d_name);
						
						/*char path2[1024];
						snprintf(path2, sizeof(path2), "%s/%s", inputDirectory, entry->d_name);*/
						int test = csvfilehandler(path2, entry->d_name, inputColumn, outputDir );

						exit(1);
					}
					else{// I am the parent process for filehandler
						waitpid(forked_pid, &status, 0);
						
						process_count += WEXITSTATUS(status);
					}

					//printf("path2 string :%s\n", path2);
					//printf("entry->d_name string: %s\n", entry->d_name);
					/*if(test == 0){
						fprintf(stderr,"csvfile was successfully sorted\n");
					}
					else if(test == -1){
						fprintf(stderr, "csvfile was not a valid file and could not be sorted\n");
					}
					else if(test == -2){
						//misleading statement lol :fprintf(stderr, "Error: output directory does not exist\n ");
					}*/

					//printf("%*s- %s\n", indent, "", entry->d_name);
			
		}
	}
	
	closedir(dir);
	finalRun = 1;
	char inputfilepath[100] = "";
	char inputfile[] = "AllFiles-sorted-";
	strcat(inputfilepath, outputDir);
	strcat(inputfilepath, "/");
	strcat(inputfilepath,inputfile);
	strcat(inputfilepath,inputColumn);
	strcat(inputfilepath, ".csv");
	csvfilehandler(inputfilepath, inputfile, inputColumn, outputDir);
	return;
}

