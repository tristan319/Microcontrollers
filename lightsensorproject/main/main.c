#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "myADC.h"
#include "hal/adc_types.h"

#define LIGHT_CHANNEL ADC_CHANNEL_6

void app_main(void)
{
    myADC_setup(LIGHT_CHANNEL);

    while (1)
    {
        int value = myADC_getValue(LIGHT_CHANNEL);
        printf("Light: %d\n", value);

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}