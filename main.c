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
char* concatenate(char* A, char* B, char* res); //for directories; concat A and R into res

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

		else if (strcmp ("enter", comm) == 0 ){
			char* dirName = command[1];
			char path[256] = {'\0'};
			concatenate(currentDirectory, dirName, path);			int chdirResult =  chdir(path);
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
			char path [256]= {'\0'};
			concatenate(currentDirectory, command[1], path);
			
			if (isFile(path))
				unlink(path);
			else
				remove_directory(path);
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
char* concatenate(char* A, char* B, char* res){
	strcpy(res, A);
	strcat(res, "\\");
	strcat(res, B);
	return res;
}

int isFile(const char *path)
{
    struct stat pathStat;
    stat(path, &pathStat);
    return S_ISREG(pathStat.st_mode);
}

int remove_directory(const char *path) {
   DIR *d = opendir(path);
   size_t path_len = strlen(path);
   int r = -1;

   if (d) {
      struct dirent *p;

      r = 0;
      while (!r && (p=readdir(d))) {
          int r2 = -1;
          char *buf;
          size_t len;

          /* Skip the names "." and ".." as we don't want to recurse on them. */
          if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
             continue;

          len = path_len + strlen(p->d_name) + 2; 
          buf = malloc(len);

          if (buf) {
             struct stat statbuf;

             snprintf(buf, len, "%s/%s", path, p->d_name);
             if (!stat(buf, &statbuf)) {
                if (S_ISDIR(statbuf.st_mode))
                   r2 = remove_directory(buf);
                else
                   r2 = unlink(buf);
             }
             free(buf);
          }
          r = r2;
      }
      closedir(d);
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

