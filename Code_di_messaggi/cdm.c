  /*Scrivere un programma C che genera un figlio e
comunica con lui usando una coda di messaggi
– il figlio legge da terminale una stringa e la invia al padre
– quando il padre riceve una stringa, invia al figlio un
messaggio contenente il numero di “a” nella stringa
– quando il figlio riceve la risposta dal padre, la mostra sul
terminale
● Assicurarsi che alla fine la coda sia cancellata */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#define MSG_SIZE 256


struct messaggi{
    long mtype;
    char msg[MSG_SIZE];
};

int main () {
    int i;
    pid_t pid;
    int msgid;
    struct messaggi msg;

    //Creazione coda di messaggi
    msgid = msgget(IPC_PRIVATE, 0666 | IPC_CREAT);

    // creazione processo figlio 
    
    pid = fork();
    if(pid < 0) {
        printf("Errore creazione fork!\n");
        exit(1);
    }else if( pid == 0) {
        printf("Utente inserire stringa:");
        fgets(msg.msg, MSG_SIZE, stdin);


        // invia il messaggio al padre
        msg.mtype = 1;
        msgsnd(msgid, &msg, strlen(msg.msg)+1,0);

        //riceve la risposta del padre
        msgrcv(msgid, &msg,MSG_SIZE,2,0);

        //mostra la risposta sul terminale 
        printf("Numero di 'a' nella stringa: %s\n", msg.msg);

        exit(0);
    } else {

        // Processo padre

        //riceve il messaggio dal figlio
         msgrcv(msgid,&msg,MSG_SIZE,1,0);


        int count = 0;
        for(i=0;i < strlen(msg.msg); i++) {
            if(msg.msg[i] == 'a' || msg.msg[i] == 'A') {
                count++;
            }
        }

        //converte il conteggio in una stringa
        snprintf(msg.msg, MSG_SIZE, "%d", count);

        //invia la risposta al figlio
        msg.mtype = 2;
        msgsnd(msgid, &msg, strlen(msg.msg) +1, 0);

        // Riceve la risposta dal figlio
        msgrcv(msgid, &msg, MSG_SIZE, 3, 0);

        // Stampa la risposta ricevuta
        printf("Numero di 'a' nella stringa: %s\n", msg.msg);

        msgctl(msgid, IPC_RMID,NULL);
    }

    return 0;
}

