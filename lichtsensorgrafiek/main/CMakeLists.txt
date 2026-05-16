# =============================================================
# BESTAND: CMakeLists.txt  (in de map main/)
# ROL:     Vertelt ESP-IDF welke bronbestanden gecompileerd
#          worden en welke componenten nodig zijn.
#          WiFi en webserver zijn verwijderd - data gaat via
#          USB-serieel naar het Python-script op de pc.
# =============================================================

idf_component_register(
    SRCS
        "main.c"         # Hoofdprogramma: ADC uitlezen + serieel sturen
        "myADC.c"        # Jouw ADC-component (ongewijzigd)

    INCLUDE_DIRS "."     # Zoek headers in dezelfde map

    REQUIRES
        esp_adc          # ADC oneshot + kalibratie
)