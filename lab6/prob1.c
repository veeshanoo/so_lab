#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void *runner(void *params) {
    char *str = (char *) params;
    int len = strlen(str);
    char *res = (char*) malloc((len + 1) * sizeof(char));

    for (int i = 0; i < len; i++)
        res[i] = str[len - i - 1];

    return res;
}

int main() {
    pthread_t thread_id;

    if (pthread_create(&thread_id, NULL, runner, "12345678")) {
        perror(NULL);
        return errno;
    }

    char *res = NULL;
    if (pthread_join(thread_id, (void **) &res)) {
        perror(NULL);
        return errno;
    }

    printf("new string: %s\n", res);

    return 0;
}