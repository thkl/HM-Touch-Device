#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include "ConfigurationServer.h"
#include "HMDimmer.h"
#include "HMSwitch.h"
#include "HMThermostat.h"
#include "HMClock.h"

#define tmpConfigFile "/tmp_config.json"
#define bufferSize 2048
const char HTTP_CONFIG_OPTIONS[] PROGMEM  = "<form action=\"/resetwifi\"method=\"get\"><button>Reset WiFi</button></form><br/><form action=\"/resetdevice\"method=\"get\"><button>Reboot</button></form><br/><form action=\"/config\"method=\"get\"><button>Upload Config</button></form><br/><form action=\"/cleancache\"method=\"post\"><button>Clean Cache</button></form><br /><br /><form action=\"/sysreset\"method=\"post\"><button>Reset to default</button></form>";
const char HTTP_CONFIG_HOME[] PROGMEM  = "<form action=\"/\"method=\"get\"><button>Home</button></form>";
const char HTTP_CONFIG_UPLOAD[] PROGMEM  = "<form method=\"post\" enctype=\"multipart/form-data\"><input type=\"file\" name=\"name\"><input class=\"button\" type=\"submit\" value=\"Upload\"></form>";


File fsUploadFile;

void ConfigurationServer::init() {
  httpUpdater.setup(&httpServer, update_path, update_username, update_password);
  httpServer.begin();
  httpServer.on("/",  [this](){ sendHTMLResponseMessage(FPSTR(HTTP_CONFIG_OPTIONS));});
  httpServer.on("/resetwifi",  [this](){ wifiManager->resetSettings(); handleReset();});
  httpServer.on("/resetdevice", [this](){ handleReset(); });
  httpServer.on("/config", HTTP_GET, [this](){sendHTMLResponseMessage(FPSTR(HTTP_CONFIG_UPLOAD));});
  httpServer.on("/config",HTTP_POST, [this]{handleConfigUpload();});
  httpServer.on("/sysreset", [this]{handleSysReset();});
  httpServer.on("/cleancache", [this]{handleCacheCleaning();});
  httpServer.on("/fail", [this]{sendHTMLResponseMessage("Sorry, the last operation failed. :(");});
  httpServer.on("/success", [this]{sendHTMLResponseMessage("Great success.");});
  httpServer.onNotFound ( [this](){ sendHTMLResponseMessage("File not found.");});

  Serial.println("Mounting FS...");

  if (!SPIFFS.begin()) {
        Serial.println("Failed to mount file system");
        return;
  }

}

void ConfigurationServer::handleCacheCleaning(){
  if (SPIFFS.begin()) {
    Dir dir = SPIFFS.openDir("/gfx");
    while (dir.next()) {
      Serial.print(dir.fileName());
      SPIFFS.remove(dir.fileName());
    }
    delay(2000);
    handleReset();
  } else {
    httpServer.sendHeader("Location","/fail");      // Redirect the client to the success page
    httpServer.send(303);
  }
}

// deletes the SPI File System / wifi settings / and performs a reset
void ConfigurationServer::handleSysReset() {
  if (!SPIFFS.begin()) {
        Serial.println("Failed to mount file system");
        return;
  } else {
    SPIFFS.format();
    delay(2000);
    wifiManager->resetSettings();
    delay(2000);
    handleReset();
  }
}

bool ConfigurationServer::loadConfigFile(String filename,bool test) {
  // check Valid Json here
  bool result = false;
  Serial.print("Trying to open config at ");
  Serial.println(filename);
  File file = SPIFFS.open(filename,"r");
  if (file) {
    StaticJsonBuffer<bufferSize> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(file);
    if (!root.success()) {
      Serial.println(F("Failed to read file"));
    } else {
    // Get CCU IP     // get Items Array
      String _ccuIP = root["ccu_ip"];
      JsonArray& _items = root["items"];
      // Check if there is a ip and items in config
      if (_ccuIP && _items.success()) {
        if (!test) {
          // if not test set CCUIP and Array of Items
          _jsonConfig = root["items"];
          ccuIP = _ccuIP;
          numberOfItems = _jsonConfig.size();
        }
        result = true;
      } else {
        numberOfItems = 0;
        Serial.println("Missing Data in config");
      }
  }
  file.close();
} else {
  Serial.print(filename);
  Serial.println(" not found.");
}
return result;
}

