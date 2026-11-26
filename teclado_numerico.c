#include "teclado.h"
#include "gpio.h"
#include "timer.h"

/* Filas */
static const uint8_t filas[4] = {6, 1, 2, 4};

/* Columnas */
static const uint8_t columnas[3] = {5, 7, 3};

/* Distribución de las teclas */
static const char teclas[4][3] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};


/**
 * @brief Comprobar si hay alguna tecla pulsada.
 * @return uint8_t 1 si hay alguna tecla pulsada, 0 si no hay ninguna.
 */
static uint8_t tecla_pulsada(void) {
  uint8_t i;

  /* Recorrer las columnas del teclado */
  for (i = 0; i < 3; i++) {
    /* Si una columna está a 0, significa que hay una tecla pulsada */
    if (gpio_getPin('H', columnas[i]) == 0) {
      return 1;
    }
  }
  /* Si están todas las teclas a 1, no hay ninguna tecla pulsada */
  return 0;
}


/**
 * @brief Identifica la tecla que está pulsada.
 * @return Caracter ASCII de la tecla pulsada.
 */
static char buscar_tecla(void) {
  uint8_t fila;
  uint8_t columna;

  /* Poner todas las filas a 1 antes de empezar */
  for (fila = 0; fila < 4; fila++) {
    gpio_setPin('H', filas[fila], 1);
  }

  /* Recorrer las filas para localizar la tecla */
  for (fila = 0; fila < 4; fila++) {

    /* Poner la fila actual a 0 para activarla */
    gpio_setPin('H', filas[fila], 0);

    /* Comprobar el estado de las columnas */
    for (columna = 0; columna < 3; columna++) {
      /* Si una columna está a 0, se encontró la tecla*/
      if (gpio_getPin('H', columnas[columna]) == 0) {
        /* Volver a poner todas las filas a 0*/
        for (uint8_t i = 0; i < 4; i++) {
          gpio_setPin('H', filas[i], 0);
        }
        /* Devolver el carácter de la posición encontrada */
        return teclas[fila][columna];
      }
    }

    /* Si no se encuentra la tecla, se desactiva esta fila antes de
       pasar a la siguiente. */
    gpio_setPin('H', filas[fila], 1);
  }
  /* Si no se encuentra ninguna tecla, se restaura el estado inicial */
  for (fila = 0; fila < 4; fila++) {
    gpio_setPin('H', filas[fila], 0);
  }

  /* Devolver un carácter nulo para indicar que no se encontró ninguna */
  return '\0';
}


/**
 * @brief Espera hasta que el usuario suelte la tecla pulsada.
 */
static void esperar_tecla(void) {
  /* Poner todas las filas a 0 para detectar si la tecla sigue pulsada */
  for (uint8_t i = 0; i < 4; i++) {
    gpio_setPin('H', filas[i], 0);
  }

  /* Mientras alguna columna está a 0, esperamos */
  while (tecla_pulsada()) {
    /* Esperar a que se suelte la tecla */ 
  }

  /* Esperar 20 ms*/
  timer_sleep_ms(20);
}


/**
 * @brief Configura los pines necesarios para utilizar el teclado.
 * 
 */
void teclado_init(void) {
  /* Configurar las filas como salidas y las columnas como entradas
     0x56 = 01010110, donde los bits a 1 son de las salidas */
  gpio_setDDRPort('H', 0x56);

  /* Activar las resistencias pull-up del puerto H, y así las columnas 
     valdrán 1 cuando no haya pulsaciones */
  gpio_setPullUp('H', 1);

  /* Dejar todas las filas a 0 para establecer el estado inicial */
  for (uint8_t i = 0; i < 4; i++) {
    gpio_setPin('H', filas[i], 0);
  }
}


/**
 * @brief Espera hasta que sepulse una tecla.
 * @return char Caracter ASCII de la tecla pulsada.
 */
char teclado_getch(void) {
  /* Llamar a la función con timeout 0 */
  return teclado_getch_timeout(0);
}

/**
 * @brief Espera hasta que se pulse una tecla o se agote el tiempo.
 * 
 * @param ms Tiempo máximo de espera en milisegundos.
 * @return char Caracter ASCII de la tecla o T si se agota el tiempo.
 */
char teclado_getch_timeout(uint32_t ms) {
  /* Guardar el instante inicial */
  uint32_t inicio = timer_get_ms();

  /* Variable para guardar la tecla */
  char tecla;

  /* Repetir hasta encontrar una tecla o agotar el tiempo */
  while (1) {
    if (tecla_pulsada()) {
      /* Esperar 20 ms para evitar rebotes */
      timer_sleep_ms(20);

      /* Volver a comprobar si la tecla sigue pulsada */
      if (tecla_pulsada()) {
        /* Buscar la tecla */
        tecla = buscar_tecla();
        /* Comprobar que de verdad hemos encontrado una tecla */
        if (tecla != '\0') {
          /* Esperar hasta que el usuario suelte la tecla */
          esperar_tecla();
          return tecla;
        }
      }
    }

    /* Si el timeout es distinto de 0, comprobamos si se ha agotado */
    if (ms != 0 && (uint32_t)(timer_get_ms() - inicio) >= ms) {
      return 'T';
    }
  }
}