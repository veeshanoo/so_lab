#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define MAXN (100)

int N, M, K;

int A[MAXN][MAXN], B[MAXN][MAXN], C[MAXN][MAXN];

pthread_t thread_ids[MAXN * MAXN];
int thread_cnt = 0;

typedef struct {
    int line, col, res;
} MultiplyResult;

void* multiply(void *params) {
    int *ptr = (int *) params;
    int ii = ptr[0];
    int jj = ptr[1];
    //printf("\n %d %d \n", ii, jj);

    int rs = 0;
    for (int i = 0; i < M; i++) {
        rs += A[ii][i] * B[i][jj];
    }

    MultiplyResult *res = (MultiplyResult *) malloc(sizeof(MultiplyResult));
    res->line = ii;
    res->col = jj;
    res->res = rs;

    return res;

}

int main() {
    FILE* fin = fopen("test.in", "r");

    fscanf(fin, "%d %d", &N, &M);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            fscanf(fin, "%d", &A[i][j]);
        }
    }

    fscanf(fin, "%d %d", &M, &K);
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < K; j++) {
            fscanf(fin, "%d", &B[i][j]);
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < K; j++) {

            int *params = malloc(2 * sizeof(int));
            params[0] = i;
            params[1] = j;
            if (pthread_create(&thread_ids[thread_cnt], NULL, multiply, params ) ) {
                perror("bad thread create:");
                return errno;   
            }
            thread_cnt++;
        }
    }

    for (int i = 0; i < thread_cnt; i++) {
        MultiplyResult *rs;
        if(pthread_join(thread_ids[i], (void **) &rs)) {
            perror("bad thread join:");
            return errno;
        }
        C[rs->line][rs->col] = rs->res;

    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < K; j++) {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }
    
    fclose(fin);
    
    return 0;
}