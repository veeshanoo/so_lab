#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>


int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("bad fork:");
        return errno;
    } else if (pid == 0) {
        char *binaryPath = "/bin/ls";
        char *args[] = {binaryPath, "-la", NULL};
        execve(binaryPath, args, NULL);
    } else {
        int status = 0;
        wait(&status);
        printf("\nreturned status %d\n", status);
    }
    return 0;
}