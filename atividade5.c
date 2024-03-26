// Nome: Matheus da Cruz Percine Pinto
// DRE: 121068501

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 108  // Tamanho N do vetor
#define MTHREADS 5  // Número M de threads
int vetor[N];  // Vetor global a ser processado pelas threads

typedef struct {
    int thread_id;
    int inicio;
    int final;
} t_Args;

void *ElevarAoQuadrado(void *arg) {
    t_Args args = *(t_Args *)arg;
    for (int i = args.inicio; i < args.final; i++) {
        vetor[i] = vetor[i] * vetor[i];
    }
    free(arg);
    pthread_exit(NULL);
}

// Inicializando vetor
void InicializarVetor(){
	for (int i = 0; i < N; i++) {
		vetor[i] = i;
	}
}

//Verifica se os valores finais do vetor estão corretos.
void VerificarValores() {

	for (int i = 0; i < N; i++) {
		if (vetor[i] != (i) * (i)) {
			printf("Apos verificacao, valores finais do vetor estao incorretos!\n");
			return;
		}
	}
	printf("Apos verificacao, valores finais do vetor estao corretos!\n");
}

int main() {
    pthread_t tid[MTHREADS];
    t_Args args[MTHREADS];
    int elementos_por_thread = N / MTHREADS;  // Número de elementos por thread
    int elementos_extras = N % MTHREADS;  // Número de elementos em excesso a serem tratados
    int inicio = 0;
    int final = 0;

    InicializarVetor();

    // Criar threads
    for (int i = 0; i < MTHREADS; i++) {
        printf("--Cria a thread %d\n", i);
        args[i].thread_id = i;
        args[i].inicio = inicio;
        final = inicio + elementos_por_thread + (i < elementos_extras ? 1 : 0);
        args[i].final = final;

        if (pthread_create(&tid[i], NULL, ElevarAoQuadrado, (void *)&args[i]) != 0) {
			printf("--ERRO: pthread_create()\n");
			exit(-1);
        }

        inicio = final;
    }

    // Aguardar todas as threads terminarem
    //espera as threads terminarem
    for (int i=0; i<MTHREADS; i++) {
        if (pthread_join(tid[i], NULL)) {
            printf("ERRO -- pthread_create\n");
        }
    }

    // Imprimir o vetor resultante (apenas para fins de demonstração)
    for (int i = 0; i < N; i++) {
        printf("%d ", vetor[i]);
    }
    printf("\n");

    VerificarValores();

	printf("Thread principal terminou\n");

	pthread_exit(NULL); // desvincula termino da main com termino do programa

    return 0;
}
