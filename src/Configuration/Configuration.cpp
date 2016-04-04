#include "Configuration.h"


bool  Configuration::isInitialized(){
  return _initialized;
}

void Configuration::loadFromJson(String pConfigJson){
  _configJson = pConfigJson;
  parseJsonConfig();
}

void Configuration::parseJsonConfig(){
  static StaticJsonBuffer<200> jsonBuffer;
  static JsonObject& root = jsonBuffer.parseObject(_configJson);
  _root = &root;
  _initialized = true;
  return;
}

String Configuration::getConfigurationValue(String pName){
   return   (*_root)[pName].asString();
}

String Configuration::getConfigJson(){
  return _configJson;
}

void Configuration::addData(String label, String value){
  (*_root)[label] = value;
  String json;
  (*_root).printTo(json);
  _configJson = json;
  Serial.println(json);
}

void Configuration::saveToEEprom(){
  Serial.println("Saving conf to EEprom:");
  Serial.println(_configJson);
  int i =0;
  for(; i < _configJson.length(); i++){
    EEPROM.write(i,_configJson.charAt(i));
  }
  EEPROM.write(i,0);
  bool result = EEPROM.commit();
}

int Configuration::loadFromEEprom(){
  String conf = "";
  Serial.println("Loading conf from EEprom:");
  int i = 0;
  byte value = EEPROM.read(i);
  if(char(value)!='{'){
    return -1;
  }
  do{
    value = EEPROM.read(i);
    conf += char(value);
    i++;
  }while(value!=0 && i < 200);
  _configJson = conf;
  parseJsonConfig();
  return 0;
}
