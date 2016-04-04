#include <Arduino.h>
#include <ArduinoJson.h>
#include <EEPROM.h>

class Configuration{
public:
  void loadFromJson(String pConfigJson);
  String getConfigurationValue(String pName);
  bool  isInitialized();
  String getConfigJson();
  void addData(String label, String value);
  void saveToEEprom();
  int loadFromEEprom();

private:
  String _configJson;
  JsonObject* _root;
  bool _initialized = false;
  void parseJsonConfig();
};
