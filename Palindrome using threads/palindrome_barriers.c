#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <string.h>

#define NUM_THREADS 6
#define STR_LENGHT 6
#define CICLES_EXEC 10

int count_char = 0;
int lock = 0;
int word_times = 0;
char gen_str[STR_LENGHT];

pthread_barrier_t my_barrier;

void* random_caracter_function (void* thread_id) {
    int t_id = (intptr_t) thread_id;

    printf ("Thread %d created sucefully \n", t_id);
    printf ("Thread %d arrived in the critical section area. \n\n",t_id);

    pthread_barrier_wait (&my_barrier);

    char random_char = 'A' + (rand() % 26); // generate a random caracter

    gen_str[count_char++] = random_char;

    printf ("I am the thread #%d and generated the caracteer %c \n\n", t_id, random_char);

    return NULL;
}

void is_palindrome_function (char* gen_str) {
    int tmp = 0;
    int k = strlen(gen_str)-1;
    count_char = 0;
    
    printf ("The string created was: %s \n", gen_str);
        for (int j = 0; j < strlen(gen_str)/2; ++j) {
            if (gen_str[j] == gen_str[k--]) {
                ++tmp;
            } else {
                puts ("The string is not a palindrome. \n");
                break;
            }
        }

    if (tmp == strlen(gen_str)/2) {
        puts ("The string is a palindrome. \n");
    }
}

int main (void) {
    srand(time(NULL));

    // char str_thread[STR_LENGHT] = {'M', 'U', 'S', 'S', 'U', 'M'};
    pthread_t thread_caracter_gen[NUM_THREADS];
        
    pthread_barrier_init (&my_barrier, NULL, NUM_THREADS+1);
            
    for (int index = 0; index < CICLES_EXEC; index++) {
        for (int i = 0; i < NUM_THREADS; ++i) {
            pthread_create (&thread_caracter_gen[i], NULL, random_caracter_function, (void*) (intptr_t) i);
        }

        pthread_barrier_wait (&my_barrier);
            
        for (int j = 0; j < NUM_THREADS; ++j) {
            pthread_join (thread_caracter_gen[j], NULL);
        }

        is_palindrome_function (gen_str);
            
        }
            
        pthread_barrier_destroy (&my_barrier);

    return 0;
}