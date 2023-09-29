/*Scrivere un programma in C in cui un processo padre (P) genera n processi figli (con n inserito da tastiera).
Il processo padre, dopo aver generato i processi figli, ciclicamente per 10 iterazioni:
invia un segnale SIGUSR1 ad ogni processo figlio
va in sleep per 2 secondi
Dopo le 10 iterazioni invia un segnale SIGUSR2 a tutti i processi figli, attende la loro terminazione e termina esso stesso.
Ogni processo figlio, ciclicamente:
•
•
attende un segnale dal padre se arriva il segnale SIGUSR1
o genera un numero casuale x tra 1 e 90
• stampa a video il proprio PID e il valore del numero x generato
• se arriva il segnale SIGUSR2
• stampa a video il proprio PID
о
termina
N.B. Nei processi figli far sì che, durante la gestione del segnale SIGUSR1, l'eventuale arrivo del segnale
SIGUSR2 sia mascherato.*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>


void child_signal_handler(int sig);


void signal_handler(int sig) ;


int main () {

    int i,n;
    int stat;

    printf("inserire quanti figli creare:\n");
    scanf("%d",&n);

    struct sigaction act;
    pid_t figli[n];

    sigemptyset(&(act.sa_mask));
    sigaddset(&act.sa_mask, SIGUSR2);

    act.sa_handler=child_signal_handler;

    //Generazione figli

    for(i=0;i<n;i++) {
        figli[i] =  fork();

        switch (figli[i])
        {
        case -1:
                printf("Errore fork\n");
                exit(EXIT_FAILURE);
            break;
        
        case 0:
            act.sa_handler = child_signal_handler;

            sigaction(SIGUSR1, &act, 0);
            signal(SIGUSR2, signal_handler);

            while(1) {
                pause();
            }
            exit(0);

            default:
            break;
        }
    }

    //codice processo padre (n = figli)

    for(i=0;i<10;i++) {
        for(i=0;i<n;i++) {
            kill(figli[i], SIGUSR1);
            sleep(2);
        }
    }

    for(i=0;i<n;i++) {
        kill(figli[i],SIGUSR2);
    }

    for(i=0;i<10;i++) {
        wait(&stat);
    }

    exit(0);


}

//gestore segnali

void child_signal_handler(int sig) {

   

    if(sig == SIGUSR1) {
        int x = (rand()%90)+1;
        printf("Il figlio %d ha generato il numero %d:\n",getpid(), x);
    }else if(sig == SIGUSR2){
        printf("Arrivato il segnale SIGUSR2 quindi stampo il mio PID: %d",getpid());
        exit(0);

    }

}

//Gestore padre

void signal_handler(int sig) {
    printf("il processo avente pid %d ha terminato",getpid());
    exit(0);
}