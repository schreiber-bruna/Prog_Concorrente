#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

float *a, *b;
long int n;
int nthreads;

typedef struct {
   int id;
} t_args;

void *tarefa(void *arg) {
   t_args *args = (t_args*) arg;
   int id = args->id;
   long int ini, fim, bloco;
   double *soma_local = malloc(sizeof(double));
   if(!soma_local) {
      fprintf(stderr, "Erro malloc thread\n");
      pthread_exit(NULL);
   }
   *soma_local = 0.0;

   bloco = n / nthreads;
   ini = id * bloco;
   fim = (id == nthreads-1) ? n : ini + bloco;

   for(long int i=ini; i<fim; i++) {
      *soma_local += (double)a[i] * (double)b[i];
   }

   free(args);
   pthread_exit((void*) soma_local);
}

int main(int argc, char *argv[]) {
   if(argc < 3) {
      printf("Use: %s <arquivo entrada> <nthreads>\n", argv[0]);
      return 1;
   }

   FILE *arq = fopen(argv[1], "rb");
   if(!arq) {
      printf("Erro fopen\n");
      return 2;
   }

   fread(&n, sizeof(long int), 1, arq);
   a = (float*) malloc(sizeof(float) * n);
   b = (float*) malloc(sizeof(float) * n);
   fread(a, sizeof(float), n, arq);
   fread(b, sizeof(float), n, arq);

   double d_seq;
   fread(&d_seq, sizeof(double), 1, arq);
   fclose(arq);

   nthreads = atoi(argv[2]);
   if(nthreads > n) nthreads = n;

   pthread_t *tid = malloc(sizeof(pthread_t)*nthreads);

   for(int i=0; i<nthreads; i++) {
      t_args *args = malloc(sizeof(t_args));
      args->id = i;
      pthread_create(&tid[i], NULL, tarefa, (void*) args);
   }

   double d_conc = 0.0;
   for(int i=0; i<nthreads; i++) {
      double *ret;
      pthread_join(tid[i], (void**) &ret);
      d_conc += *ret;
      free(ret);
   }

   double erro_rel = fabs((d_seq - d_conc) / d_seq);

   printf("Produto interno concorrente = %.6f\n", d_conc);
   printf("Produto interno sequencial  = %.6f\n", d_seq);
   printf("Erro relativo = %.6e\n", erro_rel);

   free(a);
   free(b);
   free(tid);
   return 0;
}
