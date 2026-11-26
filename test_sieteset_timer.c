
/* prueba_sieteseg.c -- Programa de prueba de los displays de 7 
   segmentos.

   Autores: Alba Hidalgo Martín, Xuan Sun y Juhi Kamal Chatani Mansukhani
   Fecha: 21/11/2026
   
   // Prueba porque quiero y porque puedo
*/

#include "sieteseg.h"
#include "timer.h"

int main(void) {
  /* Inicializar los displays */
  sieteSeg_init();
  serial_print("\r\n\nTemporizador inicializado.\n");
  while (1) { 
    uint16_t tiempo = (timer_get_ms() / 1000);
    sietesSeg_valor(tiempo);
  }
}