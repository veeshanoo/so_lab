#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#define N 5
 
sem_t sem;
pthread_mutex_t mtx;
 
int S;
int released;
 
void barrier_point() {
    pthread_mutex_lock(&mtx);
    S++;
    pthread_mutex_unlock(&mtx);
    if (S < N) {
        if (sem_wait(&sem)) {
            perror("bad sem wait:");
            exit(errno);
        }
        if (sem_post(&sem)) {
            perror("bad sem post:");
            exit(errno); 
        }
    } else {
        if (sem_post(&sem)) {
            perror("bad sem post");
            exit(errno);
        }
    }
}

void * tfun(void *v) {
    int *tid = (int *)v;
    printf("%d reached the barrier \n", *tid);
    barrier_point();
    sleep(1);
    printf("%d passed the barrier \n", *tid);

    return NULL;
}

int main() {
    pthread_t thr[100];
    int v[] = {1, 2, 3, 4, 5};
    if (sem_init(&sem, 0, S)) {
        perror("bad semaphore init:");
        return errno;
    } 

    if(pthread_mutex_init(&mtx, NULL)) {
        perror("bad mutex init:");
        return errno;
    }

    for(int i = 0;i < N; i++) {
        if (pthread_create(&thr[i], NULL, tfun, &v[i])) {
            perror("bad thread create");
            return errno;
        }
    }
    void *rez;
    for (int i = 0;i < N; i++) {
        if (pthread_join(thr[i], &rez)) {
            perror("bad thread join");
            return errno;
        }
    }
    int *ans = malloc(1 * sizeof(int));
    sem_getvalue(&sem, ans);
    printf("%d", *ans);
    return 0;
}