#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

typedef enum{false, true} bool;

void do_ls(char[], bool);
void dostat(char *);
void show_file_info(char *, struct stat *);
void mode_to_letters(int, char []);
char *uid_to_name(uid_t);
char *gid_to_name(gid_t);

int main(int ac, char **av)
{
	if(ac==1)
		do_ls(".",false);
	else if((ac==2) && (strcmp(av[1],"-R") == 0 ))
		do_ls(".", true);
	else
		while(--ac){
			printf("%s:\n", *++av);
			if((ac==2) &&((strcmp(av[1],"-R") == 0)))
				do_ls(*av, true);
			else
				do_ls(*av, false);
		}
}

void do_ls(char dirname[], bool check)
{
	DIR *dir_ptr;
	struct dirent *direntp;
	struct stat fstat;
	if((dir_ptr = opendir(dirname)) == NULL)
		fprintf(stderr, "ls1:cannot open %s\n", dirname);
	else
	{
		chdir(dirname);
		while((direntp = readdir(dir_ptr)) != NULL)
		{
			if(strcmp(direntp->d_name, ".") == 0 || strcmp(direntp->d_name,"..") ==0)
			{
				continue;
			}
			if(stat(direntp->d_name, &fstat) == -1)
			{
				fprintf(stderr, "can't stat file (%s)\n", direntp->d_name);
				perror("stat file");
				continue;
			}
			if(S_ISDIR(fstat.st_mode))
			{
				printf("%s\n", direntp->d_name);
				do_ls(direntp->d_name, true);
			}
			else
			{
				dostat(direntp->d_name);
			}
		}
		closedir(dir_ptr);
	}
}

void dostat(char *filename)
{
	struct stat info;
	if(stat(filename, &info) == -1)
		perror(filename);
	else
		show_file_info(filename, &info);
}

void show_file_info(char *filename, struct stat *info_p)
{
	char *uid_to_name(), *ctime(), *gid_to_name(), *filemode();
	void mode_to_letters();
	char modestr[11];

	mode_to_letters(info_p->st_mode, modestr);
	printf("%s", modestr);
	printf("%4d", (int) info_p->st_nlink);
	printf("%-8s", uid_to_name(info_p->st_uid));
	printf("%-8s", gid_to_name(info_p->st_gid));
	printf("%8ld", (long)info_p->st_size);
	printf("%.12s", 4+ctime(&info_p->st_mtime));
	printf("%s\n", filename);

}

void mode_to_letters(int mode, char str[])
{
	strcpy(str, "----------");
	if(S_ISDIR(mode)) str[0] = 'd';
	if(S_ISCHR(mode)) str[0] = 'c';
	if(S_ISBLK(mode)) str[0] = 'b';

	if(mode & S_IRUSR) str[1] = 'r';
	if(mode & S_IWUSR) str[2] = 'w';
	if(mode & S_IXUSR) str[3] = 'x';

	if(mode & S_IRGRP) str[4] = 'r';
	if(mode & S_IWGRP) str[5] = 'w';
	if(mode & S_IXGRP) str[6] = 'x';

	if(mode & S_IROTH) str[7] = 'r';
	if(mode & S_IWOTH) str[8] = 'w';
	if(mode & S_IXOTH) str[9] = 'x';
}

