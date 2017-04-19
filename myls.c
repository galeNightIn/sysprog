#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
//------------------------------------------implementation--------------------------------------//

// char* concat(const char*, const char* );

//------------------------------------------start code------------------------------------------//

int main(int argc, char* argv[])
{	
//-------------------------------------------init section----------------------------------------//

	char link_buffer[1024];
	char* current_dir_name = NULL;
	int close_dir;
	struct dirent* dirent_temp;  
	int st;

//-------------------------------------getting dirrectory name-----------------------------------//
	if(argc < 2) {
		current_dir_name = (char*) malloc(1024 * sizeof(char));
		if (getcwd(current_dir_name, 1024) == NULL) {
			printf("ERROR: %s\n", strerror(errno));
			exit(-1);
		}
	} 
	else {
		current_dir_name = argv[1];
	}

//----------------------------------------open dirrectory----------------------------------------//

	DIR* current_dir = opendir(current_dir_name);
	
	if(current_dir == NULL){
			printf("ERROR: %s\n", strerror(errno));
			exit(-1);
	}
//----------------------------------------some working code----------------------------------------//
	rewinddir(current_dir);

	while( (dirent_temp = readdir(current_dir)) != NULL ){
		
	//-----------------------------------stat info about file--------------------------------------//
		struct stat sb;
		int str_size = strlen(current_dir_name) + strlen(dirent_temp->d_name) + 1; 
		char* str_name = NULL;

		if(argc > 1) {
			str_name = (char* ) malloc(str_size * sizeof(char));
			sprintf(str_name,"%s/%s", current_dir_name, dirent_temp->d_name);
		} else {
			str_name = dirent_temp->d_name;
		}

//		printf("DBG: %s\n", str_name);
		st = lstat(str_name, &sb);
		if(st == -1) {
			printf("ERROR stat: %s\n", strerror(errno));
			exit(-1);
		}

		const char *type = NULL;
		switch (sb.st_mode & S_IFMT) {
	       case S_IFBLK:  type = "_block device:";            break;
	       case S_IFCHR:  type = "_character device:";        break;
	       case S_IFDIR:  type = "_directory:";               break;
	       case S_IFIFO:  type = "_FIFO/pipe:";               break;
	       case S_IFREG:  type = "_regular file:";            break;
	       case S_IFSOCK: type = "_socket:";                  break;
	       case S_IFLNK:  type = "_symlink:";					break;
	       default:       type = "_unknown?:";                break;
        }
        printf("%-20s %3o ", type, sb.st_mode & 0777);

        //printf("File size: 		 %lld bytes ", (long long) sb.st_size);
        //printf("Last status change:       %s", ctime(&sb.st_ctime));
        //printf("Last file access:         %s", ctime(&sb.st_atime));
        //printf("Last file modification:   %s", ctime(&sb.st_mtime));

		printf("%s", dirent_temp->d_name);
		if (S_ISLNK(sb.st_mode)) {
       		int l = readlink(str_name, link_buffer, 1024);
       		link_buffer[l] = '\0';
       		printf(" -> %s", link_buffer);
       	}
		printf("\n");
		if (argc > 1)
		{
			free(str_name);
		}
	}

	if(argc < 2) {
		free(current_dir_name);
	}

//----------------------------------------close dirrectory----------------------------------------//
	close_dir = closedir(current_dir);
	if(close_dir == -1){
			printf("ERROR: %s\n", strerror(errno));
			exit(-1);
	}

	return 0;
}

/*char* concat(const char *s1, const char *s2)
{
    const size_t len1 = strlen(s1);
    const size_t len2 = strlen(s2);
    char *result = malloc(len1+len2+1);//+1 for the zero-terminator
    //in real code you would check for errors in malloc here
    memcpy(result, s1, len1);
    memcpy(result+len1, s2, len2+1);//+1 to copy the null-terminator
    return result;
}*/