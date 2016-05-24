#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT "9036"   // port we're listening on

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


#define IP "127.0.0.1"
#define PUERTO "9034"
#define PACKAGESIZE 1024	// Define cual va a ser el size maximo del paquete a enviar
char buf[256];

int main(){

	int socketCliente;
	 client(&socketCliente);
	 server(&socketCliente);

	return 0;

}

int server (int *clientSocket){

    fd_set master;    // master file descriptor list
    fd_set read_fds;  // temp file descriptor list for select()
    int fdmax;        // maximum file descriptor number

    int listener;     // listening socket descriptor
    int newfd;        // newly accept()ed socket descriptor
    struct sockaddr_storage remoteaddr; // client address
    socklen_t addrlen;

    char buf[256];    // buffer for client data
    int nbytes;

    char remoteIP[INET6_ADDRSTRLEN];

    int yes=1;        // for setsockopt() SO_REUSEADDR, below
    int i, rv;

    struct addrinfo hints, *ai, *p;

    FD_ZERO(&master);    // clear the master and temp sets
    FD_ZERO(&read_fds);

    // get us a socket and bind it
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; //localhost: 127.0.0.1
    if ((rv = getaddrinfo(NULL, PORT, &hints, &ai)) != 0) {
        fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
        exit(1);
    }

    for(p = ai; p != NULL; p = p->ai_next) {
        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listener < 0) {
            continue;
        }

        // lose the pesky "address already in use" error message
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
            close(listener);
            continue;
        }

        break;
    }

    // if we got here, it means we didn't get bound
    if (p == NULL) {
        fprintf(stderr, "selectserver: failed to bind\n");
        exit(2);
    }

    freeaddrinfo(ai); // all done with this

    // listen
    printf("Waiting for the first connection \n");
    if (listen(listener, 10) == -1) {
        perror("listen");
        exit(3);
    }

    // add the listener to the master set
    FD_SET(listener, &master);
    //Set STDI/O descriptor
    //FD_SET(fileno(stdin), &master);
    FD_SET(*clientSocket, &master);

    // keep track of the biggest file descriptor
    fdmax = listener; // so far, it's this one

    // main loop
    for(;;) {
        read_fds = master; // copy it
        if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(4);
        }

        // run through the existing connections looking for data to read
        for(i = 0; i <= fdmax; i++) {
            if (FD_ISSET(i, &read_fds)) { // we got one!!
                if (i == listener) {
                    // handle new connections
                    addrlen = sizeof remoteaddr;
                    newfd = accept(listener,
                        (struct sockaddr *)&remoteaddr,
                        &addrlen);

                    if (newfd == -1) {
                        perror("accept");
                    } else {
                        FD_SET(newfd, &master); // add to master set
                        if (newfd > fdmax) {    // keep track of the max
                            fdmax = newfd;
                        }
                        printf("new connection from server %s on "
                            "socket %d\n",
                            inet_ntop(remoteaddr.ss_family,
                                get_in_addr((struct sockaddr*)&remoteaddr),
                                remoteIP, INET6_ADDRSTRLEN),
                            newfd);
                    }
                } else {
                	if (i > 2) {
						// handle data from a client
						if ((nbytes = recv(i, buf, sizeof buf, 0)) <= 0) {
							// got error or connection closed by client
							if (nbytes == 0) {
								// connection closed

							/*	printf("socket %d from server %s hung up\n", i, inet_ntop(remoteaddr.ss_family,
										get_in_addr((struct sockaddr*)&remoteaddr),
										remoteIP, INET6_ADDRSTRLEN));*/
							} else {
								perror("recv");
							}
							close(i); // bye!
							FD_CLR(i, &master); // remove from master set
						}else {

							printf("message from server %s on socket %d: %s", inet_ntop(remoteaddr.ss_family,
									get_in_addr((struct sockaddr*)&remoteaddr),
									remoteIP, INET6_ADDRSTRLEN),i,buf);
							send(i+1, buf, strlen(buf) + 1, 0);


						}

					} // END handle data from client

					/*else
					{
						memset(&buf[0],0,sizeof (buf));
						nbytes = read(i, buf, sizeof buf);
						int j;
						 for(j = 0; j <= fdmax; j++) {
							 //envio a todos menos al lisener y a la consola
							 if (j>2 && j != listener) {
									send(j, buf, strlen(buf) + 1, 0);
							 }
						 }

					}*/

            }

            } // END got new incoming connection
        } // END looping through file descriptors
    } // END for(;;)--and you thought it would never end!

    return 0;

}

int client (int *serverSocket){

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


	*serverSocket = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol) ;
	if (*serverSocket == -1) {
		perror(socket);
		exit(1);
	}

	if (connect(*serverSocket, serverInfo->ai_addr, serverInfo->ai_addrlen) == -1) {
		perror(connect);
		exit(1);
	}
	freeaddrinfo(serverInfo);	// No lo necesitamos mas

	int enviar = 1;
	int socketUsed;
	char message[PACKAGESIZE];

	printf("Conectado al servidor. Bienvenido al sistema, ya puede enviar mensajes. Escriba 'exit' para salir\n");
	/*FD_SET(serverSocket, &master);
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



	close(serverSocket); */
	return(0);
}
