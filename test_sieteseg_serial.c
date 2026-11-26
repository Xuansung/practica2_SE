/* test_sieteseg_serial.c -- Prueba interactiva de los displays de
   7 segmentos.

   Mediante un menú por la serial se pide al usuario qué quiere mostrar:
     1. Un número (sietesSeg_valor)
     2. Cuatro dígitos sueltos (sieteSeg_digitos)
     3. Un contador entre dos números, a 2 Hz

   Mientras se espera a que el usuario escriba, los displays siguen
   refrescándose porque el refresco se hace por interrupciones.

   IMPORTANTE: el array que se pasa a sieteSeg_digitos() debe ir en
   orden INVERTIDO. La posición 0 se muestra en el display de la
   derecha (unidades) y la posición 3 en el de la izquierda (millares).
   Por eso en la opción 2 los dígitos se guardan al revés de como se
   escriben.

   Autores: Alba Hidalgo Martín, Xuan Sun y Juhi Kamal Chatani Mansukhani
   Fecha: 21/11/2026
*/

#include "sieteseg.h"
#include "timer.h"

/* Periodo del contador: 500 ms -> el valor cambia como mucho a 2 Hz */
#define CONTADOR_MS 500


/* Lee un número decimal por la serial hasta pulsar Enter.
   - Solo acepta dígitos (el resto se ignora).
   - Permite borrar con la tecla de retroceso.
   - Si el número supera 'maximo', avisa y lo vuelve a pedir. */
static uint16_t leer_numero(uint16_t maximo) {
  uint32_t numero;
  uint8_t cifras;
  char c;

  while (1) {
    numero = 0;
    cifras = 0;

    while (1) {
      c = serial_get();

      if (c == '\r' || c == '\n') {         /* Enter: fin del número */
        if (cifras > 0)
          break;
      } else if ((c == '\b' || c == 127) && cifras > 0) {  /* borrar */
        numero /= 10;
        cifras--;
        serial_print("\b \b");
      } else if (c >= '0' && c <= '9' && cifras < 5) {
        numero = numero * 10 + (c - '0');
        cifras++;
        serial_send(c);                     /* eco de lo escrito */
      }
    }

    if (numero <= maximo)
      return (uint16_t) numero;

    serial_print("\r\n  Fuera de rango (max ");
    serial_print_dec_uint32(maximo);
    serial_print("). Otra vez: ");
  }
}


/* Opción 1: pide un número y lo muestra */
static void opcion_numero(void) {
  uint16_t n;

  serial_print("\r\n  Numero (0 - 65535): ");
  n = leer_numero(65535);

  sietesSeg_valor(n);

  serial_print("\r\n  Mostrando ");
  serial_print_dec_uint32(n);
  if (n > 9999)
    serial_print(" (solo se ven las 4 ultimas cifras)");
}


/* Opción 2: pide 4 dígitos uno a uno y los muestra.

   El usuario los escribe de izquierda a derecha (como se leen), pero
   sieteSeg_digitos() necesita el array en orden INVERTIDO
   (posición 0 = display de la derecha). Por eso el primer dígito
   escrito se guarda en la posición 3 y el último en la posición 0. */
static void opcion_digitos(void) {
  /* static para que siga existiendo al salir de la función:
     sieteSeg_digitos() puede quedarse con el puntero */
  static uint8_t digitos[4];
  uint8_t i;

  for (i = 0; i < 4; i++) {
    serial_print("\r\n  Digito ");
    serial_print_dec_uint32(i + 1);
    serial_print(" (0 - 9): ");

    /* Orden invertido: el dígito i se guarda en la posición 3 - i */
    digitos[3 - i] = (uint8_t) leer_numero(9);
  }

  sieteSeg_digitos(digitos);

  /* Se imprime de la posición 3 a la 0 para que salga en el mismo
     orden en que se ve en los displays */
  serial_print("\r\n  Mostrando ");
  for (i = 0; i < 4; i++)
    serial_send('0' + digitos[3 - i]);
}


/* Opción 3: cuenta desde 'inicio' hasta 'fin' a 2 Hz */
static void opcion_contador(void) {
  uint16_t inicio, fin, n;

  serial_print("\r\n  Desde (0 - 9999): ");
  inicio = leer_numero(9999);
  serial_print("\r\n  Hasta (0 - 9999): ");
  fin = leer_numero(9999);

  serial_print("\r\n  Contando... (pulsa una tecla para parar)");

  /* Quitar una tecla que pudiera haber quedado pendiente */
  if (serial_receive_pending())
    serial_get();

  n = inicio;
  while (1) {
    sietesSeg_valor(n);
    timer_sleep_ms(CONTADOR_MS);

    if (serial_receive_pending()) {         /* el usuario quiere parar */
      serial_get();
      serial_print("\r\n  Parado en ");
      serial_print_dec_uint32(n);
      return;
    }

    if (n == fin) {
      serial_print("\r\n  Fin del contador");
      return;
    }

    /* Cuenta hacia arriba o hacia abajo según el caso */
    if (inicio <= fin)
      n++;
    else
      n--;
  }
}


int main(void) {
  char opcion;

  serial_init();
  sieteSeg_init();                          /* empieza mostrando 1234 */

  serial_print("\r\n===== Prueba interactiva de los 7 segmentos =====");

  while (1) {
    serial_print("\r\n\r\n--- Menu ---");
    serial_print("\r\n 1 - Mostrar un numero");
    serial_print("\r\n 2 - Mostrar 4 digitos");
    serial_print("\r\n 3 - Contador");
    serial_print("\r\nOpcion: ");

    opcion = serial_get();
    serial_send(opcion);

    switch (opcion) {
      case '1': opcion_numero();   break;
      case '2': opcion_digitos();  break;
      case '3': opcion_contador(); break;
      default:
        serial_print("\r\n  Opcion no valida");
        break;
    }
  }

}