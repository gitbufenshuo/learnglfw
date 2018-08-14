#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
 
char* fileReadAll (char* filename) {
    int input_fd;
    input_fd = open (filename, O_RDONLY);
    int length = lseek(input_fd, 0, SEEK_END)+1; 
    char* res = malloc(length + 1); // 1 for the 0 end
    res[length] = 0;
    lseek(input_fd, 0, SEEK_SET);
    read (input_fd, res, length);
    close(input_fd);
    return res;
}