#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>





int main(int argc, char* argv[])
{	
//-------------------------------------------init section----------------------------------------//

	char* current_dir_name = (char*) malloc(1024 * sizeof(char));
	int close_dir;
	struct dirent* dirent_temp;  
	int st;

//-------------------------------------getting dirrectory name-----------------------------------//
	if(argc < 2) {
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

		//TODO: The problem is in file names, try to use fd instead names
		st = stat(dirent_temp->d_name, &sb);
		if(st == -1) {
			printf("ERROR stat: %s\n", strerror(errno));
			exit(-1);
		}

		switch (sb.st_mode & S_IFMT) {
           case S_IFBLK:  printf("_block device:		");            break;
           case S_IFCHR:  printf("_character device:		");        break;
           case S_IFDIR:  printf("_directory:	");               break;
           case S_IFIFO:  printf("_FIFO/pipe:	");               break;
           case S_IFLNK:  printf("_symlink: 	");                 break;
           case S_IFREG:  printf("_regular file:	");            break;
           case S_IFSOCK: printf("_socket:		");                  break;
           default:       printf("_unknown?: 	");                break;
        }

        //printf("File size: 		 %lld bytes ", (long long) sb.st_size);
        //printf("Last status change:       %s", ctime(&sb.st_ctime));
        //printf("Last file access:         %s", ctime(&sb.st_atime));
        //printf("Last file modification:   %s", ctime(&sb.st_mtime));

   


		printf("%s", dirent_temp->d_name);
		printf("\n");
	}


//----------------------------------------close dirrectory----------------------------------------//
	close_dir = closedir(current_dir);
	if(close_dir == -1){
			printf("ERROR: %s\n", strerror(errno));
			exit(-1);
	}

	return 0;
}