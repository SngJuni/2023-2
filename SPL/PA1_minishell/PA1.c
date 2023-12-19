#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <grp.h>
#include <pwd.h>

int ls(char *dir_path, char *option);
int head(char *file_path, char *line);
int tail(char *file_path, char *line);
int mv(char *file_path1, char* file_path2);
int mk_dir(char *dir_path);
int rm_dir(char *dir_path);
void my_print(const char *str);
void int_to_str(int num, char *str, int len);

#define MAX_CMD_LEN 200

int main(){
	while(1){
		int i, cmdret;
		char cmd[MAX_CMD_LEN];
		char *argument[10];
	    size_t size;	
		
        /* Input commands */

		size = read(STDIN_FILENO, cmd, MAX_CMD_LEN);

		if (size <= 0) {
			exit(1);
		}
		cmd[size - 1] = '\0';

        /* ============== */
        
        /* Tokenize commands (Use strtok function) */

		char *token = strtok(cmd, " ");
		i = 0;
		while (token != NULL) {
			argument[i++] = token;
			token = strtok(NULL, " ");
		}
		argument[i] = NULL;

        /* ======================================= */


        /* Command */

		if (strcmp("ls", argument[0]) == 0){
			cmdret = ls(argument[1], argument[2]);		
		}
		else if (strcmp("head", argument[0]) == 0){
			cmdret = head(argument[3], argument[2]);
		}
		else if (strcmp("tail", argument[0]) == 0){
			cmdret = tail(argument[3], argument[2]);
		}
		else if (strcmp("mv", argument[0]) == 0){
			cmdret = mv(argument[1], argument[2]);
		}
		else if (strcmp("mkdir", argument[0]) == 0){
			cmdret = mk_dir(argument[1]);
		}
		else if (strcmp("rmdir", argument[0]) == 0){
			cmdret = rm_dir(argument[1]);
		}
		else if (strcmp("quit", argument[0]) == 0){
			break;
		}
		else{
			/* Print "ERROR: Invalid commands" */
			my_print("ERROR: Invalid commands\n");
            continue;
		}

		if (cmdret == -1){
            /* Print "ERROR: The command is executed abnormally" */
			my_print("ERROR: The command is executed abnormally\n");
		}
	}
	return 0;
}

int ls(char *dir_path, char *option){
	struct dirent *entry;	
	DIR *dir = opendir(dir_path);

	if (dir == NULL) {
		return -1;
	}
	if (option == NULL) {
		while ((entry = readdir(dir)) != NULL) {
			if (entry->d_type == DT_REG) {
				my_print(entry->d_name);
				my_print(" ");
			}
		}
		my_print("\n");
	}
	else if (strcmp(option, "-al") == 0) {
		while ((entry = readdir(dir)) != NULL) {
			struct stat statbuf;
			struct passwd *pw;
			struct group *gr;

			lstat(entry->d_name, &statbuf);
			pw = getpwuid(statbuf.st_uid);
			gr = getgrgid(statbuf.st_gid);

			my_print(S_ISDIR(statbuf.st_mode) ? "d" : "-");
			my_print((statbuf.st_mode & S_IRUSR) ? "r" : "-");
			my_print((statbuf.st_mode & S_IWUSR) ? "w" : "-");
			my_print((statbuf.st_mode & S_IXUSR) ? "x" : "-");
			my_print((statbuf.st_mode & S_IRGRP) ? "r" : "-");
			my_print((statbuf.st_mode & S_IWGRP) ? "w" : "-");
			my_print((statbuf.st_mode & S_IXGRP) ? "x" : "-");
			my_print((statbuf.st_mode & S_IROTH) ? "r" : "-");
			my_print((statbuf.st_mode & S_IWOTH) ? "w" : "-");
			my_print((statbuf.st_mode & S_IXOTH) ? "x" : "-");
			my_print(" ");
			char link[10];
			int int_link = statbuf.st_nlink;
			int_to_str(statbuf.st_nlink, link, sizeof(link));
			my_print(link);
			my_print(" ");
			my_print(pw->pw_name);
			my_print(" ");
			my_print(gr->gr_name);
			my_print(" ");
			char file_size[10];
			int_to_str(statbuf.st_size, file_size, sizeof(file_size));
			my_print(file_size);
			my_print(" ");
			my_print(entry->d_name);
			my_print("\n");
		}
	}
	else {
		return -1;
	}
	
	return 0;
}

int head(char *file_path, char *line){
	if (strchr(file_path, '/') != NULL) {
		my_print("ERROR: invalid path\n");
		return -1;
	}

	int fd = open(file_path, O_RDONLY);
	int lines = atoi(line);

	if (fd == -1) {
		return -1;
	}
	if (lines < 0) {
		return -1;
	}

	char buf[1];
	int cur = 0;

	while (read(fd, buf, 1) > 0) {
		my_print(buf);
		if (buf[0] == '\n') {
			++cur;
			if (cur >= lines) {
				break;
			}
		}
    }

	close(fd);

	return 0;
}

int tail(char *file_path, char *line){
	if (strchr(file_path, '/') != NULL) {
		my_print("ERROR: invalid path\n");
		return -1;
	}
	int fd = open(file_path, O_RDONLY);
	int lines = atoi(line);

	if (fd == -1) {
		return -1;
	}
	if (lines < 0) {
		return -1;
	}
	
	char buf[1];
	int total = 0;
	int cur = 0;

	while (read(fd, buf, sizeof(buf)) == 1) {
		if (buf[0] == '\n') {
			total++;
		}
	}
    
	lseek(fd, 0, SEEK_SET);

	if (lines >= total) {
		while (read(fd, buf, sizeof(buf)) > 0) {
			my_print(buf);
		}
	}
	else {
		int start = total - lines;
		
		while (read(fd, buf, sizeof(buf)) > 0) {
			if (cur >= start) {
				my_print(buf);
			}
			if (buf[0] == '\n') {
				cur++;
			}
		}
	}

	close(fd);

	return 0;
}

int mv(char *file_path1, char *file_path2){
	if (strchr(file_path1, '/') != NULL || strchr(file_path2, '/') != NULL) {
		my_print("ERROR: invalid path\n");
		return -1;
	}
	int from, to;
	char buf[4096];

	from = open(file_path1, O_RDONLY);

	if (from == -1) {
		return -1;
	}
	to = open(file_path2, O_WRONLY | O_CREAT, S_IWUSR | S_IRUSR);

	if (to == -1) {
		close(from);
		return -1;
	}

	size_t read_bytes;
	size_t written_bytes;

	while ((read_bytes = read(from, buf, sizeof(buf))) > 0) {
		written_bytes = write(to, buf, read_bytes);
		if (written_bytes != read_bytes) {
			close(from);
			close(to);
			return -1;
		}
	}

	close(from);
	close(to);

	if (unlink(file_path1) == -1) {
	}

	return 0;
}

int mk_dir(char *dir_path){
	if (mkdir(dir_path, 0777) != 0) {
		return -1;
	}

	return 0;
}

int rm_dir(char *dir_path){
	if (rmdir(dir_path) == 0) {
        return 0;
    } else {
        my_print("rmdir: failed to remove 'dir_path'\n");
        return -1;
    }
}

void my_print(const char *str) {
	write(STDOUT_FILENO, str, strlen(str));
}

void int_to_str(int num, char *str, int len) {
    int i = 0;

    while (num > 0 && i < len - 1) {
        str[i++] = '0' + (num % 10);
        num /= 10;
    }

    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }

    str[i] = '\0';
}
