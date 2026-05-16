"""
=============================================================
BESTAND: server.py
ROL:     Draait op je pc. Leest JSON-data van de ESP32 via
         de USB-seriële poort en host een webpagina op
         http://localhost:5000

         De browser vraagt elke seconde /data op.
         Dit script levert dan de meest recente meting terug.

GEBRUIK:
  1. Installeer dependencies (eenmalig):
       pip install flask pyserial

  2. Pas SERIEEL_POORT aan (zie hieronder)

  3. Zorg dat de ESP32 aangesloten en geflasht is, start dan:
       python server.py

  4. Open http://localhost:5000 in je browser
=============================================================
"""

import serial
import threading
import json
import time
from flask import Flask, jsonify, send_from_directory
import os

# ─── INSTELLING: PAS DIT AAN ────────────────────────────────
# Windows:  "COM3"  of  "COM4"  (zie Apparaatbeheer)
# Linux:    "/dev/ttyUSB0"  of  "/dev/ttyACM0"
# macOS:    "/dev/cu.usbserial-0001"
SERIEEL_POORT  = "COM3"
BAUDRATE       = 115200   # Moet overeenkomen met ESP-IDF standaard
# ─────────────────────────────────────────────────────────────

app = Flask(__name__, static_folder="web")

# Gedeelde variabele: meest recente sensorwaarden
laatste_meting = {"raw": 0, "mv": 0}
meting_lock    = threading.Lock()


# ─── Achtergrondthread: seriële poort uitlezen ───────────────
def serieel_lezen():
    """
    Draait continu in een aparte thread.
    Elke regel van de ESP32 is een JSON-object: {"raw":1234,"mv":987}
    Bij een parsefout wordt de regel gewoon overgeslagen.
    """
    global laatste_meting

    while True:
        try:
            print(f"[SERIEEL] Verbinding maken met {SERIEEL_POORT}...")
            with serial.Serial(SERIEEL_POORT, BAUDRATE, timeout=2) as ser:
                print(f"[SERIEEL] Verbonden met {SERIEEL_POORT} @ {BAUDRATE} baud")
                while True:
                    lijn = ser.readline().decode("utf-8", errors="ignore").strip()
                    if not lijn:
                        continue  # Lege regel overslaan

                    try:
                        data = json.loads(lijn)
                        # Alleen verwerken als de verwachte velden aanwezig zijn
                        if "raw" in data and "mv" in data:
                            with meting_lock:
                                laatste_meting = data
                            print(f"[DATA] raw={data['raw']}  mv={data['mv']} mV")
                    except json.JSONDecodeError:
                        # ESP32 log-berichten (die beginnen met "I (") negeren
                        pass

        except serial.SerialException as fout:
            print(f"[FOUT] Seriële poort niet beschikbaar: {fout}")
            print("       Controleer de poort en sluit de kabel opnieuw aan.")
            print("       Nieuwe poging in 3 seconden...")
            time.sleep(3)


# ─── HTTP-routes ─────────────────────────────────────────────

@app.route("/")
def index():
    """Stuurt de hoofdpagina (index.html uit de map 'web/') terug."""
    return send_from_directory("web", "index.html")


@app.route("/data")
def data():
    """
    Stuurt de meest recente sensorwaarden terug als JSON.
    Wordt elke seconde aangeroepen door het JavaScript in de browser.
    """
    with meting_lock:
        meting = dict(laatste_meting)
    return jsonify(meting)


# ─── Startpunt ───────────────────────────────────────────────
if __name__ == "__main__":
    # Maak de 'web'-map aan als die nog niet bestaat
    os.makedirs("web", exist_ok=True)

    # Start de seriële lezer in een daemon-thread
    # (daemon=True zorgt dat de thread stopt als het script stopt)
    thread = threading.Thread(target=serieel_lezen, daemon=True)
    thread.start()

    print("=" * 50)
    print("  Lichtsensor Dashboard")
    print("  Open http://localhost:5000 in je browser")
    print("=" * 50)

    # Start de Flask-webserver (debug=False voor stabiliteit)
    app.run(host="0.0.0.0", port=5000, debug=False)
