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


 #include <unistd.h>
 #include <stdlib.h>
 #include <stdio.h>
 #include <pthread.h>
 #include <errno.h>
 #include <string.h>
 #include <sys/socket.h>
 #include <sys/types.h>
 #include <netinet/in.h>
 #include <netinet/ip.h>
 #include <arpa/inet.h> 


#define PORTA 9877


int main () {

    int csockfd;
    struct sockaddr_in server_address;
    ssize_t len;
    char rx[50];
    char tx[50];

    server_address.sin_family = AF_INET;
 	server_address.sin_port = htons(PORTA); 
 	server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    csockfd = socket(AF_INET, SOCK_STREAM,0);

    connect(csockfd,(struct sockaddr *)&server_address, sizeof(server_address));

    // Riceve una stringa dal server e la stampa a video (Inserisci nome)
    read(csockfd,rx, 50);
    printf("%s",rx);

    // Legge una stringa (nome) da tastiera e la invia al server
    fgets(tx, 50, stdin);
    write(csockfd, tx, strlen(tx) +1);

    // Riceve una stringa dal server e la stampa a video (inserisci cognome)
    read(csockfd,rx, 50);
    printf("%s",rx);

    // Legge una stringa (cognome) da tastiera e la invia al server
    fgets(tx,50,stdin);
    write(csockfd,tx,strlen(tx)+1);

    // Riceve una stringa dal server e la stampa a video
    read(csockfd,rx,50);
    printf("Ho ricevuto dal server la stringa:%s\n",rx);

    // Chiude la connessione e termina
    close(csockfd);

    exit(EXIT_SUCCESS);


}