/* EXPLICAÇÃO SOBRE Os MECANISMOs AO FINAL DO CÓDIGO*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define NUM_TIMES 5
#define UPPER_EDGE 40
#define LOWER_EDGE -10

int counter = 0; // counter for how many times the program will be executed 
float temperatura = 600; // value of the temperature motor's in Celsius 

//pthread_barrier_t my_barrier;

void* increase_temperature (void* increase_temp_arg) {
    int i = (intptr_t) increase_temp_arg;

    while (counter < 5) { // while not verify five times
        int increase_temp = (rand() % (UPPER_EDGE - LOWER_EDGE + 1)) + LOWER_EDGE; // variable that generates a random value between -10 and 40 to increase the motor temperature

        printf ("Thread #%d increased the temperature in %d Celsius degrees. \n", i, increase_temp);
        temperatura += increase_temp; // put the value randomically generated in the 'temperatura'
        printf ("New temperature: %.2f. \n", temperatura);
        usleep (30000);  // sleep for 300ms
    }

    pthread_exit (0);
    return NULL;
}

void* decrease_temperature (void* decrease_temp_arg) {
    int cool = 0; // variable that will count how many time the motor was cooled

    while (counter < 5) { // while not verify five times
        if (temperatura > 1000) {
            puts ("Motor's temperature above 1000 Celsius degrees. Cooling the motor...");
            temperatura = 600;
            counter++;
            printf ("Temperature was set back to %.2f. \n\n", temperatura);
            cool++;
        }
    }

    printf ("The motor was cooled %d times. \n", cool); // How many times the motor was cooled

    pthread_exit (0);
    return NULL;
}

int main (void) {
    srand(time(NULL));

    pthread_t thread_increase;
    pthread_t thread_decrease;

    // pthread_barrier_init (&my_barrier, NULL, NUM_TIMES+1);
    pthread_create (&thread_increase, NULL, increase_temperature, NULL);
    pthread_create (&thread_decrease, NULL, decrease_temperature, NULL);

    pthread_join (thread_increase, NULL);
    pthread_join (thread_decrease, NULL);
    
   // pthread_barrier_wait (&my_barrier);

    return 0;
}

/*
    A mudança ocorre de tal maneira que a função responsável por aumentar a temperatura
    faça com que o motor chegue a uma temperatura acima de 1000 graus C, mandando um sinal(flag) de maneira imediata para que a thread responsável por diminuir
    a temperatura faça seu respectivo objetivo, utilizando o mecanismo de comunicação de memória compartilhada. Caso esse método não fosse possível, seria viável a
    utilização do mecanismo de comunicação de troca de mensagens.
*/