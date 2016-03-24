#include <Arduino.h>
#include "./SerialJson/SerialJson.h"
#include "./Configuration/Configuration.h"
#include "./KeopsAPI/KeopsAPI.h"
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

SerialJson sJson;
ESP8266WiFiMulti WiFiMulti;
Configuration conf;
bool isThisPyramidRegistred = false;
const char* host = "keops.herokuapp.com";

//const char* fingerprint = "F5 69 8C CA 29 68 5E 47 26 38 C5 1A 18 F1 8A 6A EA 60 56 4D";

void setup(){
  Serial.begin(115200);
  sJson.init(115200);
  //TODO CHECK IF THIS PYRAMID IS REGISTRED
}

void loop(){

  if(sJson.isThereNewJson()){
    conf.loadFromJson(sJson.getJson());
    Serial.println("NewConf["+conf.getConfigJson()+"]");
  }else{
    Serial.println("No, there is not a new configuration");
  }

  if(conf.isInitialized()){
      Serial.println("INITIALIZED");
      String ssid     =  String(conf.getConfigurationValue("ssid"));
      Serial.println(ssid);
      String password =  String(conf.getConfigurationValue("password"));
      Serial.println(password);
      WiFiMulti.addAP(ssid.c_str(), password.c_str());
      int attempts = 0;
      while (WiFiMulti.run() != WL_CONNECTED && attempts < 10) {
        delay(500);
        attempts ++;
        Serial.println(".");
        Serial.println(attempts);
      }
      if(attempts < 10 ){ // I got connected
          KeopsAPI keopsAPI;
          String result = keopsAPI.getAllPyramidsRegistered();
          /**
          HTTPClient http;
          Serial.print("[HTTP] begin...\n");
          http.begin("https://keops.herokuapp.com/pyramids", "F5 69 8C CA 29 68 5E 47 26 38 C5 1A 18 F1 8A 6A EA 60 56 4D"); //HTTPS sha fingerprint
          Serial.print("[HTTP] GET...\n");
          int httpCode = http.GET();
          // httpCode will be negative on error
          if(httpCode > 0) {
              // HTTP header has been send and Server response header has been handled
              Serial.printf("[HTTP] GET... code: %d\n", httpCode);
              if(httpCode == HTTP_CODE_OK) {
                  String payload = http.getString();
                  Serial.println(payload);
              }
          } else {
              Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
          }

          http.end();
          **/
          delay(1000);

      }else{
        Serial.println("Max attempts");
      }
  }

}
