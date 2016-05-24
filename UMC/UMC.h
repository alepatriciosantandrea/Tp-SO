/*
 * UMC.h
 *
 *  Created on: 18/5/2016
 *      Author: utnso
 */

#ifndef UMC_H_
#define UMC_H_

#define IP "127.0.0.1"
#define PUERTO "9035" //Puerto de la SWAP a donde se conecta la UMC
#define PACKAGESIZE 1024	// Define cual va a ser el size maximo del paquete a enviar
#define PORT "9034"   // port we're listening on
#define BACKLOG 5

void *atenderCPU (void *puertoSWAP);

typedef struct t_parametrosCPU {
	int parametro;
}t_parametrosCPU;

#endif /* UMC_H_ */
