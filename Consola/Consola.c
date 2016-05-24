#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#define IP "127.0.0.1"
#define PUERTO "9036"
#define PACKAGESIZE 1024	// Define cual va a ser el size maximo del paquete a enviar
char buf[256];

int main(){


 caller();

	return 0;

}


int caller (void){

    fd_set master;    // master file descriptor list
    fd_set read_fds;  // temp file descriptor list for select()
    FD_ZERO(&master);    // clear the master and temp sets
    FD_ZERO(&read_fds);

	struct addrinfo hints;
	struct addrinfo *serverInfo;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;		// Permite que la maquina se encargue de verificar si usamos IPv4 o IPv6
	hints.ai_socktype = SOCK_STREAM;	// Indica que usaremos el protocolo TCP

	getaddrinfo(IP, PUERTO, &hints, &serverInfo);	// Carga en serverInfo los datos de la conexion

	int serverSocket;
	serverSocket = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol) ;
	if (serverSocket == -1) {
		perror(socket);
		exit(1);
	}

	if (connect(serverSocket, serverInfo->ai_addr, serverInfo->ai_addrlen) == -1) {
		perror(connect);
		exit(1);
	}
	freeaddrinfo(serverInfo);	// No lo necesitamos mas

	int enviar = 1;
	int socketUsed;
	char message[PACKAGESIZE];

	printf("Conectado al servidor. Bienvenido al sistema, ya puede enviar mensajes. Escriba 'exit' para salir\n");
	FD_SET(serverSocket, &master);
    FD_SET(fileno(stdin), &master);
	while(enviar){
        read_fds = master; // copy it
        if (select(serverSocket+1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(4);
        }
        if (FD_ISSET(fileno(stdin), &read_fds)) {

        		fgets(message, PACKAGESIZE, stdin);			// Lee una linea en el stdin (lo que escribimos en la consola) hasta encontrar un \n (y lo incluye) o llegar a PACKAGESIZE.
        		if (!strcmp(message,"exit\n")) enviar = 0;			// Chequeo que el usuario no quiera salir
        		if (enviar) send(serverSocket, message, strlen(message) + 1, 0); 	// Solo envio si el usuario no quiere salir.
        }

         if (FD_ISSET(serverSocket, &read_fds)) {
        		int nbytes = read(serverSocket, buf, sizeof buf);
        		printf("message from server %s",buf);

         }
        }



	close(serverSocket);
	return(0);
}
