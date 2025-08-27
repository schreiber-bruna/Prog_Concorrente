#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h> 
#include <math.h>
#include <time.h>


float *vet;


typedef struct {
   long int n;     
   short int nthreads;
   short int id;      
} t_args;  


void *SomaVetor (void *args) {
  t_args *arg = (t_args*) args;
  int ini, fim, t;
  float soma_local=0, *ret;
  
  t = arg->n / arg->nthreads;
  ini = arg->id * t;
  fim = ini + t;
  if (arg->id == (arg->nthreads-1)) fim = arg->n;

  for(int i=ini; i<fim; i++) {
     soma_local += vet[i];
  }

  ret = (float*) malloc(sizeof(float));
  if (ret!=NULL) *ret = soma_local;
  else printf("--ERRO: malloc() thread\n");

  pthread_exit((void*) ret);
}


double executa_uma_vez(char *arquivo, int nthreads) {
    long int n;
    FILE *arq;
    size_t ret;
    double soma_original;
    float soma_global;
    float *soma_retorno_threads;
    pthread_t *tid_sistema;

    arq = fopen(arquivo, "rb");
    if(arq==NULL) { printf("--ERRO: fopen()\n"); exit(-1); }


    ret = fread(&n, sizeof(long int), 1, arq);
    if(!ret) { fprintf(stderr, "Erro de leitura da dimensao\n"); exit(-1); }


    vet = (float*) malloc (sizeof(float) * n);
    if(vet==NULL) { printf("--ERRO: malloc()\n"); exit(-1); }
    ret = fread(vet, sizeof(float), n, arq);
    if(ret < n) { fprintf(stderr, "Erro leitura elementos\n"); exit(-1); }

    if(nthreads > n) nthreads = n;

    tid_sistema = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
    if(tid_sistema==NULL) { printf("--ERRO: malloc()\n"); exit(-1); }

    clock_t start, end;
    start = clock();

    
    for(long int i=0; i<nthreads; i++) {
        t_args *args = (t_args*) malloc(sizeof(t_args));
        args->n = n;
        args->nthreads = nthreads;
        args->id = i;
        if (pthread_create(&tid_sistema[i], NULL, SomaVetor, (void*) args)) {
            printf("--ERRO: pthread_create()\n"); exit(-1);
        }
    }

    
    soma_global=0;
    for(int i=0; i<nthreads; i++) {
        if (pthread_join(tid_sistema[i], (void *) &soma_retorno_threads)) {
            printf("--ERRO: pthread_join()\n"); exit(-1);
        }
        soma_global += *soma_retorno_threads;
        free(soma_retorno_threads);
    }

    end = clock();
    double tempo = (double)(end - start) / CLOCKS_PER_SEC;

    ret = fread(&soma_original, sizeof(double), 1, arq);

    
    free(vet);
    free(tid_sistema);
    fclose(arq);

    return end - start;
}

int main(int argc, char *argv[]) {
    if(argc < 3) { 
        printf("Uso: %s <arquivo de entrada> <nthreads>\n", argv[0]); 
        return -1; 
    }

    char *arquivo = argv[1];
    int nthreads = atoi(argv[2]);
    int repeticoes = 5; 
    double tempos[5];

    double soma = 0.0;
    for(int i=0; i<repeticoes; i++) {
        tempos[i] = executa_uma_vez(arquivo, nthreads);
        soma += tempos[i];
    }

    double media = soma / repeticoes;
    double var = 0.0;
    for(int i=0; i<repeticoes; i++) {
        var += (tempos[i] - media) * (tempos[i] - media);
    }
    var /= repeticoes;
    double desvio = sqrt(var);
    double variacao_rel = desvio / media;

    printf("\nArquivo: %s | Threads: %d\n", arquivo, nthreads);
    printf("Tempos (s): ");
    for(int i=0; i<repeticoes; i++) printf("%.6f ", tempos[i]);
    printf("\nMédia: %.6f s | Desvio: %.6f s | Variação relativa: %.2f%%\n",
           media, desvio, variacao_rel*100);

    return 0;
}
