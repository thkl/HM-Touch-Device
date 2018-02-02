This is a implementation of a Homematic touch display. You may setup controls for multiple ccu devices. The display will communicate to your ccu over wifi.

Parts :
 * ESP8266 Board (WEMOS D1 mini)
 * TFT Touch Display (Adafruit FeatherWing 2,2" or similar)

Connecting the board to the display:
  * D1 -> TFT_DC
  * D2 -> TFT_CS
  * D3 -> STMPE_CS
  * D4 -> STMPE_IRQ
  * D5 -> SCK
  * D6 -> MISO
  * D7 -> MOSI

Config :
 see demo.json
