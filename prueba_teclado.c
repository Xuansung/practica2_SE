/* prueba_teclado.c -- Programa de prueba del teclado numérico.

   Espera pulsaciones con un timeout de 5 segundos y muestra por la
   serial la tecla pulsada o un aviso si se agota el tiempo.

   Autores: Alba Hidalgo Martín, Xuan Sun y Juhi Kamal Chatani Mansukhani
   Fecha: 26/11/2026
*/

#include "teclado_numerico.h"
#include "timer.h"

int main(void) {
  char tecla;

  /* Inicializar la serial */
  serial_init();

  /* Inicializar el temporizador */
  timer_init(3);

  /* Inicializar el teclado */
  teclado_init();

  /* Mostrar mensaje inicial */
  serial_print("\r\n\r\nTeclado inicializado.");
  serial_print("\r\nPulsa una tecla del teclado numerico.\r\n");

  while (1) {
    /* Esperar una tecla durante 5 segundos como máximo */
    tecla = teclado_getch_timeout(5000);

    /* Comprobar si se ha agotado el tiempo */
    if (tecla == 'T') {
      serial_print("\r\nTiempo agotado. No se ha pulsado ninguna tecla.");
    } else {
      /* Mostrar la tecla pulsada */
      serial_print("\r\nTecla pulsada: ");
      serial_send(tecla);
    }
  }

}