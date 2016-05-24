/*
 * parserFunciones.h
 *
 *  Created on: 17/5/2016
 *      Author: utnso
 */

#ifndef PARSERFUNCIONES_H_
#define PARSERFUNCIONES_H_

#include <parser/parser.h>

#include <stdio.h>

	t_puntero dummy_definirVariable(t_nombre_variable variable);

	t_puntero dummy_obtenerPosicionVariable(t_nombre_variable variable);
	t_valor_variable dummy_dereferenciar(t_puntero puntero);
	void dummy_asignar(t_puntero puntero, t_valor_variable variable);

	void dummy_imprimir(t_valor_variable valor);
	void dummy_imprimirTexto(char* texto);

#endif /* PARSERFUNCIONES_H_ */
