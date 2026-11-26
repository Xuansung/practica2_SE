/* prueba_teclado.c -- Programa de prueba del teclado numérico.

   Autores: Alba Hidalgo Martín, Xuan Sun y Juhi Kamal Chatani Mansukhani
   Fecha: 26/11/2026
*/

#include "teclado_numerico.h"
#include "timer.h"

int main(void) {
  /* Inicializar el temporizador */
  timer_init(3);

  /* Inicializar el teclado */
  teclado_init();

  /* Mostrar mensaje inicial */
  serial_print("\r\n\nTeclado inicializado.\n");
  serial_print("Pulsa una tecla del teclado numerico.\n");

  while (1) {
    /* Esperar una tecla para 5 segundos */
    char tecla = teclado_getch_timeout(5000);

    /* Comprobar si se ha agotado el tiempo */
    if (tecla == 'T') {
      serial_print("Tiempo agotado. No se ha pulsado ninguna tecla.\n");
    } else {
      /* Mostrar la tecla pulsada */
      serial_print("Tecla pulsada: ");
      serial_putchar(tecla);
      serial_print("\n");
    }
  }
  
  return 0;
}