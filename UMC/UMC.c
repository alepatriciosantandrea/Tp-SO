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
#include "UMC.h"
#include "Swap.h"
#include <pthread.h>



// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}




int main(){

	int socketSwap, socketNucleo;
	int socketsCPU[50];
	char* memDinamica;
	//memDinamica = malloc (10); // Asigno y limpio mi memoria dinámica.

	//Conectarme a la UMC
	connectToSwap(&socketSwap);
	//Aceptar nuevas conexiones
	escucharNuevasConexiones(&socketsCPU,&socketNucleo,&socketSwap);

	return 0;

}



void* atenderCPU (void *puertoSWAP)
{
	t_parametrosCPU *parametros;
	parametros = (t_parametrosCPU*) puertoSWAP;
 printf ("%d",parametros->parametro);
	return NULL;

}

void* iniciarPrograma (int idPrograma, int pagRequeridas){
	//Allocar memoria
	//Comunicar a SWAP que inicie el programa
	return NULL;
}

void* finalizarPrograma (int idPrograma){
	//Comunicar a SWAP que finalice el programa.
	return NULL;
}

void* escucharNuevasConexiones (int *socketsCPU, int *socketNucleo, int *socketSWAP){

	struct addrinfo hints;
	struct addrinfo *serverInfo;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC; // No importa si uso IPv4 o IPv6
	hints.ai_flags = AI_PASSIVE; // Asigna el address del localhost: 127.0.0.1
	hints.ai_socktype = SOCK_STREAM; // Indica que usaremos el protocolo TCP
	getaddrinfo(NULL, PUERTO, &hints, &serverInfo); // Notar que le pasamos NULL como IP, ya que le indicamos que use localhost en AI_PASSIVE
	pthread_t threadID;
	t_parametrosCPU parametrosCPU;


	int listenningSocket;
	listenningSocket = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);


	bind(listenningSocket,serverInfo->ai_addr, serverInfo->ai_addrlen);
	freeaddrinfo(serverInfo); // Ya no lo vamos a necesitar

	for(int i=0;;i++){

	listen(listenningSocket, BACKLOG); // IMPORTANTE: listen() es una syscall BLOQUEANTE.


	struct sockaddr_in addr; // Esta estructura contendra los datos de la conexion del cliente. IP, puerto, etc.
	socklen_t addrlen = sizeof(addr);
	int socketCliente = accept(listenningSocket, (struct sockaddr *) &addr, &addrlen);

	char tipoCliente[20];
	recv(socketCliente, tipoCliente, sizeof tipoCliente,0);

	if (tipoCliente == "CPU"){
	socketsCPU[i] = socketCliente;
	}
	else
	{
		socketNucleo = socketCliente;
	}

	//Crear nuevo hilo
	pthread_create(&threadID,NULL,&atenderCPU,&parametrosCPU);
	pthread_join(threadID,NULL);
	}
	return NULL;

}
