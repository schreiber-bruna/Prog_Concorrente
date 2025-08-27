#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"


float *vet;
float *res;
long int n;
int nthreads;


typedef struct {
    int id;
} t_args;


float f(float x) {
    return x * x;
}


void *aplicaFuncao(void *arg) {
    t_args *args = (t_args*) arg;
    int id = args->id;
    long int ini, fim, t;

    t = n / nthreads;
    ini = id * t;
    fim = (id == nthreads-1) ? n : ini + t;

    for(long int i=ini; i<fim; i++) {
        res[i] = f(vet[i]);
    }

    free(arg);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    pthread_t *tid;
    double inicio, fim;

    if(argc < 3) {
        printf("Use: %s <dimensao do vetor> <nthreads>\n", argv[0]);
        return 1;
    }

    n = atol(argv[1]);
    nthreads = atoi(argv[2]);
    if(nthreads > n) nthreads = n;

    vet = (float*) malloc(sizeof(float)*n);
    res = (float*) malloc(sizeof(float)*n);
    if(!vet || !res) { printf("Erro malloc\n"); return 2; }


    for(long int i=0; i<n; i++) {
        vet[i] = i * 0.5;
    }

    tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);

    GET_TIME(inicio);

    for(int i=0; i<nthreads; i++) {
        t_args *args = (t_args*) malloc(sizeof(t_args));
        args->id = i;
        if(pthread_create(&tid[i], NULL, aplicaFuncao, (void*) args)) {
            printf("ERRO: pthread_create\n"); return 3;
        }
    }

    for(int i=0; i<nthreads; i++) {
        pthread_join(tid[i], NULL);
    }
    GET_TIME(fim);

    printf("Tempo de execucao: %lf segundos\n", fim-inicio);

    printf("Entrada -> Saída:\n");
    for(int i=0; i<10 && i<n; i++) {
        printf("%.2f -> %.2f\n", vet[i], res[i]);
    }

    free(vet);
    free(res);
    free(tid);
    return 0;
}
