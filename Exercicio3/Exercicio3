#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <math.h>
#include <unistd.h>

long long *buffer;
int *primos;
int M;
int N;
int C;
int in = 0;
int out = 0;
int quant_buf = 0;
int total_produzido = 0;
int total_consumido = 0;

sem_t buffer_vazio;
sem_t items;
pthread_mutex_t buf_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t quat_primos_mutex = PTHREAD_MUTEX_INITIALIZER;

int ehPrimo(long long int n) {
    int i;
    if (n <= 1){
        return 0;
    } 
    if (n == 2){
        return 1;
    }
    if (n % 2 == 0){
        return 0;
    }
    for (i = 3; i < (int)(sqrt((double)n) + 1); i += 2)
        if (n % i == 0) return 0;
    return 1;
}

void *produtor(void *arg) {
    long long comeca = 1;
    while (1) {
        sem_wait(&buffer_vazio);

        pthread_mutex_lock(&mutex);
        int falta = N - total_produzido;
        if (falta <= 0) {
            pthread_mutex_unlock(&mutex);

            pthread_mutex_lock(&buf_mutex);
            for (int i = 0; i < C; ++i) {
                buffer[in] = -1;
                in = (in + 1) % M;
                quant_buf++;
                sem_post(&items);
            }
            pthread_mutex_unlock(&buf_mutex);
            break;
        }
        int count = (falta < M) ? falta : M;
        pthread_mutex_unlock(&mutex);

        pthread_mutex_lock(&buf_mutex);
        for (int i = 0; i < count; ++i) {
            long long valor_gerado = comeca + total_produzido + i;
            buffer[in] = valor_gerado;
            in = (in + 1) % M;
            quant_buf++;
            sem_post(&items);
        }
        pthread_mutex_unlock(&buf_mutex);
        pthread_mutex_lock(&mutex);
        total_produzido += count;
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

void *consumidor(void *arg) {
    int id = *((int*)arg);
    free(arg);

    while (1) {
        sem_wait(&items);

        pthread_mutex_lock(&buf_mutex);
        long long valor_retirado = buffer[out];
        out = (out + 1) % M;
        quant_buf--;
        if (quant_buf == 0) {
            sem_post(&buffer_vazio);
        }
        pthread_mutex_unlock(&buf_mutex);

        if (valor_retirado == -1) {
            break;
        }

        pthread_mutex_lock(&mutex);
        total_consumido++;
        pthread_mutex_unlock(&mutex);

        if (ehPrimo(valor_retirado)) {
            pthread_mutex_lock(&quat_primos_mutex);
            primos[id]++;
            pthread_mutex_unlock(&quat_primos_mutex);
        }
    }

    return NULL;
}

int main() {
    printf("Digite o valor de N (quantidade total de números): ");
    scanf("%d", &N);
    printf("Digite o valor de M (tamanho do buffer): ");
    scanf("%d", &M);
    printf("Digite o valor de C (quatidade de threads consumidoras): ");
    scanf("%d", &C);

    if (N <= 0 || M <= 0 || C <= 0) {
        fprintf(stderr, "N, M e C devem ser maiores que 0.\n");
        return 1;
    }

    buffer = (long long*) malloc(sizeof(long long) * M);
    primos = (int*) calloc(C, sizeof(int));

    sem_init(&buffer_vazio, 0, 1);
    sem_init(&items, 0, 0);

    pthread_t prod;
    pthread_t *cons = malloc(sizeof(pthread_t) * C);

    pthread_create(&prod, NULL, produtor, NULL);
    for (int i = 0; i < C; ++i) {
        int *id = malloc(sizeof(int));
        *id = i;
        pthread_create(&cons[i], NULL, consumidor, id);
    }

    pthread_join(prod, NULL);
    for (int i = 0; i < C; ++i) pthread_join(cons[i], NULL);

    int total_primos = 0;
    int vencedor = 0;
    for (int i = 0; i < C; ++i) {
        total_primos += primos[i];
        if (primos[i] > primos[vencedor]) vencedor = i;
    }

    printf("\nTotal de primos encontrados: %d\n", total_primos);
    printf("Consumidora vencedora: %d  quatidade de primos encontrados = %d\n", vencedor, primos[vencedor]);

    free(buffer);
    free(primos);
    free(cons);
    sem_destroy(&buffer_vazio);
    sem_destroy(&items);
    pthread_mutex_destroy(&buf_mutex);
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&quat_primos_mutex);

    return 0;
}
