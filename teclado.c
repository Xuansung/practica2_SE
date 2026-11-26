#include "teclado.h"

#define PUERTO    'H'
#define FILAS     0x56   /* bits 1, 2, 4, 6 */
#define COLUMNAS  0xA8   /* bits 3, 5, 7    */
#define REBOTE_MS 20

/* Pines de cada fila y columna, en orden */
static const uint8_t filas[4]    = {6, 1, 2, 4};
static const uint8_t columnas[3] = {5, 7, 3};

static const char teclas[4][3] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};




/* Recorre las filas y devuelve la tecla pulsada, o 'A' si no hay */
static char buscar_tecla(void) {
  uint8_t f;
  uint8_t c;
  char tecla = 'A';

  for (f = 0; f < 4 && tecla == 'A'; f++) {
    /* Solo la fila f a 0, el resto a 1 */
    gpio_setPort(PUERTO, FILAS & ~(1 << filas[f]));

    for (c = 0; c < 3; c++)
      if (gpio_getPin(PUERTO, columnas[c]) == 0) {
        tecla = teclas[f][c];
      }
  }

  gpio_setPort(PUERTO, 0);   /* reposo: todas las filas a 0 */
  return tecla;
}


void teclado_init(void) {
  gpio_setDDRPort(PUERTO, FILAS);   /* filas salida, columnas entrada */
  gpio_setPullUp(PUERTO, 1);        /* columnas a 1 si no se pulsa */
  gpio_setPort(PUERTO, 0);          /* todas las filas a 0 */
}


char teclado_getch(void) {
  return teclado_getch_timeout(0);  /* 0 = sin límite de tiempo */
}


char teclado_getch_timeout(uint32_t ms) {
  uint32_t inicio = timer_get_ms();
  char tecla = 'A'; /*Ninguna del teclado*/

  /* 1. Esperar a que se suelte la tecla anterior */
  while ((gpio_getPort(PUERTO) & COLUMNAS) != COLUMNAS) {
    if (ms && timer_get_ms() - inicio >= ms) return 'T';
  }

  /* 2. Rebote al soltar */
  timer_sleep_ms(REBOTE_MS);

  while (tecla == 'A') {
    /* 3. Esperar mientras las columnas estén a 1 */
    while (!(gpio_getPort(PUERTO) & COLUMNAS) != COLUMNAS) {/*No hay tecla*/
      if (ms && timer_get_ms() - inicio >= ms) return 'T';
    }
    /* 4. Rebote al pulsar */
    timer_sleep_ms(REBOTE_MS);

    /* 5. Escanear (si era un rebote da 'A' y se repite) */
    tecla = buscar_tecla();
  }

  /* 6. Devolver la tecla */
  return tecla;
}