#include <stdio.h>
#include "myDAC.h"
#include "driver/dac.h"

void myDAC_setup(int channel) {
    if (channel == 1) {
        dac_output_enable(DAC_CHANNEL_1);   // GPIO25
    }
    if (channel == 2) {
        dac_output_enable(DAC_CHANNEL_2);   // GPIO26
    }
}

void myDAC_write(int channel, int value) {
    if (value < 0) value = 0;
    if (value > 255) value = 255;

    if (channel == 1) {
        dac_output_voltage(DAC_CHANNEL_1, value);
    }
    if (channel == 2) {
        dac_output_voltage(DAC_CHANNEL_2, value);
    }
}

void myDAC_uit(int channel) {
    if (channel == 1) {
        dac_output_disable(DAC_CHANNEL_1);
    }
    if (channel == 2) {
        dac_output_disable(DAC_CHANNEL_2);
    }
}