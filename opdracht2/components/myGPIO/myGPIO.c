#include <stdio.h>
#include "myGPIO.h"
#include "driver/gpio.h"


void myGPIO_LED_aan(int pin) {
    gpio_set_level(pin, 1);
}
void myGPIO_LED_uit(int pin) {
    gpio_set_level(pin, 0);
}
void myGPIO_LED_setup(int pin) {
    gpio_reset_pin(pin);
    gpio_set_direction(pin, GPIO_MODE_OUTPUT);
}
void myGPIO_LED_toggle(int pin, int preValue){
    if (preValue == 1) {
        gpio_set_level(pin, 0);
    } 
    if (preValue == 0) {
        gpio_set_level(pin, 1);
    }
}




void myGPIO_knop_setup(int pin) {
    gpio_reset_pin(pin);
    gpio_set_direction(pin, GPIO_MODE_INPUT);
    gpio_pullup_en(pin);
}
int myGPIO_knop_lezen(int pin) {
    pin = gpio_get_level(pin);
    return pin;
}
int myGPIO_knop_flank(int pin) {
    static int vroeger = 1, nu = 1;
    vroeger = nu;
    nu = gpio_get_level(pin);

    if (nu != vroeger) {
        return 1;
    }
    return 0;
}