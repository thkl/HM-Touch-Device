
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>      // this is needed even tho we aren't using it
#include <EEPROM.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ILI9341.h> // Hardware-specific library
#include <Adafruit_STMPE610.h>

#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <ESP8266mDNS.h>

#include <WiFiManager.h>

#define HOSTNAME "HM_TOUCH-OTA-"
#define useProximity false


#include "HMControl.h"

#include "PageManager.h"
#include "ConfigurationServer.h"
#include "WebResource.h"

// Pins for the ILI9341
#define TFT_DC 15
#define TFT_CS 0

#define TS_MINX 3800
#define TS_MAXX 100
#define TS_MINY 100
#define TS_MAXY 3750

#define PRX_Pin 4

// pins for the touchscreen
#define STMPE_CS 16
#define STMPE_IRQ 4

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);
WebResource webResource;

GfxUi ui = GfxUi(&tft);

boolean   DEEP_SLEEP = false;
int       AWAKE_TIME = 5;
const int UPDATE_INTERVAL_SECS = 30;
TS_Point  lastPoint;
boolean   wasTouched;
uint16_t voltage;
const char* host = "hm_touch-webupdate";


long lastUpdate = millis();
long lastCheck = millis();
boolean PRX_ON = false;
boolean SCR_OFF = false;

//WiFiManager
//Local intialization. Once its business is done, there is no need to keep it around
WiFiManager wifiManager;
HMDeviceHandler hmDeviceHandler;
PageManager pageManager;
ConfigurationServer configurationserver;

void configModeCallback (WiFiManager *myWiFiManager);
void updateData();
void drawProgress(uint8_t percentage, String text);
void drawSeparator(uint16_t y);
void downloadCallback(String filename, int bytesDownloaded, int bytesTotal);
void downloadFile(String fileName);
void updateStatus();


ProgressCallback _downloadCallback = downloadCallback;

void setup() {
  Serial.begin(115200);

  if (! ts.begin()) {
    Serial.println("STMPE not found?");
  }



  ts.writeRegister8(STMPE_GPIO_DIR, _BV(2));
  ts.writeRegister8(STMPE_GPIO_ALT_FUNCT, _BV(2));
  // backlight on
  ts.writeRegister8(STMPE_GPIO_SET_PIN, _BV(2));

  tft.begin();
  tft.fillScreen(ILI9341_BLACK);
  tft.setFont(&ArialRoundedMTBold_14);
  ui.setTextColor(ILI9341_CYAN, ILI9341_BLACK);
  ui.setTextAlignment(CENTER);
  ui.drawString(120, 160, "Connecting to WiFi");

  // Uncomment for testing wifi manager
  //wifiManager.resetSettings();
  wifiManager.setAPCallback(configModeCallback);

  //or use this for auto generated name ESP + ChipID
  wifiManager.autoConnect();

  //Manual Wifi
  //WiFi.begin(WIFI_SSID, WIFI_PWD);

  // OTA Setup
  String hostname(HOSTNAME);
  hostname += String(ESP.getChipId(), HEX);
  WiFi.hostname(hostname);
  ArduinoOTA.setHostname((const char *)hostname.c_str());
  ArduinoOTA.begin();
  SPIFFS.begin();
  MDNS.begin(host);

  configurationserver.init();
  configurationserver.wifiManager = &wifiManager;

  MDNS.addService("http", "tcp", 80);

  if (configurationserver.loadConfigFile("config.json",false) == true) {
    Serial.print("CCU Ip is:");
    Serial.print(configurationserver.ccuIP);
    hmDeviceHandler.ccuIP = configurationserver.ccuIP;
    pageManager.init(&tft,&ui);
    Serial.print("Items in Config :");
    int numItems = configurationserver.numberOfItems;
    Serial.print(numItems);
    configurationserver.initControls(&tft,&ui,&hmDeviceHandler,&pageManager);

  if (useProximity == true) {
    pinMode(PRX_Pin, INPUT_PULLUP);
  }


  for (int i=10; i<37; i++  ) {
    downloadFile("therm_" + String(i) +".bmp");
  }
  downloadFile("btnplus.bmp");
  downloadFile("btnminus.bmp");
  downloadFile("btnboost.bmp");
  downloadFile("btnboost_on.bmp");
  downloadFile("btnmanu.bmp");
  downloadFile("therm_off.bmp");

  tft.fillScreen(ILI9341_BLACK);
  updateStatus();
  pageManager.updatePage();
} else {
  tft.fillScreen(ILI9341_BLACK);
  ui.setTextAlignment(CENTER);
  tft.setFont(&ArialRoundedMTBold_14);
  tft.setTextColor(ILI9341_CYAN);
  ui.drawString(120, 28, "Missing configuration");
  ui.drawString(120, 45, "Please upload via WebIF");
}
}


