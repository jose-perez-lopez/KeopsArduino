#include "SerialJson.h"

void SerialJson::init(int bps){
  Serial.begin(bps);
}

String SerialJson::getJson(){
  return _incomingMessage;
}

bool SerialJson::isThereNewJson(){
bool result = false;
_incomingMessage = "";
_bracketCount = 0;
if (Serial.available() > 0) {
    unsigned long previousMillis = millis();
    do {
        Serial.flush();
        _incomingByte = Serial.read();
        _incomingChar = char(_incomingByte);
        _incomingMessage += char(_incomingByte);
        if (_incomingChar == '{') {
          _bracketCount++;
          result = true;
        }
        if (_incomingChar == '}') _bracketCount--;
        // say what you got:
        Serial.print(".");
        Serial.flush();
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillis >=  _interval ) {
          Serial.println("[Timeout]");
          result = false;
          _incomingMessage = "";
          break;
        }
        } while (_bracketCount > 0);
    return result;
}
}
