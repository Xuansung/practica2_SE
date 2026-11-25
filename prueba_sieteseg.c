
/* prueba_sieteseg.c -- Programa de prueba de los displays de 7 
   segmentos.

   Autores: Alba Hidalgo Martín, Xuan Sun y Juhi Kamal Chatani Mansukhani
   Fecha: 21/11/2026
*/

#include "sieteseg.h"
#include "timer.h"

int main(void) {
  /* Inicializar los displays */
  sieteSeg_init();

  /* Esperar 3 segundos */
  timer_sleep_ms(3000);

  /* Mostrar el número 5678 */
  sietesSeg_valor(5678);

  /* Esperar 3 segundos */
  timer_sleep_ms(3000);

  /* Mostrar el número 25 */
  sietesSeg_valor(25);

  while (1) {
  }
}
