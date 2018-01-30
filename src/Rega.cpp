#include <ESP8266HTTPClient.h>

#include "Rega.h"
#include <WiFiClient.h>

  String Rega::runScript(String ccuIP, String script) {
    HTTPClient http;
    
    String result;
    Serial.print("Connecting to CCU at ");
    Serial.println(ccuIP);
    Serial.print("running Script ");
    Serial.println(script);
    

    http.begin(ccuIP,8181,"/tclrega.exe");
    int httpCode = http.POST(script);
    if(httpCode > 0) {
       if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                Serial.print("Result ");
                Serial.println(payload);
                Serial.print("Index ");
                int index = payload.indexOf("<xml><exec>");
                Serial.println(index);
                return payload.substring(0, index);
       }
    }
    return "";
  }
  



