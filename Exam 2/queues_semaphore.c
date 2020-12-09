/* EXPLICAÇÃO SOBRE O CONTROLE DE ACESSO DOS SEMÁFOROS AO FINAL DO CÓDIGO*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define ODD_LOWER 25
#define ODD_UPPER 75
#define EVEN_LOWER 5
#define EVEN_UPPER 100
#define DIVISION 10
#define NUM_THREADS 10

float x = 0;
float y = 0;

sem_t mutex;

void* even_number_function (void* even_number_arg) {
    int t_id_i = *(int* ) even_number_arg;
    sem_wait (&mutex);

    while (x+y <= 50) {
        float random_number_x = (rand() % EVEN_UPPER - EVEN_LOWER + 1) + EVEN_LOWER;
        x += random_number_x/DIVISION;

        printf ("Thread #%d adding the value %.2f to x.\n", t_id_i, random_number_x/DIVISION);
        printf ("Current value of x: %.2f \n", x);

        usleep (2500000);

        sem_post (&mutex);
    }

    pthread_exit (0);
    return NULL;
}

void* odd_number_function (void* odd_number_arg) {
    int t_id_j = *(int*) odd_number_arg;
    sem_wait (&mutex);


    while (x+y <= 50) {
        float random_number_y = (rand() % ODD_UPPER - ODD_LOWER + 1) + ODD_LOWER;
        y += random_number_y/DIVISION;

        printf ("Thread #%d adding the value %.2f to y. \n", t_id_j, random_number_y/DIVISION);
        printf ("The cuurent value of y: %.2f \n", y);

        int random_time = (rand() % (2000000)) + 100000; // sleep for a random time between 100 and 300ms
        usleep (random_time);

        sem_post (&mutex);
    }

    pthread_exit (0);
    return NULL;
}


int main (void) {
    srand(time(NULL));
    
    pthread_t thread_ids[NUM_THREADS];
    int t_ids[NUM_THREADS];

    sem_init (&mutex, 0, 1);

    for (int i = 0; i < NUM_THREADS; ++i) {
        t_ids[i] = i;
        if (i % 2 == 0) {
            pthread_create (&thread_ids[i], NULL, even_number_function, &t_ids[i]);
         } else {
            pthread_create (&thread_ids[i], NULL, odd_number_function, &t_ids[i]);
         }
    }

    for (int j = 0; j < NUM_THREADS; ++j) {
        pthread_join (thread_ids[j], NULL);
    }

    sem_destroy (&mutex);

    printf ("\nFinal value of x: %.2f.\n", x);
    printf ("Final value of y: %.2f.\n", y);

    return 0;
}

    /*

    Utilizando o método clássico de controle de acesso à seções críticas utilizando as funções da biblioteca "semaphore.h",
    'sem_wait' e 'sem_post, respectivamente. Em ambas as seções, o uso da função 'sem_wait' realiza o controle de ambas as seções críticas, deixando estas em estado de espera.
    Já a 'sem_post', também em ambas as seções faz a liberação do acesso à seção crítica.
    
    */