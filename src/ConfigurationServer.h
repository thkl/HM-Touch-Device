#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ArduinoJson.h>
#include "HMControl.h"
#include <Adafruit_ILI9341.h>
#include "PageManager.h"

class ConfigurationServer {

public:
  void init();
  void handle();
  int numberOfItems;
  void initControls(Adafruit_ILI9341 * _tft, GfxUi * _ui,HMDeviceHandler * _deviceHandler, PageManager * pagemanager);

  bool loadConfigFile(String filename,bool test);
  WiFiManager * wifiManager;
  String ccuIP;

private :
  ESP8266WebServer httpServer;
  ESP8266HTTPUpdateServer httpUpdater;
  JsonVariant _jsonConfig;
// updater

  const char* update_path = "/firmware";
  const char* update_username = "admin";
  const char* update_password = "admin";

  void handleNotFound();
  void handleRoot();
  void handleReset();
  void handleConfigUpload();
  void handleCondigForm();
  void handleSysReset();
};
