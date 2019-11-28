#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#define MAX 100

int arr[] = {5, 1, 30, 14, 2};
int sz = sizeof(arr) / sizeof(arr[0]);

int child_pids[MAX];

int main() {
    for (int i = 0; i < sz; i++) {
        pid_t child_pid = fork();
        if (child_pid < 0) {
            perror("bad fork:");
            return errno;
        } else if (child_pid == 0) {
            int nr = arr[i];
            while (1) {
                printf("%d ", nr);
                if (nr == 1)
                    break;
                if (nr % 2 == 0) {
                    nr /= 2;
                } else {
                    nr = nr * 3 + 1;
                }
            }
            printf("\n");
            return 0;
        }  else {
            child_pids[i] = child_pid;
        }
    }

    for (int i = 0; i < sz; i++) {
        int status = 0;
        wait(NULL);
        //waitpid(child_pids[i], &status, 0);
    }
    return 0;
}