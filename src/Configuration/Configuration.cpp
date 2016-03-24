#include "Configuration.h"
#include "ArduinoJson.h"

bool  Configuration::isInitialized(){
  return _initialized;
}

void Configuration::loadFromJson(String pConfigJson){
  _configJson = pConfigJson;
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
