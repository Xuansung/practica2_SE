/* pot_sieteseg.c -- Lee uno de los potenciómetros con el conversor A/D
   a 10 bits y representa el valor de la conversión (0 - 1023) en los
   7 segmentos.

   - El conversor y el canal del potenciómetro se fijan con #define.
   - Los displays se actualizan cada 500 ms (como mucho a 2 Hz, para
     que el valor se pueda leer).
   - Por la serial se muestra el valor cada vez que cambia, como ayuda
     para comprobar que coincide con lo que se ve en los displays.


   Autores: Alba Hidalgo Martín, Xuan Sun y Juhi Kamal Chatani Mansukhani
   Fecha: 26/11/2026
*/

#include <types.h>
#include <sys/interrupts.h>
#include <sys/sio.h>
#include "ad.h"
#include "timer.h"
#include "sieteseg.h"

/* Configuración (ajustar a la placa) */
#define CONVERSOR    0      /* módulo A/D: 0 o 1                    */
#define CANAL_POT    0      /* pin al que va el potenciómetro (PAD0x) */
#define RESOLUCION   10     /* 10 bits - valores de 0 a 1023        */
#define MUESTREO     16     /*ciclos de muestreo (16 da tiempo a que cargue)*/
#define REFRESCO_MS  500    /* el valor cambia como mucho a 2 Hz     */



int main(void) {
  uint16_t valor;
  uint16_t valor_anterior = 0;   

  serial_init();
  sieteSeg_init();                    /* hace timer_init() y muestra 1234 */
  ad_set_start_pin(CONVERSOR, CANAL_POT);
  ad_set_conversion_resolution(CONVERSOR, RESOLUCION);
  ad_set_justification(CONVERSOR, 1);          /* derecha: 0..1023 tal cual */
  ad_set_sample_time(CONVERSOR, MUESTREO);     /* tiempo que mira la tensión */
  ad_set_number_of_conversions(CONVERSOR, 1);  /* una conversión por tanda  */
  ad_set_channel_mode(CONVERSOR, 0);           /* siempre el mismo canal    */
  ad_set_continuous_conversion(CONVERSOR, 1);  /* convierte sin parar       */

  ad_start_conversion(CONVERSOR);

  serial_print("\r\n===== Potenciometro -> 7 segmentos =====");
  serial_print("\r\nConversor ");
  serial_print_dec_uint32(CONVERSOR);
  serial_print(", canal ");
  serial_print_dec_uint32(CANAL_POT);
  serial_print(", resolucion 10 bits (0 - 1023)\r\n");

  while (1) {
    /* Esperar a que termine la conversión y leer el resultado */
    ad_wait_to_finish(CONVERSOR);
    valor = ad_return_value(CONVERSOR);

    /* Representar el valor en los 7 segmentos */
    sietesSeg_valor(valor);

    /* Mostrarlo por la serial solo si ha cambiado */
    if (valor != valor_anterior) {
      serial_print("  Valor = ");
      serial_print_dec_uint16(valor);
      serial_print("      \r");
      valor_anterior = valor;
    }

    /* Esperar para no cambiar el número más de 2 veces por segundo */
    timer_sleep_ms(REFRESCO_MS);
  }

}