/* prueba_sieteseg.c -- Programa de prueba de los displays de 7
   segmentos.

   Muestra por orden:
     1. El array {4,3,2,1} con sieteSeg_digitos()  -> 1234
     2. El número 5678 con sietesSeg_valor()       -> 5678
     3. El número 25 con sietesSeg_valor()         ->   25
   Por la serial se indica en cada paso qué debe verse.

   IMPORTANTE: el array que se pasa a sieteSeg_digitos() debe ir en
   orden INVERTIDO. La posición 0 se muestra en el display de la
   derecha (unidades) y la posición 3 en el de la izquierda (millares).
   Para que se vea 1234 hay que pasar {4, 3, 2, 1}.

   Autores: Alba Hidalgo Martín, Xuan Sun y Juhi Kamal Chatani Mansukhani
   Fecha: 21/11/2026
*/

#include "sieteseg.h"
#include "timer.h"

/* Tiempo que se mantiene cada prueba */
#define ESPERA_MS 3000

int main(void) {
  /* Orden invertido: posición 0 = display de la derecha.
     {4, 3, 2, 1} se ve como 1234 */
  uint8_t digitos[4] = {4, 3, 2, 1};

  serial_init();
  serial_print("\r\n===== Prueba de los displays de 7 segmentos =====\r\n");

  /* Inicializar los displays */
  serial_print("\r\nInicializando displays...");
  sieteSeg_init();
  serial_print(" OK");

  /* Prueba 1: sieteSeg_digitos (array en orden invertido) */
  serial_print("\r\n\r\n[1] sieteSeg_digitos({4, 3, 2, 1})");
  serial_print("\r\n    Debe verse: 1234 (el array va en orden invertido)");
  sieteSeg_digitos(digitos);
  timer_sleep_ms(ESPERA_MS);

  /* Prueba 2: sietesSeg_valor con 4 cifras */
  serial_print("\r\n\r\n[2] sietesSeg_valor(5678)");
  serial_print("\r\n    Debe verse: 5678");
  sietesSeg_valor(5678);
  timer_sleep_ms(ESPERA_MS);

  /* Prueba 3: sietesSeg_valor con 2 cifras */
  serial_print("\r\n\r\n[3] sietesSeg_valor(25)");
  serial_print("\r\n    Debe verse:   25 (los dos primeros apagados)");
  sietesSeg_valor(25);

  serial_print("\r\n\r\nFin de la prueba. Se queda mostrando 25.\r\n");

  /* El refresco sigue funcionando por interrupciones */
  while (1) {
  }

  return 0;
}