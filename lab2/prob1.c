#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

const char* arr = "HelloWorld";

int main() {
    if (write(1, arr, strlen(arr)) == -1) {
        perror("write error:");
        return errno;    
    }
    return 0;
}