#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

/* Funciones para infrarojo */

/*
 *  @brief  Inicializa un sensor infrarojo en el pin indicado
 *  @param  pin: numero de pin conectado al infrarojo
 */
static inline void infrarojo_init(uint8_t pin){
    gpio_init(pin);
    gpio_set_dir(pin, false);
}

/*
 *  @brief  Obtiene el valor que entregue el infrarojo
 *  @param  pin: numero de pin conectado al infrarojo
 *  @return true si la luz volvio, false si no
 */
static inline bool infrarojo_get(uint8_t pin) { return !gpio_get(pin); }

/* Prototipos para funciones de ultrasonido */
void ultrasonico_init(uint8_t trigger, uint8_t echo);
float ultrasonico_get_distance_cm(uint8_t trigger, uint8_t echo);

/* Prototipos para funciones de motores */
void motor_init(uint8_t pin_forward, uint8_t pin_reverse);
void motor_forward(uint8_t pin_forward, uint8_t pin_reverse);
void motor_reverse(uint8_t pin_forward, uint8_t pin_reverse);
void motor_forward_partial(uint8_t pin_forward, uint8_t pin_reverse, uint8_t partial);
void motor_reverse_partial(uint8_t pin_forward, uint8_t pin_reverse, uint8_t partial);

/*
 *  @brief  Programa principal
 */

int 
int main(void) 
{

    stdio_init_all();
    infrarojo_init(1);
    infrarojo_init(2);
    infrarojo_init(3); // INFRARROJOS 
    gpio_init(4);
    gpio_init(5);
    gpio_init(6);
    gpio_init(7);
    gpio_set_dir(4,0);
    gpio_set_dir(5,0);
    gpio_set_dir(6,0);
    gpio_set_dir(7,0); // SWITCHES 
    motor_init(9,10); //MOTOR IZQUIERDO
    motor_init(27,28); //MOTOR DERECHO
    ultrasonico_init(12,13); //ULTRASONICO
    gpio_init(16);
    gpio_init(17);
    gpio_init(18); //LEDS



    while(1)
    {
        // inf1: izquierda, inf2: derecha, inf3: adelante 
        bool infL = infrarojo_get(1); bool infR = infrarojo_get(2); bool infA = infrarojo_get(3);
        const int motor1f = 9; const int motor1b = 10; // motor izquierdo
        const int motor2f= 27; const int motor2b = 28; // motor derecho
        const int trigger= 27; const int echo = 28;
        

        if (gpio_get(4) == 1)
        {
            modo = 1;   // se pone al robot en modo despejar el área
        }

        if (gpio_get(5) == 1)
        {
            modo = 2; // se pone al robot en modo prueba de velocidad y control 
        }

        if (gpio_get(6) == 1)
        {
            modo = 3; // se pone al robot en modo sumo robótico 
        }

        if (gpio_get(7) == 1)
        {
            modo = 0; // se pone al robot en nulo
        }
        if(modo == 1)  //modo despejar el área 
        {
            float dist = ultrasonico_get_distance_cm(trigger,echo);
            
            
           if(infR = 0)   // si el sensor derecho se detecta fuera de la pista, se gira a la izquierda 
           {

                motor_forward(motor2f,motor2b);
                motor_forward_partial(motor1f,motor1b,5);

           }
           
           if(infL = 0)   // si el sensor izquierdo se detecta fuera de la pista, se gira a la derecha 
           {

                motor_forward(motor1f,motor1b);
                motor_forward(motor2f,motor2b);

           }
           if(infA = 0)   // si el sensor delantero se detecta fuera de la pista, se gira a la izquierda 
           {

                motor_reverse_partial(motor1f,motor1b);
                motor_reverse_partial(motor2f,motor2b);

           }

           else 
           {         
                motor_reverse(motor1f, motor1b);
                motor_forward(motor2f, motor2b); // el robot gira en circulos 

                if(dist <= 1500) // si se detecta algo cerca, se avanza
                {
                    motor_forward(motor1f,motor1b);  
                    motor_forward(motor2f,motor2b);
                }
           }


        }

        if(modo == 2) // modo velocidad y control
        {
            /*
            primero, el robot va a buscar ir hacia la derecha, y va tener como referencia
            la parte negra del costado; el reglamento lo permite
            */
            if (infL == 1 && infA == 1 && infR = 0) // si solo el sensor derecho detecta la parte negra, se avanza
            {

                motor_forward(motor1f,motor1b);  
                motor_forward(motor2f,motor2b);

            }

            if ( infL == 1 && infA == 0 && infR = 0 ) // si el sensor delantero y derecho detectan negro, se gira a la izquierda
            {

                motor_forward_partial(motor1f,motor1b, 20);
                motor_forward_partial(motor2f,motor2b, 80);

            }
            
            else    // si detecta todo blanco, se dobla a la derecha 
            {

                motor_forward_partial(motor1f,motor1b, 60 );        
                motor_forward_partial(motor2f,motor2b, 30 );

            }
            
        }

        if(modo == 3) // modo sumo
        {


            
        }
    
    }

    

}

 
 
/*
 *  @brief  Inicializa los pines del ultrasonico
 *
 *  @param  trigger: numero de GP para usar de TRIGGER
 *  @param  echo: numero de GP para usar de ECHO
 */