void ConfigurationServer::initControls(Adafruit_ILI9341 * _tft, GfxUi * _ui,HMDeviceHandler * _deviceHandler, PageManager * pagemanager) {
  Serial.println("ConfigurationServer::initControls");
  int itemNumber = _jsonConfig.size();
  for (int number=0; number<itemNumber; number++) {
    Serial.print(number);
    Serial.println("We have that number of items");
    JsonObject &item = _jsonConfig[number];

    String items_name = item["name"]; // "Clock"
    String items_type = item["type"]; // "HMClock"
    String items_adress = item["adress"]; // "LEQ0440411"

    JsonArray &items_rect = item["rect"];
    int items_x = items_rect[0]; // 0
    int items_y = items_rect[1]; // 10
    int items_w = items_rect[2]; // 230
    int items_h = items_rect[3]; // 10

    Serial.print("Type is:");
    Serial.println(items_type);

    if (items_type == "HMClock") {
        HMClock *result = new HMClock;
        result->init(_tft,_ui,_deviceHandler);
        result->ctrl_name = items_name;
        result->setRect(items_x,items_y,items_w,items_h);
        int tzOffset = item["tzoffset"];
        result->setTimeZone(tzOffset);
        int fsize = item["fontsize"];
        result->fontSize = fsize;
        pagemanager->addControl(result,number+1);
    }

    if (items_type == "HMThermostat") {
        HMThermostat * result = new HMThermostat;
        result->init(_tft,_ui,_deviceHandler);
        result->ctrl_name = items_name;
        result->adress = items_adress;
        result->setRect(items_x,items_y,items_w,items_h);
        pagemanager->addControl(result,number+1);
    }

    if (items_type == "HMSwitch") {
        HMSwitch *result = new HMSwitch;
        result->init(_tft,_ui,_deviceHandler);
        result->ctrl_name = items_name;
        result->adress = items_adress;
        result->setRect(items_x,items_y,items_w,items_h);
        pagemanager->addControl(result,number+1);
    }

    if (items_type == "HMDimmer") {
        HMDimmer *result = new HMDimmer;
        result->init(_tft,_ui,_deviceHandler);
        result->ctrl_name = items_name;
        result->adress = items_adress;
        result->setRect(items_x,items_y,items_w,items_h);
        pagemanager->addControl(result,number+1);
    }
  }
}

void ConfigurationServer::handleConfigUpload() {
  Serial.println("Upload Config");
  HTTPUpload& upload = httpServer.upload();
  Serial.println(upload.status);

  if(upload.status == UPLOAD_FILE_START){
      Serial.println("Create SPIFSS Object");

      if (SPIFFS.exists(tmpConfigFile)) {
        SPIFFS.remove(tmpConfigFile);
      }

      fsUploadFile = SPIFFS.open(tmpConfigFile, "w");
  } else

  if(upload.status == UPLOAD_FILE_WRITE) {
    Serial.println("UPLOAD_FILE_WRITE");
    if (!fsUploadFile){
      fsUploadFile = SPIFFS.open(tmpConfigFile, "w");
    }
      fsUploadFile.write(upload.buf, upload.currentSize); // Write the received bytes to the file
      Serial.print(upload.currentSize);
      Serial.println("bytes written");
  } else

  if(upload.status == UPLOAD_FILE_END){
    Serial.println("UPLOAD_FILE_END");
    if(!fsUploadFile) {                                    // If the file was successfully created
      Serial.println("create file");

      if (SPIFFS.exists(tmpConfigFile)) {
        Serial.println("remove old file");
        SPIFFS.remove(tmpConfigFile);
      }

      fsUploadFile = SPIFFS.open(tmpConfigFile, "w");
      if (!fsUploadFile) {
        Serial.println("Cannot create file");

      } else {
        Serial.println("Write data");
        fsUploadFile.write(upload.buf, upload.currentSize);
      }
    }

    fsUploadFile.close();

    if (loadConfigFile(tmpConfigFile,true) == true) {
        Serial.print("Seems legit");
        SPIFFS.remove("config.json");
        // seems to be a valid file rename it and perform a reset
        SPIFFS.rename(tmpConfigFile,"config.json");
        handleReset();
    }

    httpServer.sendHeader("Location","/success.html");      // Redirect the client to the success page
    httpServer.send(303);
 }
}

void ConfigurationServer::handleReset() {
  String page = "";
  page += "<meta http-equiv='refresh' content='20; URL=/'>";
  page += F("Module will reset in a few seconds.");
  httpServer.sendHeader("Content-Length", String(page.length()));
  httpServer.send(200, "text/html", page);
  delay(5000);
  ESP.restart();
  delay(2000);
}

void ConfigurationServer::handle() {
  httpServer.handleClient();
}


// We reuse some stuff from WifiManager
void ConfigurationServer::sendHTMLResponseMessage(String message) {
  String page = FPSTR(HTTP_HEAD);
   page.replace("{v}", "HM-Touch Device");
   page += FPSTR(HTTP_SCRIPT);
   page += FPSTR(HTTP_STYLE);
   page += FPSTR(HTTP_HEAD_END);
   page += "<h1>HM-Touch Device</h1>";
   page += F("<h3>Configuration</h3>");
   page += FPSTR(HTTP_CONFIG_HOME);
   page += "<br />";
   page += message;
   page += "<br />";
   page += FPSTR(HTTP_END);
   httpServer.sendHeader("Content-Length", String(page.length()));
   httpServer.send ( 200, "text/html", page);
}
