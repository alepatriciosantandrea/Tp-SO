#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include "cpuConfig.h"
#include "cpuConfig.h"
#include "parserFunciones.h"


#define IP "127.0.0.1"
#define PUERTO "9036"  // Nucleo
#define PUERTO2 "9034" // UMC

#define PACKAGESIZE 1024	// Define cual va a ser el size maximo del paquete a enviar
char buf[256];


AnSISOP_funciones functions = {
		.AnSISOP_definirVariable		= dummy_definirVariable,
		.AnSISOP_obtenerPosicionVariable= dummy_obtenerPosicionVariable,
		.AnSISOP_dereferenciar			= dummy_dereferenciar,
		.AnSISOP_asignar				= dummy_asignar,
		.AnSISOP_imprimir				= dummy_imprimir,
		.AnSISOP_imprimirTexto			= dummy_imprimirTexto,

};
AnSISOP_kernel kernel_functions = { };
static const char* ASIGNACION = "a = b + 12";

int main(){

	t_config * config = config_create("/home/utnso/config.txt");

		cargarConfigCPU(config);



			printf("PuertoProg: %i\n",configCPU->puerto_prog );
			printf("PuertoCPU: %i\n",configCPU->puerto_cpu);
			printf("quantum: %i\n",configCPU->quantum);
			printf("quantum sleep: %i\n",configCPU->quantum_sleep);
			printf("semaforos ids %s\n",configCPU->sem_ids);
			printf("semaforos init %s\n",configCPU->sem_init);
			printf("io ids %s\n",configCPU->io_ids);
			printf("io sleep %s\n",configCPU->io_sleep);
			printf("shared %s\n",configCPU->shared_vars);
			destroy_config_cpu(configCPU);

			//analizadorLinea(strdup(ASIGNACION), &functions, &kernel_functions);
			//si alguien tiene el parser.h bien instalado deberia andar
			printf("================\n");
			/* Dejo comentado el llamado a caller para quien lo use que lo descomente*/
 //caller();

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

	int nucleoSocket;
	nucleoSocket = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol) ;
	if (nucleoSocket == -1) {
		perror(socket);
		exit(1);
	}

	if (connect(nucleoSocket, serverInfo->ai_addr, serverInfo->ai_addrlen) == -1) {
		perror(connect);
		exit(1);
	}
	freeaddrinfo(serverInfo);	// No lo necesitamos mas

	FD_SET(nucleoSocket, &master);

	getaddrinfo(IP, PUERTO2, &hints, &serverInfo);	// Carga en serverInfo los datos de la conexion

	int UMCSocket;
	UMCSocket = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol) ;
	if (UMCSocket == -1) {
		perror(socket);
		exit(1);
	}

	if (connect(UMCSocket, serverInfo->ai_addr, serverInfo->ai_addrlen) == -1) {
		perror(connect);
		exit(1);
	}
	freeaddrinfo(serverInfo);	// No lo necesitamos mas

	int enviar = 1;
	int socketUsed;
	char message[PACKAGESIZE];

	printf("Conectado al servidor. Bienvenido al sistema, ya puede enviar mensajes. Escriba 'exit' para salir\n");
	FD_SET(UMCSocket, &master);
    while(enviar){
        read_fds = master; // copy it
        if (select(UMCSocket+1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(4);
        }
        if (FD_ISSET(nucleoSocket, &read_fds)) {

        		int nbytes = read(nucleoSocket, buf, sizeof buf);
        		if (!strcmp(message,"exit\n")) enviar = 0;			// Chequeo que el usuario no quiera salir
        		if (enviar) send(UMCSocket, buf, strlen(buf) + 1, 0); 	// Solo envio si el usuario no quiere salir.
				printf("message : %s",buf);
        }
/*
         if (FD_ISSET(nucleoSocket, &read_fds)) {
        		int nbytes = read(nucleoSocket, buf, sizeof buf);
        		printf("message from server %s",buf);

         }*/
        }



	close(nucleoSocket);
	close(UMCSocket);
	return(0);
}
