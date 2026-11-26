#ifndef TECLADO_NUMERICO_H
#define TECLADO_NUMERICO_H

#include <stdint.h>

/**
 * @brief Inicializa el teclado.
 */
void teclado_init(void);


/**
 * @brief Espera hasta que se pulse una tecla.
 * 
 * @return char Caracter ASCII de la tecla pulsada.
 */
char teclado_getch(void);


/**
 * @brief Espera hasta que sepulse una tecla o se agote el tiempo. 
 * 
 * @param ms Tiempo básico de espera en milisegundos.
 * @return char Caracter ASCII de la tecla o 'T' si se agota el tiempo.
 */
char teclado_getch_timeout(uint32_t ms);

#endif // TECLADO_NUMERICO_H