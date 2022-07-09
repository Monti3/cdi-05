#include <stdio.h>
#include "pico/stdlib.h"

int main() {
  stdio_init_all();

  for (int x; x <10;x++)  // se inicializan los pines 
  {
    gpio_init(x);
  }
  for (int x; x <7;x++) // se inicializan como pines de salida 
  {
    gpio_set_dir(x,1);
  }
  gpio_set_dir(7,0); // se inicializan como pines de entrada 
  gpio_set_dir(8,0);
  gpio_set_dir(9,0);

  int g= 0; // acumulador 
  uint8_t digitos [10] =  { 0x01,0x4f,0x12,0x06,0x4c, 0x24,0x20,0x0f,0x00,0x0c}; // El valor en hexa de cada numero en el display
  while(1)
  {
    bool sum = gpio_get(7);
    bool res = gpio_get(8);
    bool reset =gpio_get(9);

    if(sum == 1 && g<9)
    {
      g= g+1;  // g se incrementa si un switch esta en 1
    }
    if(res == 1 &&  g>0) 
    {
      g= g-1; // g decrementa si otro switch esta en 1 
    }
    if(reset == 1) 
    {
      g= 0;  // g vuelve a valer 0 si el tercer switch esta pulsado 
    }
    for (int i = 0; i<7; i++)
    {
      bool val = digitos[g] & (1<<i); // se evalua cada digito del elemento(bajo el indice g) del array en binario
      gpio_put(i, val); // Se expresa el resultado de la operacion anterior en una salida (que depende del contador)

    }
    sleep_ms(500);
  }
}
