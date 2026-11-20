/* timer.h -- Librería de temporización para el 68HC12 (placa ADAPT912DT60)

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


void sieteSeg_init();
void sieteSeg_digitos(uint8_t* array);
void sietesSeg_valor(uint16_t decimal);

#endif //SIETESEG_H