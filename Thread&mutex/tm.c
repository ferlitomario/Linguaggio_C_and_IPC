/*Scrivere un programma multithread in C su Linux in cui il main thread (MT) genera tre thread secondari (TI, T2 e T3).
Tutti i thread condividono una stringa di 30 caratteri e una variabile di tipo int inizializzata a 0.
Il main thread, ciclicamente, per 25 volte, doo aver generato i tre thread secondari:
• controlla che la variabile intera condivisa sia guale a 1
o se lo è, stampa a video la stringa condivisa e imposta il valore della variabile intera condivisa a 0
• va in sleep per 1 secondo
Al termine dei 25 cicli il main thread cancella i thread secondari, ne attende la terminazione e termina esso stesso.
Ogni thread secondario, ciclicamente:
• controlla che la variabile intera condivisa sia uguale a 0
• se lo è, chiede all'utente di inserire la stringa condivisa da tastiera ed imposta il valore della variabi intera condivisa a 1
• va in sleep per 1 secondo
Utilizzare il meccanismo dei pthread mutex per l'accesso in sezione critica.*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>
#include <string.h>

 char STRINGA[30];
int variabile_condivisa = 0;

pthread_mutex_t mutex;

void* Thread_func(void *args);

int main () {

    int i;
    pthread_t T1,T2,T3;

    pthread_mutex_init(&mutex,NULL);

   pthread_create(&T1,NULL,&Thread_func,NULL );
   pthread_create(&T2,NULL,&Thread_func,NULL );
   pthread_create(&T3,NULL,&Thread_func,NULL );


   for(i=0;i<25;i++) {
    pthread_mutex_lock(&mutex);
    if(variabile_condivisa == 1) {
        printf("Stampo a video la stringa:%s" , STRINGA);
        variabile_condivisa = 0;

        sleep(1);
    }
    pthread_mutex_unlock(&mutex);
   }

   pthread_join(T1,NULL);
   pthread_join(T2,NULL);
   pthread_join(T3,NULL);

   printf("Main thread terminato\n");

   exit(0);

}

void* Thread_func(void *args) {

pthread_mutex_lock(&mutex);

if(variabile_condivisa == 0) {
    printf("Inserisci una stringa da tastiera:\n");
    fgets(STRINGA, 30, stdin);
    variabile_condivisa = 1;

}

pthread_mutex_unlock(&mutex);
sleep(1);

printf("Funzione Thread terminata\n");

};