//program output should match linux cmd: 'find <dir> -type f -print -exec grep <string> {} \; | more'
//don't forget makefile and test
//cmd line parameters: findNgrep.c start_dir keyword
//Works cited: 
//https://www.geeksforgeeks.org/c-program-list-files-sub-directories-directory/
//http://man7.org/linux/man-pages/man3/readdir.3.html
//http://man7.org/linux/man-pages/man7/inode.7.html
//https://codeforwin.org/2018/03/c-program-to-list-all-files-in-a-directory-recursively.html
//https://www.shayanderson.com/ui/media/walk.c.txt
//https://stackoverflow.com/questions/11736060/how-to-read-all-files-in-a-folder-using-c
//http://codewiki.wikidot.com/c:system-calls:stat

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>

void recursive(char *path, char *keyword);

int main(int argc, char** argv){//taking in cmd line input
	char *basepath = argv[1];
	char *keyword = argv[2];
	
	recursive(basepath, keyword);
	
	return 0;
}

void recursive(char *basepath, char *keyword){
	//use: opendir, readdir, stat/lstat
    //opendir(name): open dir, returns pointer to 1st entry
    //readdir(dir): recursive read, return pointer to dirent/next dir
    //stat(char *path, struct stat *buffer): fill buffer with file info
    DIR *dir = opendir(basepath);//get starting directory
    if(dir == NULL){
        printf("Error: could not get starting directory\n");
        return;
    }

    char *word = keyword;
	struct dirent *entry;//create ptr to next dir stream entry
	//struct dirent buffer;
    struct stat info;//create stat object
    //size_t size;
	char path[500];
	char line[200];
	FILE *fp = NULL;

    while((entry = readdir(dir)) != NULL){//for all files & sub-directories of dir...
		stat(basepath, &info);//get info of current place in path
		
        if((info.st_mode & S_IFMT) == S_IFDIR){//if find a directory...
			printf("in file: %s\n", entry->d_name);//debug
			//...and if is '.' directory
			if(strcmp(entry->d_name, ".") == 0){
			    continue;//ignore/onward to the next loop
		    }
			//...and if is '..' directory
			if(strcmp(entry->d_name, "..") == 0){
			    //ignore/shouldn't be going that way
		    }
			//else... recursive read
			strcpy(path, basepath);
			strcat(path, "/");
			strcat(path, entry->d_name);//path: 0 + basepath + /current
			recursive(path, word);
        }
		else if((info.st_mode & S_IFMT) == S_IFREG){//if find a file...
			printf("in file: %s\n", entry->d_name);//debug
			//need to get file out of dirent, then get line out of file
			fp = fopen(entry->d_name, "r");
			printf("got file: %s\n", entry->d_name);//debug
            //fgets(str, size, stream): get line of length 'size' from (FILE) stream, store into str
            while(fgets(line, sizeof(line), fp) != NULL){
			//go through every line in file...
                //strstr(str1, str2): search str1 for str2, return ptr to 1st occurence
                if (strstr(line, word) != NULL){//if keyword in line...
                    printf("%s\n", line);//...print line
                }
            }
		}
		else{//anything else...
			printf("in other: %s\n", entry->d_name);//debug
			continue;//ignore
		}
    }
    closedir(dir);
}