#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "myADC.h"
#include "esp_adc/adc_oneshot.h"

// GPIO36 = ADC1_CHANNEL_0 on ESP32
#define LIGHT_SENSOR_CHANNEL    ADC_CHANNEL_0

// Thresholds (tune these to your sensor/environment)
#define LIGHT_DARK_MV           500     // below this → dark
#define LIGHT_DIM_MV            1500    // below this → dim
#define LIGHT_BRIGHT_MV         2500    // below this → bright
                                        // above      → very bright

static const char *TAG = "LightSensor";

void app_main(void)
{
    // Initialise ADC channel for the light sensor
    myADC_setup(LIGHT_SENSOR_CHANNEL);

    while (1)
    {
        int raw_value = myADC_getValue(LIGHT_SENSOR_CHANNEL);
        int millivolt  = myADC_getMiliVolt(LIGHT_SENSOR_CHANNEL);

        // Classify ambient light level
        const char *level;
        if      (millivolt < LIGHT_DARK_MV)   level = "Dark";
        else if (millivolt < LIGHT_DIM_MV)    level = "Dim";
        else if (millivolt < LIGHT_BRIGHT_MV) level = "Bright";
        else                                   level = "Very bright";

        printf("[%s] Raw: %4d  |  Voltage: %4d mV  |  Level: %s\n",
               TAG, raw_value, millivolt, level);

        vTaskDelay(pdMS_TO_TICKS(500));   // sample every 500 ms
    }
}