void downloadCallback(String filename, int bytesDownloaded, int bytesTotal) {
  int percentage = 100 * bytesDownloaded / bytesTotal;
  if (percentage == 0) {
    tft.fillRect(20,160,300,20,0x00000);
    ui.drawString(20, 160, filename);
    ui.drawProgressBar(10, 165, 240 - 20, 15, 0, ILI9341_WHITE, ILI9341_BLUE);
  }
  if (percentage % 5 == 0) {
    ui.setTextAlignment(CENTER);
    ui.setTextColor(ILI9341_CYAN, ILI9341_BLACK);
    ui.drawProgressBar(10, 165, 240 - 20, 15, percentage, ILI9341_WHITE, ILI9341_BLUE);
  }

}

void downloadFile(String fileName) {
  ui.drawString(120, 45, "Downloading ...");
  webResource.downloadFile("https://raw.githubusercontent.com/thkl/HM-Touch-Device/master/src/images/" + fileName, "/gfx/" + fileName, _downloadCallback,"CCAA484866460E91532C9C7C232AB1744D299D33");
}

void screenManagerCheck(){

	// Pin is Low
   if ((SCR_OFF == true) && (digitalRead(PRX_Pin) == 0)) {
	   // Switch ON
	   Serial.println("Switch On");
	     // we'll use STMPE's GPIO 2 for backlight control
		 ts.writeRegister8(STMPE_GPIO_DIR, _BV(2));
		 ts.writeRegister8(STMPE_GPIO_ALT_FUNCT, _BV(2));
		 // backlight on
		 ts.writeRegister8(STMPE_GPIO_SET_PIN, _BV(2));
	     // Run a Query
	     pageManager.updatePage();
		 SCR_OFF = false;
		 lastCheck = millis();
   }


   if ((SCR_OFF==false) && (millis() - lastCheck > 1000 * UPDATE_INTERVAL_SECS)) {
		ts.writeRegister8(STMPE_GPIO_CLR_PIN, _BV(2)); // backlight off
		ts.writeRegister8(STMPE_INT_STA, 0xFF);
		SCR_OFF = true;
		// Jump Back
		pageManager.jumpBack();
	}

}

void loop() {
configurationserver.handle();

  if (useProximity == true) {
	    screenManagerCheck();
  }

  if (! ts.bufferEmpty()) {

      if (ts.touched()) {
        lastPoint = ts.getPoint();
        wasTouched = true;
        lastPoint.x = map(lastPoint.x, TS_MINX, TS_MAXX, 0, tft.width());
        lastPoint.y = map(lastPoint.y, TS_MINY, TS_MAXY, 0, tft.height());
      }
    }

    if ((!ts.touched()) && (wasTouched)) {
         Serial.print("X = "); Serial.print(lastPoint.x);
         Serial.print("\tY = "); Serial.print(lastPoint.y);
         Serial.print("\tPressure = "); Serial.println(lastPoint.z);
         pageManager.touched(lastPoint);
		 wasTouched = false;
    }

    if ((millis() - lastUpdate > 1000 * UPDATE_INTERVAL_SECS)) {
	   updateStatus();
    }

    if ((pageManager.currentPage != 0) && (millis() - pageManager.lastDetailPageUpdate > 1000 * 20)) {
      pageManager.jumpBack();
    }


}

void updateStatus() {
    lastUpdate = millis();
	if (SCR_OFF==false) {
		  // Only update Main Page periodically
	    if (pageManager.currentPage == 0) {
	      pageManager.updatePage();
		}
	}
}

// Called if WiFi has not been configured yet
void configModeCallback (WiFiManager *myWiFiManager) {
  ui.setTextAlignment(CENTER);
  tft.setFont(&ArialRoundedMTBold_14);
  tft.setTextColor(ILI9341_CYAN);
  ui.drawString(120, 28, "Wifi Manager");
  ui.drawString(120, 42, "Please connect to AP");
  tft.setTextColor(ILI9341_WHITE);
  ui.drawString(120, 56, myWiFiManager->getConfigPortalSSID());
  tft.setTextColor(ILI9341_CYAN);
  ui.drawString(120, 70, "To setup Wifi Configuration");
}



// Progress bar helper
void drawProgress(uint8_t percentage, String text) {

  ui.setTextAlignment(CENTER);
  ui.setTextColor(ILI9341_CYAN, ILI9341_BLACK);
  tft.fillRect(0, 140, 240, 45, ILI9341_BLACK);
  ui.drawString(120, 160, text);
  ui.drawProgressBar(10, 165, 240 - 20, 15, percentage, ILI9341_WHITE, ILI9341_BLUE);
}


void drawSeparator(uint16_t y) {
  tft.drawFastHLine(10, y, 240 - 2 * 10, 0x4228);
}
