#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdio.h>


int main() {
    const char* file_from = "test.in";
    const char* file_to = "test.out";

    int file_from_fd = open(file_from, O_RDONLY);
    if (file_from_fd == -1) {
        perror("bad file open:");
        return errno;
    }

    struct stat sb;
    if (stat(file_from, &sb)) {
        perror("error:");
        return errno;
    }

    int file_to_fd = open(file_to, O_CREAT | O_WRONLY);
    if (file_to_fd == -1) {
        perror("bad file open:");
        return errno;
    }

    int nrOfBytes = 0;  
    int bufSize = 5;
    char *input[bufSize];
    int read_so_far = 0;
    while ((nrOfBytes = read(file_from_fd, input, bufSize)) != 0) {
        printf("%d\n", nrOfBytes);
        read_so_far += nrOfBytes;   
        int pendingBytes = nrOfBytes;
        int offset = 0;

        while (pendingBytes > 0) {
            int nrOfWrittenBytes = write(file_to_fd, input + offset, pendingBytes);
            pendingBytes -= nrOfWrittenBytes;
            offset += nrOfWrittenBytes;
        }
    }

    if (read_so_far != sb.st_size) {
        perror("unsuccsessful read");
        return -1;
    }


    close(file_from_fd);
    close(file_to_fd);

    return 0;
}