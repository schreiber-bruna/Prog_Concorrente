#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 10000
#define P 4

long soma = 0;
long valor_imprimir = 0;
int pronto = 0;
pthread_mutex_t mutex;
pthread_cond_t cond;


void *ExecutaTarefa(void *arg) {
    for (int i = 0; i < N; i++) {
        pthread_mutex_lock(&mutex);
        soma++;
        if (soma % 1000 == 0) {
            valor_imprimir = soma;
            pronto = 1;
            pthread_cond_signal(&cond);
            while (pronto == 1) {
                pthread_cond_wait(&cond, &mutex);
            }
        }
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *ImprimeMultiplo(void *arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        while (pronto == 0) {
            pthread_cond_wait(&cond, &mutex);
        }

        printf("Soma atingiu múltiplo de 1000: %ld\n", valor_imprimir);
        pronto = 0;
        pthread_cond_broadcast(&cond);
        
        if (soma >= N * P) {
            pthread_mutex_unlock(&mutex);
            break;
        }
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t threads[P];
    pthread_t extra;

    pthread_create(&extra, NULL, ImprimeMultiplo, NULL);

    for (int i = 0; i < P; i++) {
        pthread_create(&threads[i], NULL, ExecutaTarefa, NULL);
    }

    for (int i = 0; i < P; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_join(extra, NULL);
    printf("Soma final = %ld\n", soma);
    return 0;
}
