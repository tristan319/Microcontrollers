void myGPIO_LED_aan(int pin);
void myGPIO_LED_uit(int pin);
void myGPIO_LED_setup(int pin);
void myGPIO_LED_toggle(int pin, int preValue);

void myGPIO_knop_setup(int pin);
int myGPIO_knop_lezen(int pin);
int myGPIO_knop_flank(int pin, int preValue);