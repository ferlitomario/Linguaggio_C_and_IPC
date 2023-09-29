/*Scrivere un programma in C in cui il main thread genera 2 thread secondari (T1, T2).
Tutti i thread condividono una struct con tre variabili:
- nome, di tipo stringa di 30 caratteri, inizializzata dal main thread a stringa vuota.
cognome, di tipo stringa di 30 caratteri, inizializzata dal main thread a stringa vuota.
- buffer, di tipo stringa di 60 caratteri, inizializzata dal main thread a stringa vuota.
Il main thread, che fa da scrittore, ciclicamente:
1. Controlla che le tre variabili siano tutte e tre stringhe vuote
2. Se non lo sono, attende 1 secondo e torna al punto 1, se invece lo sono:
a. Chiede all'utente di inserire un nome e lo assegna alla variabile nome
b.
Chiede all'utente di inserire un cognome e lo assegna alla variabile cognome
c. Se la variabile nome è uguale a "esci" attende la terminazione dei thread secondari e termina esso
stesso, altrimenti
i. attende un secondo e torna al punto 1.
Il thread T1, ciclicamente:
1. Controlla che le variabili nome e cognome siano stringhe vuote
2. Se lo sono, attende 1 secondo e torna al punto 1, se non lo sono:
Se nome è uguale a "esci" termina, altrimenti:
Concatena le due variabili nome e cognome nella variabile buffer e stampa buffer a video ii. Assegna sia a nome che a cognome la stringa vuota
iii.
Attende 1 secondo e torna al punto 1
Il thread T2, dopo aver aperto un file di testo (chiamato daticompito.txt) in scrittura, ciclicamente:
1. Controlla se la variabile nome è uguale a "esci", se si, chiude il file e termina, altrimenti:
Controlla che la variabile buffer sia una stringa vuota
b. Se lo è, attende I secondo e torna al punto 1, se non lo è:
i. Scrive il valore della variabile buffer sul file di testo ii.
Assegna alla variabile buffer la stringa vuota
ili.
Attende 1 secondo e torna al punto 1
Utilizzare le system call di Linux (open, write) per scrivere su file e utilizzare i pthread sezione critica. Le sleep vanno sempre fuori la sezione critica.
_mutex per l'accesso alla*/




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_NAME_LEN 30
#define MAX_BUFFER_LEN 60

struct shared_data {
    char name[MAX_NAME_LEN + 1];
    char surname[MAX_NAME_LEN + 1];
    char buffer[MAX_BUFFER_LEN + 1];
    pthread_mutex_t mutex;
};

void* thread1(void* arg) {
    struct shared_data* data = (struct shared_data*)arg;
    while (1) {
        pthread_mutex_lock(&data->mutex);
        if (strlen(data->name) == 0 && strlen(data->surname) == 0) {
            pthread_mutex_unlock(&data->mutex);
            sleep(1);
            continue;
        }
        if (strcmp(data->name, "esci") == 0) {
            pthread_mutex_unlock(&data->mutex);
            break;
        }
        sprintf(data->buffer, "%s %s", data->name, data->surname);
        printf("%s\n", data->buffer);
        data->name[0] = '\0';
        data->surname[0] = '\0';
        pthread_mutex_unlock(&data->mutex);
        sleep(1);
    }
    printf("Thread 1 terminato\n");
    return NULL;
}

void* thread2(void* arg) {
    struct shared_data* data = (struct shared_data*)arg;
    FILE* fp = fopen("daticompito.txt", "w");
    while (1) {
        pthread_mutex_lock(&data->mutex);
        if (strcmp(data->name, "esci") == 0) {
            pthread_mutex_unlock(&data->mutex);
            break;
        }
        if (strlen(data->buffer) == 0) {
            pthread_mutex_unlock(&data->mutex);
            sleep(1);
            continue;
        }
        fprintf(fp, "%s\n", data->buffer);
        fflush(fp);
        data->buffer[0] = '\0';
        pthread_mutex_unlock(&data->mutex);
        sleep(1);
    }
    fclose(fp);
    printf("Thread 2 terminato\n");
    return NULL;
}

int main() {
    struct shared_data data;
    data.name[0] = '\0';
    data.surname[0] = '\0';
    data.buffer[0] = '\0';
    pthread_mutex_init(&data.mutex, NULL);

    pthread_t t1, t2;
    pthread_create(&t1, NULL, thread1, &data);
    pthread_create(&t2, NULL, thread2, &data);

    while (1) {
        pthread_mutex_lock(&data.mutex);
        if (strlen(data.name) != 0 || strlen(data.surname) != 0) {
            if (strcmp(data.name, "esci") == 0) {
                pthread_mutex_unlock(&data.mutex);
                break;
            }
            printf("Inserire nome: ");
            scanf("%s", data.name);
            printf("Inserire cognome: ");
            scanf("%s", data.surname);
        }
        pthread_mutex_unlock(&data.mutex);
        sleep(1);
    }

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_mutex_destroy(&data.mutex);

    printf("Programma terminato\n");

    return 0;
}
