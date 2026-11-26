/* sieteseg.c -- Manejo de los 4 displays de 7 segmentos

  Hay un único decodificador compartido por los 4 displays, así que solo
  se puede encender uno cada vez. Se hace multiplexación temporal: una
  función periódica del timer enciende un display en cada llamada, por
  turnos. Si se hace rápido (> 100 Hz), el ojo ve los 4 encendidos.

  Conexionado (ajustar a la placa): Posiblemente se deba cambiar esto
    bits 0-3 del puerto -> entradas del decodificador (número 0-9)
    bits 4-7 del puerto -> encendido de cada display (bit 4 = display 0)

  Autores: Alba Hidalgo Martín, Xuan Sun y Juhi Kamal Chatani Mansukhani
  Fecha: 19/11/2026
*/

#include "sieteseg.h"
#include "gpio.h"
#include "timer.h"

/* Configuración del puerto y del temporizado */
#define PORT 'T'
/* 2ms por display */
#define PERIODO 2000 
#define FACTOR 3

/* Número de displays disponibles */
#define NUM_DISPLAYS 4

/* Array para enseñar 1234 en el inicio */
static uint8_t array_pasado[NUM_DISPLAYS] = {4,3,2,1}; 

/* Indica el display que se tiene que actualizar (0 a 3) */
static uint8_t bit_display_actual = 0; 


/* Refresca un display en cada ejecución.
   Se ejecuta periódicamente mediante una interrupción.
*/
static void refrescar(void) {
  /* Obtener el número que corresponde al display actual */
  uint8_t num = array_pasado[bit_display_actual] & 0x0F;
  
  /* Seleccionar el display que se va a encender */
  uint8_t interruptor = 1 << (
    NUM_DISPLAYS + bit_display_actual
  );
  
  /* Encender el display y enviar el número al decodificador */
  gpio_setPort(PORT, interruptor | num);

  /* Pasar al siguiente display */
  bit_display_actual++;

  /* Cuando se llega al último, se vuelve al primero */
  bit_display_actual %= NUM_DISPLAYS;

}


/* Inicializa los displays y programa su refresco periódico */
void sieteSeg_init(void){
  /* Configurar los pines del puerto como salidas */
  gpio_setDDRPortAll(PORT, 1);

  /* Inicializa el temporizador */
  timer_init(FACTOR);

  /* Apaga inicialmente todos los displays */
  gpio_setPort(PORT, 0); 

  /* Programa el refresco de los displays cada 2ms */
  timer_set_periodic(PERIODO, refrescar);
}


/* Selecciona el array de dígitos que se quiere mostrar */
void sieteSeg_digitos(uint8_t array[]){
  /*uint8_t *puntero_array = array;
  if (puntero_array != 0) {
    *array_pasado = *puntero_array;
  }*/

  for (int i=0; i < NUM_DISPLAYS; i++) {
    array_pasado[i] = array[i];
  }
}


/* Convierte un número decimal en cuatro cifras */
void sietesSeg_valor(uint16_t decimal){
   /* Separa las cifras empezando por las unidades.
      Si el valor es muy grande se queda con las últimas 4 cifras.
   */
  for (int i=0; i < NUM_DISPLAYS; i++) {
    array_pasado[i] = decimal % 10;
    decimal /= 10;
  }

  /* Selecciona el array con las cifras obtenidas */

}
