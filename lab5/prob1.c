#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    char shm_name[] = "myshm";
    int shm_fd = shm_open(shm_name, O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
    if (shm_fd < 0) {
        perror("bad open:");
        return errno;
    }

    printf("%d\n", shm_fd);

    size_t shm_size = 10 * getpagesize();
    if (ftruncate(shm_fd, shm_size) == -1) {
        perror("bad truncate:");
        shm_unlink(shm_name);
        return errno;
    }

    int page_size = getpagesize();

    int nr[] = {9, 16, 25, 36};
    for (int i = 0; i < 4; i++) { 
        pid_t pid = fork();
        if (pid < 0) {
            perror("bad fork");
            return errno;
        } else if (pid == 0) { 
            int *shm_ptr_write = mmap(0, page_size, PROT_WRITE, MAP_SHARED, shm_fd, i * page_size);
            if (shm_ptr_write == MAP_FAILED) {
                perror("bad mmap");
                shm_unlink(shm_name);
                return errno;
            }

            int x = nr[i];
            int vf = 0;
            int arr[100] = {};
            while (1) {
                arr[++vf] = x;
                if (x == 1) 
                    break;  
                if (x % 2 == 0) {
                    x = x / 2;
                } else {
                    x = 3 * x + 1;
                }
            }

            shm_ptr_write[0] = vf;
            for (int j = 1; j <= vf; j++) {
                shm_ptr_write[j] = arr[j];
            }

            printf("[son] pid %d from [parent] pid %d\n", getpid(), getppid()); 
            int err = munmap(shm_ptr_write, 500);
            if (err != 0) {
                perror("bad munmap:");
                return errno;   
            }
            exit(0); 
        } else {
            // nimeni nimic 
        }
    } 

    for (int i = 0; i < 4; i++) {
        wait(NULL);
        int *shm_ptr_read = mmap(0, page_size, PROT_READ, MAP_SHARED, shm_fd, i * page_size);
        if (shm_ptr_read == MAP_FAILED) {
            perror("bad mmap:");
            shm_unlink(shm_name);
            return errno;
        }

        int vf = shm_ptr_read[0];
        for (int j = 1; j <= vf; j++) {
            printf("%d ", shm_ptr_read[j]);
        }
        printf("\n");
        int err = munmap(shm_ptr_read, 500);
        if (err != 0) {
            perror("bad munmap:");
            return errno;
        }
    } 

    return 0;
}