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
char* concatenate(char* A, char* B, char* res); //for directories; concat A and R into res, if B is absolute path (C:... - only copy it to res)
int copyFile (char* src_path, char* dst_path );
int copyDirectory(char *from, char *to);
int createPath(char* path, char* currentDir);
void print(char* message, char* cdir);

void push(char* path);
char* pop();

const int MAXWORDS = 5; // maximum accepted amount of words
const int MAXWORDLENGTH = 100;       // maximum length of each word
const int STACKSIZE = 100;

char* stack[100] = {NULL};
int stackPointer = 0;

int main(int argc, char *argv[]) {
	

	char *currentDirectory = getcwd(NULL, 0);
	printf(currentDirectory);
	printf(">");
	
		int x = 1;
	
	char comm[MAXWORDLENGTH];
	while(x){	
		scanf("%s", comm);

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
			print("", currentDirectory);
		}

		else if (strcmp ("enter", comm) == 0 ){
			char dirName[MAXWORDLENGTH];
			scanf("%s", dirName);
			char path[256] = {'\0'};
			concatenate(currentDirectory, dirName, path);			
            int chdirResult =  chdir(path);
			if (chdirResult < 0){
				printf("Failed to enter directory %s\n", dirName);
				print("", currentDirectory);
            }
			else{
				push(currentDirectory);
				currentDirectory = getcwd(NULL, 0);
				print("", currentDirectory);
			}
		}
			
		else if (strcmp ("leave", comm) == 0 ){
			char* path = pop();
			if (path == NULL){
				print("You can't go back any more", currentDirectory);
				continue;
			}
			int chdirResult =  chdir(path);
			if (chdirResult < 0){
			    printf("Failed to go back to directory %s\n", path);
			    print("", currentDirectory);
            }
			else{
				currentDirectory = getcwd(NULL, 0);
				print("", currentDirectory);
			}
		}
		
		else if (strcmp ("copy", comm) == 0 ){
		    char srcPath [256]= {'\0'};
			char dstPath [256]= {'\0'};
			
			char src[MAXWORDLENGTH];
			char dst[MAXWORDLENGTH];
			scanf("%s", src);
			scanf("%s", dst);

			concatenate(currentDirectory, src, srcPath);
			concatenate(currentDirectory, dst, dstPath);
			
			int err;
			
			if (isFile(srcPath))
				err = copyFile(srcPath, dstPath);
			else
				err = copyDirectory(srcPath, dstPath);
            if (!err)
                print("File or directory copied sucessfully", currentDirectory);
            else
                print("Copying failed", currentDirectory);
				
		}
		else if (strcmp ("delete", comm) == 0 ){
			char path [256]= {'\0'};
			char target[MAXWORDLENGTH];
			scanf("%s", target);
			concatenate(currentDirectory, target, path);
			
			int err;
			
			if (isFile(path))
				err = unlink(path);
			else
				err = remove_directory(path);
				
			if (!err)
                print("File or directory deleted sucessfully", currentDirectory);
            else
                print("Deleting failed. Make sure file or directory name is correct", currentDirectory);
		}
		
		else if (strcmp ("create", comm) == 0 ){
		    char fname[256] = {'\0'};               //filename
			scanf("%s", fname);
		    createPath(fname, currentDirectory);
		    char buff[4096];
		    gets(buff);
		    
		    int file = open(fname, O_WRONLY | O_CREAT | O_EXCL, 0666);
		    int n = strlen(buff) + 2;
            int sucess = write(file, buff, n);
            if (!sucess){
                printf("Failed to create a file %s", fname);
                print("", currentDirectory);
            }
            else{
                printf("%s created sucessfully\n", fname);
                print("", currentDirectory);
            }
                
        }
			
		else if (strcmp ("stop", comm) == 0 )
		{
		    printf("Exiting...");
		    x = 0;
        }
			
		else 
			print("Wrong command", currentDirectory);
		
		// read and discard any excess charactrs
        char buff[4096];		
        gets(buff);
	}

	return 0;
}