void ultrasonico_init(uint8_t trigger, uint8_t echo) {
    /* Habilito el pin de Trigger */
    gpio_init(trigger);  
    /* Configuro el Trigger como salida */
    gpio_set_dir(trigger, true);
    /* Pongo un 0 en el pin de Trigger */
    gpio_put(trigger, false);
    /* Habilito el pin de Echo */
    gpio_init(echo);
    /* Configuro el Echo como entrada */
    gpio_set_dir(echo, false);
}

/*
 *  @brief  Obtiene la distancia en cm
 *  
 *  @param  trigger: numero de GP para usar de TRIGGER
 *  @param  echo: numero de GP para usar de ECHO
 * 
 *  @return distancia en cm
 */
float ultrasonico_get_distance_cm(uint8_t trigger, uint8_t echo) {
    /* Escribo un 1 en el Trigger */
    gpio_put(trigger, true);
    /* Espero 10 us con el pulso encendido */
    sleep_us(10);
    /* Escribo un 0 en el Trigger */
    gpio_put(trigger, false);
    /* Espero a que el pulso llegue al Echo */
    while(!gpio_get(echo));
    /* Mido el tiempo de cuando llega el pulso */
    absolute_time_t from = get_absolute_time();
    /* Espero a que el pulso del Echo baje a cero */
    while(gpio_get(echo));
    /* Mido el tiempo cuando termina el pulso */
    absolute_time_t to = get_absolute_time();
    /* Saco la diferencia de tiempo (el ancho del pulso) en us */
    int64_t pulse_width = absolute_time_diff_us(from, to);
    /* Calculo la distancia y la devuelvo */
    return pulse_width / 59.0;
}

/*
 *  @brief  Inicializa los pines para trabajar con un motor de DC
 *  @param  pin_forwart: numero de pin que controla la direccion de avance
 *  @param  pin_reverse: numero de pin que controla la direccion de reversa
 */
void motor_init(uint8_t pin_forward, uint8_t pin_reverse) {
    /* Seteo pines como PWM */
    gpio_set_function(pin_forward, GPIO_FUNC_PWM);
    gpio_set_function(pin_reverse, GPIO_FUNC_PWM);
    /* Consigo los slices de los pines */
    uint slice_pin_forward = pwm_gpio_to_slice_num(pin_forward);
    uint slice_pin_reverse = pwm_gpio_to_slice_num(pin_forward);
    /* Obtengo configuracion por defecto de PWM */
    pwm_config config = pwm_get_default_config();
    /* Inicializo ambos slices */
    pwm_init(slice_pin_forward, &config, true);
    pwm_init(slice_pin_reverse, &config, true);
    /* Arranco con 0% de ancho de pulso */
    pwm_set_gpio_level(pin_forward, 0);
    pwm_set_gpio_level(pin_reverse, 0);
}

/*
 *  @brief  Indica al motor de que vaya hacia adelante
 *  @param  pin_forwart: numero de pin que controla la direccion de avance
 *  @param  pin_reverse: numero de pin que controla la direccion de reversa
 */
void motor_forward(uint8_t pin_forward, uint8_t pin_reverse) {
    /* 0% de ancho de pulso para el pin de reversa */
    pwm_set_gpio_level(pin_reverse, 0);
    /* 100% de ancho de pulso para el pin de avance */
    pwm_set_gpio_level(pin_forward, 0xffff);
}

/*
 *  @brief  Indica al motor de que vaya hacia atras
 *  @param  pin_forwart: numero de pin que controla la direccion de avance
 *  @param  pin_reverse: numero de pin que controla la direccion de reversa
 */
void motor_reverse(uint8_t pin_forward, uint8_t pin_reverse) {
    /* 0% de ancho de pulso para el pin de avance */
    pwm_set_gpio_level(pin_forward, 0);
    /* 100% de ancho de pulso para el pin de reversa */
    pwm_set_gpio_level(pin_reverse, 0xffff);
}

/*
 *  @brief  Indica al motor de que vaya hacia adelante parcialmente
 *  @param  pin_forwart: numero de pin que controla la direccion de avance
 *  @param  pin_reverse: numero de pin que controla la direccion de reversa
 *  @param  partial: porcentaje de potencia entregada
 */
void motor_forward_partial(uint8_t pin_forward, uint8_t pin_reverse, uint8_t partial) {
    /* 0% de ancho de pulso para el pin de reversa */
    pwm_set_gpio_level(pin_reverse, 0);
    /* Calculo el valor de duty necesario */
    uint16_t duty = (uint32_t)partial * 0xffff / 100.0;
    /* Porcentaje de ancho de pulso indicado para el pin de avance */
    pwm_set_gpio_level(pin_forward, duty);
}

/*
 *  @brief  Indica al motor de que vaya hacia atras parcialmente
 *  @param  pin_forwart: numero de pin que controla la direccion de avance
 *  @param  pin_reverse: numero de pin que controla la direccion de reversa
 *  @param  partial: porcentaje de potencia entregada
 */
void motor_reverse_partial(uint8_t pin_forward, uint8_t pin_reverse, uint8_t partial) {
    /* 0% de ancho de pulso para el pin de avance */
    pwm_set_gpio_level(pin_forward, 0);
    /* Calculo el valor de duty necesario */
    uint16_t duty = (uint32_t)partial * 0xffff / 100.0;
    /* Porcentaje de ancho de pulso indicado para el pin de reversa */
    pwm_set_gpio_level(pin_reverse, duty);
}