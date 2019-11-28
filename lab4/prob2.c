#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main() {
    pid_t child_pid;
    child_pid = fork();
    
    if (child_pid < 0) {
        perror("bad fork:");
        return errno;
    } else if (child_pid == 0) {
        int n = 24;
        while (1) {
            printf("%d ", n);
            if (n == 1)
                break;
            if (n % 2 == 0) {
                n /= 2;
            } else {
                n = n * 3 + 1;
            }
        }
    } else {
        int status = 0;
        wait(&status);
        printf("\nreturned status %d\n", status);
    }

    return 0;
}