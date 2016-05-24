/*
 * cpuConfig.c
 *
 *  Created on: 17/5/2016
 *      Author: utnso
 */

#include "cpuConfig.h"

t_config_cpu* create_config_cpu()
{
	t_config_cpu *config = malloc(sizeof(t_config_cpu));
	config->sem_ids = string_new();
	config->io_ids = string_new();
	config->io_sleep = string_new();
	config->sem_init =string_new();
	config->shared_vars = string_new();
	return config;
}
void destroy_config_cpu(t_config_cpu* config)
{
	free(config->sem_ids);
	free(config->io_ids);
	free(config->io_sleep);
	free(config->sem_init);
	free(config->shared_vars);
	free(config);
}


void cargarConfigCPU(t_config* configuracionCPU) {
	configCPU = create_config_cpu();
	 string_append(&configCPU->sem_ids,config_get_string_value(configuracionCPU,
			"SEM_ID"));
	 string_append(&configCPU->io_ids,config_get_string_value(configuracionCPU,
	 			"IO_ID"));
	 string_append(&configCPU->io_sleep,config_get_string_value(configuracionCPU,
	 	 			"IO_SLEEP"));
	 string_append(&configCPU->sem_init,config_get_string_value(configuracionCPU,
	 	 			"SEM_INIT"));
	 string_append(&configCPU->shared_vars,config_get_string_value(configuracionCPU,
	 	 			"SHARED_VARS"));
	 configCPU->puerto_prog = config_get_int_value(configuracionCPU,
				"PUERTO_PROG");
	 configCPU->puerto_cpu = config_get_int_value(configuracionCPU,
	 				"PUERTO_CPU");
	 configCPU->quantum = config_get_int_value(configuracionCPU,
	 	 				"QUANTUM");
	 configCPU->quantum_sleep = config_get_int_value(configuracionCPU,
	 	 				"QUANTUM_SLEEP");


}

