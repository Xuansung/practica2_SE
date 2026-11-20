

#include "sieteseg.h"

#define PORT 'T'
#define FACTOR 3

uint8_t* array_pasado = NULL;
int digito_actual;

void sieteSeg_init(){
  gpio_setDDRPortAll(PORT, 1);
  timer_init(FACTOR);
  gpio_setPort('1234');
}



void sieteSeg_digitos(uint8_t* array){
  array_pasado = array;
  timer_set_periodic(20000, refrescar);
}

void sietesSeg_valor(uint16_t decimal){
  gpio_setPort(PORT, decimal);

}


void refrescar() {
  for (int i = 0; i < 4; i++) {
    gpio_setPort(PORT, array_pasado[i]);
  }
}
