/**
	Scrivere un programma in C in cui viene implementato un server concorrente e un client che 
	utilizzino i socket stream nel dominio di Internet.

	Il server una volta avviato, ciclicamente, accetta una connessione sulla porta 9877 
	e crea un worker thread (detached) che gestisce la connessione col client.

	Il worker thread
		1. trasmette al client la stringa “Inserisci nome: ”
		2. attende la ricezione di una stringa contenente il nome dal client
		3. trasmette al client la stringa “Inserisci cognome: ”
		4. attende la ricezione di una stringa contenente il cognome dal client
		5. trasmette al client la stringa “nome_cognome\n” contenente i dati ricevuti dal client
		6. Chiude la connessione col client e termina

	Il client si connette alla porta 9877 del server e:
		1. Riceve una stringa dal server e la stampa a video (Inserisci nome)
		2. Legge una stringa (nome) da tastiera e la invia al server
		3. Riceve una stringa dal server e la stampa a video (inserisci cognome)
		4. Legge una stringa (cognome) da tastiera e la invia al server
		5. Riceve una stringa dal server e la stampa a video
		6. Chiude la connessione e termina
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>



#define PORTA 9877

void *worker_thread(void *args);


// socket() -> bind() -> listen() -> accept() -> close 

int main () {

    int sockfd;
    int new_sockfd;
    struct sockaddr_in server_address;
    pthread_t tid;

    // avvio server

    server_address.sin_family = AF_INET;
 	server_address.sin_port = htons(PORTA);
 	server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    if((sockfd = socket(AF_INET,SOCK_STREAM, 0)) == -1) {
        printf("Errore creazione socket\n");
        exit(EXIT_FAILURE);
    }

    if(bind(sockfd,(struct sockaddr *) &server_address,sizeof(server_address)) == -1) {
         printf("Errore bind\n");
        exit(EXIT_FAILURE);
    }

    listen(sockfd,5);

    printf("aspettando i client...\n");

    // accept

    while(1) {
        if((new_sockfd = accept(sockfd,NULL,NULL)) == -1) {
            printf("errore accept");
            exit(EXIT_FAILURE);
        }

        printf("[SERVER] Connessione stabilita");
        pthread_create(&tid,NULL,worker_thread,(void *)&new_sockfd);

    }

exit(EXIT_SUCCESS);
 

}

void *worker_thread(void *args){

    pthread_detach(pthread_self());

    int sockfd = *(int *) args;
    char rx_nome[50];
 	char rx_cognome[50];
 	char tx_buffer[101];
    size_t len;

    // trasmette al client la stringa “Inserisci nome: ”
    write(sockfd, "Inserisci nome\n", 16);

    // attende la ricezione di una stringa contenente il nome dal client
    len = read(sockfd, rx_nome, 50);
    printf("Ho ricevuto %s:\n",rx_nome);


// trasmette al client la stringa “Inserisci cognome: ”
    write(sockfd, "Inserisci cognome:\n", 21);

    //attende la ricezione di una stringa contenente il cognome dal client
    len = read(sockfd, rx_cognome,50);
    printf("Ho ricevuto %s:\n",rx_cognome);


    // trasmette al client la stringa “nome_cognome\n” contenente i dati ricevuti dal client
    snprintf(tx_buffer,sizeof(tx_buffer), "%s_%s\n",rx_nome,rx_cognome);
    write(sockfd,tx_buffer,21);

    //Chiude la connessione col client e termina
    close(sockfd);
    pthread_exit(EXIT_SUCCESS);


}