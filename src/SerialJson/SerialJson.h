#include <Arduino.h>

class SerialJson
{
  public:
    void init(int bps);
    bool isThereNewJson();
    String getJson();
  private:
    String _incomingMessage = "";
    const long _interval = 1000;
    int _incomingByte = 0;   // for incoming serial data
    int _incomingChar;   // for incoming serial data
    int _bracketCount = 0;
};
