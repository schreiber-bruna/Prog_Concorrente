#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 10000  // número de incrementos por thread
#define P 4      // número de threads de soma

long soma = 0; // variável compartilhada
long valor_a_imprimir = 0; // guarda o múltiplo exato atingido

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int pronto_para_imprimir = 0; // flag de controle

void *ExecutaTarefa(void *arg) {
    for (int i = 0; i < N; i++) {
        pthread_mutex_lock(&mutex);
        soma++;

        // Se atingiu múltiplo de 1000, salva o valor e avisa a impressora
        if (soma % 1000 == 0) {
            valor_a_imprimir = soma;
            pronto_para_imprimir = 1;
            pthread_cond_signal(&cond);

            // Espera até que a thread impressora libere
            while (pronto_para_imprimir == 1) {
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

        // Espera até que exista algo para imprimir
        while (pronto_para_imprimir == 0) {
            pthread_cond_wait(&cond, &mutex);
        }

        // imprime o valor salvo
        printf("Soma atingiu múltiplo de 1000: %ld\n", valor_a_imprimir);

        // libera as threads de soma
        pronto_para_imprimir = 0;
        pthread_cond_broadcast(&cond);

        // condição de parada
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
    pthread_t impressora;

    // cria thread de impressão
    pthread_create(&impressora, NULL, ImprimeMultiplo, NULL);

    // cria threads de soma
    for (int i = 0; i < P; i++) {
        pthread_create(&threads[i], NULL, ExecutaTarefa, NULL);
    }

    // espera threads de soma terminarem
    for (int i = 0; i < P; i++) {
        pthread_join(threads[i], NULL);
    }

    // espera a thread impressora terminar
    pthread_join(impressora, NULL);

    printf("Soma final = %ld\n", soma);
    return 0;
}
