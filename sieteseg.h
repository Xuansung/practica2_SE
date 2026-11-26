/**
 * @file sieteseg.h
 * @brief Librería para el manejo de los 4 displays de 7 segmentos de la
 * placa ADAPT912DT60.
 */

#ifndef SIETESEG_H
#define SIETESEG_H

#include <types.h>
#include <sys/param.h>
#include <sys/interrupts.h>
#include <sys/sio.h>
#include <sys/locks.h>
#include "timer.h"
#include "gpio.h"

/** @defgroup sieteseg Displays de 7 segmentos

  @brief Funciones para mostrar dígitos y números decimales en los
  4 displays de 7 segmentos.

  Los 4 displays comparten un único decodificador, por lo que solo se
  puede encender uno en cada instante. Se utiliza multiplexación
  temporal: una función periódica del temporizador enciende un display
  en cada ejecución, por turnos, con una frecuencia de refresco
  superior a 100 Hz.

  Recursos que utiliza:
  - El puerto T completo como salida.
  - Un canal del temporizador para el refresco periódico.

  
  @author Alba Hidalgo Martín, Xuan Sun, Juhi Kamal Chatani Mansukhani
  @date 19/11/2026

  @{
*/

/**
 * @brief Inicializa los displays de 7 segmentos.
 *
 * Configura como salida los pines del puerto utilizados por los
 * displays, inicializa el temporizador y programa el refresco
 * periódico. Al empezar se muestra 1234.
 *
 * @note Llama a timer_init(), por lo que cancela cualquier función de
 * timeout o periódica programada antes. Debe invocarse antes de
 * programar otras funciones del temporizador.
 */
void sieteSeg_init(void);

/**
 * @brief Muestra cuatro dígitos en los displays de 7 segmentos.
 *
 * Recibe un puntero a un array de al menos cuatro bytes. Cada posición
 * contiene el valor (0 - 9) que se enseñará en el display
 * correspondiente.
 * IMPORTANTE: el array que se pasa a sieteSeg_digitos() debe ir en
 * orden INVERTIDO. La posición 0 se muestra en el display de la
 * derecha (unidades) y la posición 3 en el de la izquierda (millares).
 * Para que se vea 1234 hay que pasar {4, 3, 2, 1}.

 * @param array puntero al array con los cuatro dígitos.
 *
 * @note El array debe mantenerse válido mientras se esté mostrando.
 */
void sieteSeg_digitos(uint8_t* array);

/**
 * @brief Muestra un número decimal en los displays de 7 segmentos.
 *
 * Separa el número en sus cuatro últimas cifras y las enseña en los
 * displays. Los ceros a la izquierda no se muestran (el display queda
 * apagado), salvo el de las unidades, de modo que el 0 se ve como "0".
 *
 * @param decimal número decimal que se quiere mostrar (si es mayor
 * que 9999 se muestran sus cuatro últimas cifras).
 */
void sietesSeg_valor(uint16_t decimal);

/**@}*/

#endif // SIETESEG_H