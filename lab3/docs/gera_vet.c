#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 1000

int main(int argc, char*argv[]) {
   float *a, *b;
   long int n;
   double d = 0.0;
   FILE * descritorArquivo;
   size_t ret;

   if(argc < 3) {
      fprintf(stderr, "Digite: %s <dimensao> <nome arquivo saida>\n", argv[0]);
      return 1;
   }
   n = atol(argv[1]);

   a = (float*) malloc(sizeof(float) * n);
   b = (float*) malloc(sizeof(float) * n);
   if(!a || !b) {
      fprintf(stderr, "Erro malloc\n"); 
      return 2; 
   }

   srand(time(NULL));
   for(long int i=0; i<n; i++) {
      a[i] = (rand() % MAX) / 5.0;
      b[i] = (rand() % MAX) / 5.0;
      d += (double)a[i] * (double)b[i];
   }

   descritorArquivo = fopen(argv[2], "wb");
   if(!descritorArquivo) {
      fprintf(stderr, "Erro fopen\n"); 
      return 3;
   }

   ret = fwrite(&n, sizeof(long int), 1, descritorArquivo);
   ret = fwrite(a, sizeof(float), n, descritorArquivo);
   ret = fwrite(b, sizeof(float), n, descritorArquivo);
   ret = fwrite(&d, sizeof(double), 1, descritorArquivo);

   fclose(descritorArquivo);
   free(a); free(b);

   printf("Arquivo %s gerado (N=%ld, produto interno=%.6f)\n", argv[2], n, d);
   return 0;
}
