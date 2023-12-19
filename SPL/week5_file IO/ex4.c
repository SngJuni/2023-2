#include <string.h>
#include <fcntl.h>
#include <unistd.h>

void int_to_str(int num, char *str, int len);

int main(int argc, char* argv[]) {

    char output_filename[] = "Aladdin_num.txt";    

    int input_fd, output_fd;
    char buf[1000];
    int line_num = 1;
    char line_num_str[10];
    int read_bytes;

    input_fd = open(argv[1], O_RDONLY);
    output_fd = open(output_filename, O_WRONLY | O_CREAT, 0755);

    while ((read_bytes = read(input_fd, buf, sizeof(buf))) > 0) {
        int line_start = 0;
        for (int i = 0; i < read_bytes; i++) {
            if (buf[i] == '\n') {
                int_to_str(line_num, line_num_str, sizeof(line_num_str));

                write(output_fd, line_num_str, strlen(line_num_str));
                write(output_fd, " | ", 3);
                write(output_fd, buf + line_start, i - line_start + 1);

                line_start = i + 1;
                line_num++;
            }
        }

        if (line_start < read_bytes) {
            int_to_str(line_num, line_num_str, sizeof(line_num_str));
            
            write(output_fd, line_num_str, strlen(line_num_str));
            write(output_fd, " | ", 3);
            write(output_fd, buf + line_start, read_bytes - line_start);
        }
    }

    close(input_fd);
    close(output_fd);

    return 0;
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
