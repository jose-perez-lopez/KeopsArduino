#include <Arduino.h>
#include <ArduinoJson.h>


class Configuration{
public:
  void loadFromJson(String pConfigJson);
  String getConfigurationValue(String pName);
  bool  isInitialized();
  String getConfigJson();

private:
  String _configJson;  
  JsonObject* _root;
  bool _initialized = false;
};
