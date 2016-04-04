#include <Arduino.h>
#include "./SerialJson/SerialJson.h"
#include "./Configuration/Configuration.h"

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <EEPROM.h>

SerialJson sJson;
Configuration conf;
bool isThisPyramidAlreadyRegistred = false;
ESP8266WiFiMulti WiFiMulti;
HTTPClient http;
int eeAddress = 0;
char HTTPSshaFingerprint[] = "F5 69 8C CA 29 68 5E 47 26 38 C5 1A 18 F1 8A 6A EA 60 56 4D";
char apiURL[] = "https://keops.herokuapp.com/pyramids";
String pyramidId;
#define LED     D0
int loop_count = 0;
int pin_base = 12;

void splitData(int result[],String data ){
  data = data + ",";
  int i=0;
  do{
    Serial.print("**");
    Serial.print(data);
    Serial.print("_[");
    Serial.print(i);
    Serial.print("]=[");
    result[i] = data.substring(0,data.indexOf(',')).toInt();
    Serial.print(data.substring(0,data.indexOf(',')).toInt());
    Serial.print("]_");
    data.remove(0,data.indexOf(',')+1);
    Serial.println(data);
    i++;
  }while(data.indexOf(',') != -1 );

    Serial.print("&&");
}


int APIgetPyramidData(String id,String *responseBody){
        Serial.println("Getting pyramid data ["+id+"]");
        http.begin(String(apiURL)+"/"+id, HTTPSshaFingerprint); //HTTPS sha fingerprint
        digitalWrite(LED, HIGH);   // turn the LED off.
        int httpCode = http.GET();
        Serial.println(httpCode);
        digitalWrite(LED, LOW);
        if(httpCode > 0) {
                if(httpCode == HTTP_CODE_OK) {
                        *responseBody = http.getString();
                }
        } else {
                Serial.print("HTTP REPONSE");
                Serial.println(httpCode);
                return -1; //error
        }
        http.end();
        return 0;
}


int APIregisterPyramid(String *id){
        http.begin(apiURL, HTTPSshaFingerprint); //HTTPS sha fingerprint
        int httpCode = http.POST("");
        if(httpCode > 0) {
                if(httpCode == HTTP_CODE_CREATED) {
                        String responseBody = http.getString();
                        *id = responseBody.substring(responseBody.indexOf(':')+1,responseBody.indexOf('}'));
                }
        } else {
                return -1; //error
        }
        http.end();
        return 0;
}


/**
/ Set up
**/
void setup(){
        sJson.init(115200);
        Serial.begin(115200);
        EEPROM.begin(512);
        pinMode(LED, OUTPUT);
        for(int i=pin_base; i < pin_base+3; i++){
          pinMode(i,OUTPUT);
          digitalWrite(i,LOW);
        }
}

void loop(){
        loop_count++;
        Serial.flush();
        Serial.println("INIT CICLE:"+ String(loop_count));
        // Checking serial port to get wifi connection data
        if(loop_count==1) {
                int result = conf.loadFromEEprom();
                pyramidId = conf.getConfigurationValue("pyramidId");
                isThisPyramidAlreadyRegistred = true;
        }

        if(sJson.isThereNewJson()) {
                Serial.println("New Json");
                conf.loadFromJson(sJson.getJson());
                if(isThisPyramidAlreadyRegistred) {
                        conf.addData("pyramidId", pyramidId );
                        conf.saveToEEprom();
                }
                Serial.println("NewConf["+conf.getConfigJson()+"]");
        }

        if(conf.isInitialized()) {
                Serial.println("Initialized:");
                Serial.println(pyramidId);
                String ssid     =  String(conf.getConfigurationValue("ssid"));
                String password =  String(conf.getConfigurationValue("password"));
                Serial.println("ssid_"+ssid);
                Serial.println("password_"+password);
                WiFiMulti.addAP(ssid.c_str(), password.c_str());
                int attempts = 0;
                while (WiFiMulti.run() != WL_CONNECTED && attempts < 10) {
                        delay(500);
                        attempts++;
                        Serial.print(".");
                        Serial.print(attempts);
                }
                if(attempts < 10 ) { // I got connected
                        if(!isThisPyramidAlreadyRegistred) {
                                Serial.println("Registering pyramid");
                                if(APIregisterPyramid(&pyramidId)==0) {
                                        Serial.println("Success Registering pyramid");
                                        isThisPyramidAlreadyRegistred = true;
                                        conf.addData("pyramidId", pyramidId );
                                        conf.saveToEEprom();
                                }else{
                                        Serial.println("Error Registering pyramid");
                                }
                        }
                        if(isThisPyramidAlreadyRegistred) {
                                String payload;
                                Serial.println("Getting this pyramid data");
                                int result = APIgetPyramidData(pyramidId,&payload);
                                Serial.println(result);
                                Serial.print("End getting pyramid data:");
                                Serial.println(payload);
                                Serial.println(payload.indexOf('['));

                                String statusData = payload.substring(payload.indexOf("status\":\"")+9,payload.indexOf("\"}"));
                                Serial.print("-->");
                                Serial.println(statusData);
                                Serial.flush();
                                int statusArray[3];
                                splitData(statusArray,statusData);
                                for(int i=0;i < 3; i++){
                                  Serial.print("!!");
                                  Serial.println(statusArray[i]);
                                  if(statusArray[i]!=0){
                                     Serial.print("PIN:");
                                     Serial.print(i+pin_base);
                                     Serial.print(":");
                                     Serial.println("HIGH");
                                     digitalWrite(i+pin_base, HIGH);
                                  }else{

                                     Serial.print("PIN:");
                                     Serial.print(i+pin_base);
                                     Serial.print(":");
                                     Serial.println("LOW");
                                      digitalWrite(i+pin_base, LOW);
                                  }
                                  Serial.flush();
                                }
                        }

                }else{
                        Serial.println("Max attempts");
                }
        }
        delay(2000);
}

/**
   Serial.println("1");
   conf.loadFromJson("{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}");
   Serial.println("2");
   conf.saveToEEprom();
   Serial.println("3");
   int result = conf.loadFromEEprom();
    Serial.println(conf.getConfigJson());
   Serial.println("4");
   Serial.println(conf.getConfigurationValue("sensor"));
   delay(1000);
 **/
