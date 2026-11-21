/* sieteseg.h -- Librería para el manejo de los 4 displays de 7 
   segmentos de la placa ADAPT912DT60

  Autores: Alba Hidalgo Martín, Xuan Sun, Juhi Kamal Chatani Mansukhani
  Fecha: 19/11/2026
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


/* Inicializa los displays de 7 segmentos.

   Configura los pines del puerto utilizados por los displays, 
   inicializa el temporizador y programa el refresco periódico.
*/
void sieteSeg_init(void);


/* Muestra cuatro dígitos en los displays de 7 segmentos.
   
   Recibe un puntero a un array de al menos cuatro bytes. Cada posición
   contiene el valor que se enseñará en el display correspondiente.

   El array debe mantenerse válido mientras se esté mostrando.

   @param array puntero al array con los cuatro dígitos.
*/
void sieteSeg_digitos(uint8_t* array);

/* Muestra un número decimal en los display de 7 segmentos.

   Separa el número en sus cuatro últimas cifras y las enseña en los 
   displays. Si tiene menos de cuatro cifras, completa con ceros por la
   izquierda.

   @param decimal número decimal que se quiere mostrar.
*/
void sietesSeg_valor(uint16_t decimal);

#endif //SIETESEG_H