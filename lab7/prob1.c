#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#define MAX_RESOURCES 5

int available_resources = MAX_RESOURCES;

pthread_mutex_t mtx;

int decrease_count(int count) {
    pthread_mutex_lock(&mtx);
    if (available_resources < count) {
        pthread_mutex_unlock(&mtx);
        return -1;
    } else {
        available_resources -= count;   
        printf("Got %d resources %d remaining\n", count, available_resources);
    }
    pthread_mutex_unlock(&mtx);
    return 0;
}

int increase_count(int count) {
    pthread_mutex_lock(&mtx);
    available_resources += count;
    printf("Released %d resources %d remaining\n", count, available_resources);
    pthread_mutex_unlock(&mtx);
    return 0;
}
 
void *func(void *k) {
    int value = *((int *)k);
    if (value > MAX_RESOURCES) {
        printf("Number of resources needed for current thread greater than maximum number of resources\n");
        return NULL;
    }
    while (1) {
        if (decrease_count(value) == 0) {
            int s = 0;
            sleep(1);
            break;
        } else {
            continue;
        }
    }
    increase_count(value);
    return NULL;
}

int main() {
    int n = 5;
    int v[] = {2, 3, 5, 1, 4};
    pthread_t thr[100];
    if (pthread_mutex_init(&mtx, NULL)) {
        perror("bad mutex init:");
        return errno;
    }

    for (int i = 0; i < n; i++) {
        if (pthread_create(&thr[i], NULL, func, &v[i])) {
            perror("bad thread create:");
            return errno;
        }
    }

    void *rez; // useless rn 
    for (int i = 0; i < n; i++) {
        if (pthread_join(thr[i], &rez)) {
            perror("bad thread join:");
            return errno;
        }
    }

    pthread_mutex_destroy(&mtx);
    return 0;
}