/**
 * @file teclado.h
 * @brief Librería para el manejo del teclado matricial 4x3.
 */

#ifndef TECLADO_NUMERICO_H
#define TECLADO_NUMERICO_H

#include <types.h>
#include "gpio.h"
#include "timer.h"

/** @defgroup teclado Teclado matricial

  @brief Funciones para leer las pulsaciones del teclado matricial 4x3.

  Las filas se conectan como salidas y las columnas como entradas con
  pull-up en el puerto H. Se esperan 20 ms tras cada pulsación y
  liberación para eliminar los rebotes.

  Requiere haber llamado a timer_init() antes de usar el teclado.

  @author Alba Hidalgo Martín, Xuan Sun, Juhi Kamal Chatani Mansukhani
  @date 26/11/2026

  @{
*/

/**
 * @brief Inicializa el teclado.
 *
 * Configura las filas como salidas, las columnas como entradas con
 * pull-up y deja todas las filas a 0.
 */
void teclado_init(void);


/**
 * @brief Espera hasta que se pulse una tecla.
 *
 * Si sigue pulsada la tecla anterior, espera primero a que se suelte.
 *
 * @return char Carácter ASCII de la tecla pulsada ('0' - '9', '*' o '#').
 */
char teclado_getch(void);


/**
 * @brief Espera hasta que se pulse una tecla o se agote el tiempo.
 *
 * @param ms Tiempo máximo de espera en milisegundos (0 = sin límite).
 * @return char Carácter ASCII de la tecla pulsada, o 'T' si se agota
 * el tiempo.
 */
char teclado_getch_timeout(uint32_t ms);

/**@}*/

#endif // TECLADO_NUMERICO_H