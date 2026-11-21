/* sieteseg.c -- Manejo de los 4 displays de 7 segmentos de la DISEN-EXP

  Hay un único decodificador compartido por los 4 displays, así que solo
  se puede encender uno cada vez. Se hace multiplexación temporal: una
  función periódica del timer enciende un display en cada llamada, por
  turnos. Si se hace rápido (> 100 Hz), el ojo ve los 4 encendidos.

  Conexionado (ajustar a la placa): Posiblemente se deba cambiar esto
    bits 0-3 del puerto -> entradas del decodificador (número 0-9)
    bits 4-7 del puerto -> encendido de cada display (bit 4 = display 0)

  Requiere haber llamado a timer_init() antes de sieteSeg_init().
*/

#include "sieteseg.h"
#include "gpio.h"
#include "timer.h"

#define PORT 'T'
#define PERIODO 2000 /* 2ms por display */
#define FACTOR 3

/*A partir de qué bit se encienden los displays (cambiar si es necesario) */
#define ENCENDER_BIT 4


static uint8_t array_inicial[4] = {1, 2, 3, 4}; /* Para empezar*/
static uint8_t valor_mostrado[4] = {0}; /* Para el decimal */
uint8_t *array_pasado = array_inicial;
uint8_t bit_display_actual = 0; /*Nº de display actual del 0 al 3*/

void refrescar() {
  uint8_t num = array_pasado[bit_display_actual] & 0x0F; /*Solo 4 bits*/
  uint8_t interruptor = 1 << (ENCENDER_BIT + bit_display_actual); /* Bit real que toca*/
  gpio_setPort(PORT, interruptor | num); /* Para encender y mostrar el num*/

  bit_display_actual++;
  if (bit_display_actual >= ENCENDER_BIT) {
    bit_display_actual = 0; /* Cambiar numero totalmente*/
  }

}

void sieteSeg_init(){
  gpio_setDDRPortAll(PORT, 1);
  timer_init(FACTOR);
  gpio_setPort(PORT, 0); 
  timer_set_periodic(PERIODO, refrescar);
}



void sieteSeg_digitos(uint8_t *array){
  array_pasado = array;
}

void sietesSeg_valor(uint16_t decimal){
  /** Separa el número en cifras de las unidades hacia la izq
   * Si el valor es muy grande se queda últimas 4 cifras
   */
  for (int i= ENCENDER_BIT - 1; i >= 0; i--) {
    valor_mostrado[i] = decimal % 10;
    decimal /= 10;
  }
  array_pasado = valor_mostrado;

}
