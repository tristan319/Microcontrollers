/**
 * =============================================================
 * BESTAND: main.c
 * ROL:     Hoofdprogramma – leest de lichtsensor uit via de
 *          myADC managed component en stuurt elke seconde
 *          een JSON-regel via de USB-seriële verbinding.
 *
 *          Geen WiFi nodig! Verbinding via USB-kabel.
 *          Python-script op pc leest data en host webpagina
 *          op http://localhost:5000
 *
 *          Uitvoerformaat (één regel per seconde op UART0):
 *            {"raw":1234,"mv":987}
 *
 * STRUCTUUR:
 *   main/
 *     main.c              ← dit bestand
 *     CMakeLists.txt
 *     idf_component.yml
 *   managed_components/
 *     myADC/
 *       include/myADC.h   ← include pad
 *       myADC.c
 * =============================================================
 */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

/*
 * Include van de myADC managed component.
 * ESP-IDF voegt managed_components/myADC/include automatisch
 * toe aan de include-paden, dus "myADC.h" volstaat.
 */
#include "myADC.h"

/* ─── Constanten ─────────────────────────────────────────── */
#define TAG "MAIN"
#define LIGHT_SENSOR_GPIO ADC_CHANNEL_6 /* GPIO34 op ESP32  */
#define MEETINTERVAL_MS 1000            /* 1 meting/seconde */

/* ─── app_main ────────────────────────────────────────────── */
void app_main(void)
{
    ESP_LOGI(TAG, "Lichtsensor project gestart (USB-serieel modus)");
    ESP_LOGI(TAG, "Start pc/server.py op je pc en open http://localhost:5000");

    /* ADC initialiseren via de myADC managed component */
    myADC_setup(LIGHT_SENSOR_GPIO);

    while (1)
    {
        /* Lees ruwe ADC-waarde (0–8191 bij 13-bit) */
        int raw = myADC_getValue(LIGHT_SENSOR_GPIO);

        /* Lees gekalibreerde spanning in millivolt */
        int mv = myADC_getMiliVolt(LIGHT_SENSOR_GPIO);

        /*
         * Stuur JSON-regel naar de pc via USB-serieel (UART0).
         * printf() schrijft naar UART0, zichtbaar als seriële
         * poort (COMx / /dev/ttyUSBx) op je pc.
         * Het Python-script leest deze regels en stuurt ze
         * door naar de browser.
         */
        printf("{\"raw\":%d,\"mv\":%d}\n", raw, mv);

        /* Wacht 1 seconde tot de volgende meting */
        vTaskDelay(pdMS_TO_TICKS(MEETINTERVAL_MS));
    }
}