//Tomas Mikna 

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <limits.h>
#include <dirent.h> 
#include <sys/stat.h>


char* getFirstWord(char* str);
char** split(char* str);
char* concatenate(char* A, char* B);  // for directories

void push(char* path);
char* pop();

const int MAXWORDS = 5; // maximum accepted amount of words
const int MAXWORDLENGTH = 40;       // maximum length of each word
const int STACKSIZE = 100;

//char** stack = (char**)malloc(STACKSIZE * sizeof(char*));
char* stack[100] = {NULL};
int stackPointer = 0;

int main(int argc, char *argv[]) {
	
	
	
	char *currentDirectory = getcwd(NULL, 0);
	printf(currentDirectory);
	printf(">");
	
	
	char commandStr[MAXWORDS*MAXWORDLENGTH];       // command as one string
	int x = 1;
	while(x){
		gets(commandStr);       // get the command
		char** command = split(commandStr); // split command by words (split by space)
		if (command == NULL){
			printf("Wrong command (make sure it does not contain more than %d words and/or more than %d characters)", MAXWORDS, MAXWORDLENGTH);
			continue;
		}
		char* comm = command[0]; // first world of the command; main command without parameters
		
		if(strcmp ("list", comm) == 0 ){
			  DIR *d;
			  struct dirent *dir;
			  d = opendir(currentDirectory);
			  if (d) {
			    while ((dir = readdir(d)) != NULL) {
			      printf("%s\n", dir->d_name);
			    }
			    closedir(d);
			  }
		}
//		else if (strcmp ("enter", getFirstWord(command)) == 0 ){
//			printf("enter");
//		}
		else if (strcmp ("enter", comm) == 0 ){
			char* dirName = command[1];
			char* path = concatenate(currentDirectory, dirName);
			int chdirResult =  chdir(path);
			if (chdir < 0)
				printf("Failed to enter directory %s", dirName);
			else{
				push(currentDirectory);
				currentDirectory = getcwd(NULL, 0);
				printf(getcwd(NULL, 0));
			}
		}
			
		else if (strcmp ("leave", comm) == 0 ){
			char* path = pop();
			if (path == NULL){
				printf("You can't go back any more'");
			}
			int chdirResult =  chdir(path);
			if (chdir < 0)
				printf("Failed to go back to directory %s", path);
			else{
				currentDirectory = getcwd(NULL, 0);
				printf(getcwd(NULL, 0));
			}
		}
		
		else if (strcmp ("copy", comm) == 0 ){
		}
		else if (strcmp ("delete", comm) == 0 ){
			char* path = concatenate(currentDirectory, command[1]);
		//	remove_directory(path);
		}
		
		else if (strcmp ("stop", comm) == 0 )
			x = 0;
		
		else if (strcmp ("stop", comm) == 0 )
			x = 0;
			
		else 
			printf("Wrong command");
				


	}

	return 0;
}

// Unused
char* getFirstWord(char* str){
	int len = sizeof(str)/sizeof(str[0]);
	char subStr[len];
	int i = 0;
	for (i = 0; i<len; i++)
		if (str[i] != '\0'){
			subStr[i] = str[i];
		}
		else{
			subStr[i] = '\0';
			break;
		}
	return subStr;
}

char** split(char* str){	
     
	
	int len = sizeof(str)/sizeof(str[0]);
	char **words;
	int i = 0;
	
	words = (char**)malloc(MAXWORDS * sizeof(char*)); 
    for (i=0; i<MAXWORDS; i++) 
         words[i] = (char*)malloc(MAXWORDLENGTH * sizeof(char)); 
		
	
	i = 0; // original string
	int k = 0; // substr
	int j = 0; // will be j-th word
	while (str[i] != '\0')
	{
		while (str[i] == ' ')   // skip spaces (othervise next word will include leading spaces
			i++;
		while (str[i] != ' ' && str[i] != '\0')
			{
				words[j][k] = str[i];
				k++;
				i++;
			}

		words[j][k] = '\0';  // end of line
		k = 0;
		j += 1;
		if (j >= MAXWORDS)
		return NULL;
	}
	return words;
}
char* concatenate(char* A, char* B){
	char str [256]= {'\0'};
	//str = (char*)malloc(sizeof(A) + sizeof(B)-3*sizeof(char));
	strcpy(str, A);
	strcat(str, "\\");
	strcat(str, B);
	return str;
}

   if (!r)
      r = rmdir(path);

   return r;
}


// Stack //
void push(char* path){
	stack[stackPointer%STACKSIZE] = path;    
	stackPointer++;
}

char* pop(){
	if (stackPointer > 0){
		stackPointer--;
		return stack[stackPointer];
	}
	else{
		return 0;
	}
}