char* concatenate(char* A, char* B, char* res){
    if (B[1] == ':') {
        //B is absolute (full) path
        strcpy(res, A);
        return res;
    }
    else{ 
        //B is path furher from current ditectory
    	strcpy(res, A);
    	strcat(res, "\\");
    	strcat(res, B);
    	return res;
	}
}

// cant create a directory if it's name include a dot
int createPath(char* path, char* currentDir){
    if (path[1] == ':') {
        //path is absolute, I do not intend to create a disc (C:\..)
        return -1;
    }
    //B is path furher from current ditectory
    char* cDir = currentDir; 
    int i = 0;
    int a = 1;

    while (a)
        {
            int j = 0;
            char dir [256] = {'\0'}; 
            while (path[i] != '\\' && path[i] != '/'){   // somtimes path is written with '/' ?
                if (path[i] == '.'){
                    break;
                }
                            

                dir[j] = path[i];
                i++;
                j++;
            }

            if (path[i] == '.')
                break;
            else{
                char temp [256] = {'\0'}; 
                concatenate(cDir, dir, temp);
                int err = mkdir(temp);
                if (err != 0)
                printf("Failed to create new directory %s If it is already existing, files will be written inside of it. \n", temp);
                strcpy(cDir, temp);
                i++;
            }
        }
    return 0;
}

void print(char* message, char* cdir){
    if (strlen(message)>0)
        printf("%s\n%s>", message, cdir);
    else
        printf("%s>", cdir);
}

int isFile(const char *path)
{
    struct stat pathStat;
    stat(path, &pathStat);
    return S_ISREG(pathStat.st_mode);
}

int copyDirectory(char *from, char *to) {
	// TODO Handle error (failed because of already created, copy to non-existing folder or something else?
	mkdir(to); // enables writing copy a b/c (a contents to be copient into c, c is not created in andvance)
	
    DIR *d = opendir(from);
    size_t PathFromLen = strlen(from);
    size_t PathToLen = strlen(to);

    int r = -1;

    if (d) {
	    struct dirent *p;

		r = 0;
    	while (!r && (p=readdir(d))) {
            int r2 = -1;
            char *bufFrom;
            size_t lenFrom;
            char *bufTo;
            size_t lenTo;

          /* Skip the names "." and ".." as we don't want to recurse on them. */
            if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
                continue;

            lenFrom = PathFromLen + strlen(p->d_name) + 2; 
            bufFrom = malloc(lenFrom);
              
            lenTo = PathToLen + strlen(p->d_name) + 2; 
            bufTo = malloc(lenTo);
        
            if (bufFrom && bufTo) {
                struct stat statbuf;

                snprintf(bufFrom, lenFrom, "%s\\%s", from, p->d_name);
                snprintf(bufTo, lenTo, "%s\\%s", to, p->d_name);
    
                if (!stat(bufFrom, &statbuf)) {
                    if (S_ISDIR(statbuf.st_mode)){
                    	r2 = mkdir(bufTo);
                        r2 = copyDirectory(bufFrom, bufTo);
    				}
                    else
                        r2 = copyFile(bufFrom, bufTo);
                }
                free(bufFrom);
                free(bufTo);
            }
            r = r2;
        }
        closedir(d);
   }
   return r;
}

int copyFile (char* src_path, char* dst_path ){
	int src_fd, dst_fd, n, err;
    char buffer[4096];

    src_fd = open(src_path, O_RDONLY);
    dst_fd = open(dst_path, O_WRONLY | O_CREAT | O_EXCL, 0666);

    while (1) {
        err = read(src_fd, buffer, 4096);
        if (err == -1) {
            printf("Error reading file %s\n", src_path);
            return -1;
        }
        n = err;

        if (n == 0) break;

        err = write(dst_fd, buffer, n);
        if (err == -1) {
            printf("Error writing to file %s\n", dst_path);
            return -1;
        }
	}
	return 0;
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

// Unused after remaking reading from stdin
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

