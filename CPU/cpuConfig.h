/*
 * cpu.h
 *
 *  Created on: 17/5/2016
 *      Author: utnso
 */

#ifndef CPUCONFIG_H_
#define CPUCONFIG_H_

#include <commons/config.h>
#include <commons/string.h>

typedef struct
{

	int puerto_prog;
	int puerto_cpu;
	int quantum;
	int quantum_sleep;
	char* sem_ids;
	char* sem_init;
	char* io_ids;
	char* io_sleep;
	char* shared_vars;
}t_config_cpu;

t_config_cpu* configCPU;
/* crea la configuracion con sus parametros */
t_config_cpu* create_config_cpu();

/* libera la memoria de la estructura */
void destroy_config_cpu(t_config_cpu* config);

/* carga los parametros del archivo*/
void cargarConfigCPU(t_config* configuracionCPU);
#endif /* CPUCONFIG_H_ */
