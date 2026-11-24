/* pot_sieteseg.c -- Lee un potenciómetro con el conversor A/D a 10 bits
   y muestra el valor (0 - 1023) en los 7 segmentos, 2 veces por segundo. */

#include <types.h>
#include <sys/interrupts.h>
#include <sys/sio.h>
#include "ad.h"
#include "timer.h"
#include "sieteseg.h"

/*  Configuración (ajustar a la placa)  */
#define CONVERSOR    0     /* módulo A/D: 0 o 1                 */
#define CANAL_POT    7     /* pin (0-7) al que va el potenciómetro */
#define REFRESCO_MS  500   /* cambia el número como mucho a 2 Hz */

int main(void) {
  serial_init();
  sieteSeg_init();               

  /* Configuración del conversor A/D */
  ad_set_conversion_resolution(CONVERSOR, 10);   /* 10 bits: 0..1023        */
  ad_set_justification(CONVERSOR, 1); /* a la derecha: valor tal cual */
  ad_set_sample_time(CONVERSOR, 2); /* Tiempo que mira la tensión*/
  ad_set_number_of_conversions(CONVERSOR, NUM_CONV);
  ad_set_channel_mode(CONVERSOR, 0); /* siempre el mismo canal  */
  ad_set_start_pin(CONVERSOR, CANAL_POT);
  ad_set_continuous_conversion(CONVERSOR, 1); /* convierte sin parar     */
  ad_start_conversion(CONVERSOR);

  serial_print("\r\nPotenciometro -> 7 segmentos");

  while (1) {
    ad_wait_to_finish(CONVERSOR); /* Esperar a que termine la recoleccion*/

    sietesSeg_valor(ad_return_value(CONVERSOR));/* 0 .. 1023 */

    timer_sleep_ms(REFRESCO_MS);
  }
}