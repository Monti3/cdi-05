#ifndef _INFRAROJO_H_
#define _INFRAROJO_H_

#include "hardware/gpio.h"

/*
 *  @brief  Inicializa un sensor infrarojo en el pin indicado
 *  
 *  @param  pin: numero de pin conectado al infrarojo
 */
static inline void infrarojo_init(uint8_t pin){
    gpio_init(pin);
    gpio_set_dir(pin, false);
}

/*
 *  @brief  Obtiene el valor que entregue el infrarojo
 *  
 *  @param  pin: numero de pin conectado al infrarojo
 * 
 *  @return true si la luz volvio, false si no
 */
static inline bool infrarojo_get(uint8_t pin){
    return !gpio_get(pin);
}

#endif  /* _INFRAROJO_H_ */