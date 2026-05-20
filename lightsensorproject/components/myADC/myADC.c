#include <stdio.h>
#include "myADC.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"

static adc_channel_t gpio_to_adc_channel(int gpio)
{
    switch (gpio)
    {
    case 34:
        return ADC_CHANNEL_6;
    case 35:
        return ADC_CHANNEL_7;
    case 32:
        return ADC_CHANNEL_4;
    case 33:
        return ADC_CHANNEL_5;
    case 36:
        return ADC_CHANNEL_0;
    case 39:
        return ADC_CHANNEL_3;
    default:
        return ADC_CHANNEL_6;
    }
}

#define ADC_CALIBRATION 1

adc_cali_line_fitting_config_t cali_config = {
    .unit_id = ADC_UNIT_1,
    .atten = ADC_ATTEN_DB_12,
    .bitwidth = ADC_BITWIDTH_12,
};

adc_oneshot_unit_handle_t adc1_handle;
adc_cali_handle_t calibratie_curve_adc1;

adc_oneshot_unit_init_cfg_t adc_config = {
    .unit_id = ADC_UNIT_1,
    .ulp_mode = ADC_ULP_MODE_DISABLE,
};

adc_oneshot_chan_cfg_t default_channel_config = {
    .bitwidth = ADC_BITWIDTH_DEFAULT, // dit is de max supported width en dat is 13
    .atten = ADC_ATTEN_DB_12,
};

void myADC_setup(int gpio)
{
    adc_oneshot_new_unit(&adc_config, &adc1_handle);

    adc_channel_t ch = gpio_to_adc_channel(gpio);

    adc_oneshot_config_channel(adc1_handle, ch, &default_channel_config);

#if ADC_CALIBRATION
    adc_cali_create_scheme_line_fitting(&cali_config, &calibratie_curve_adc1);
#endif
}
int myADC_getValue(int gpio)
{
    int value;
    adc_channel_t ch = gpio_to_adc_channel(gpio);

    adc_oneshot_read(adc1_handle, ch, &value);

    return value;
}
int myADC_getMiliVolt(int gpio)
{
    int raw, voltage;
    adc_channel_t ch = gpio_to_adc_channel(gpio);

    adc_oneshot_read(adc1_handle, ch, &raw);
    adc_cali_raw_to_voltage(calibratie_curve_adc1, raw, &voltage);

    return voltage;
}