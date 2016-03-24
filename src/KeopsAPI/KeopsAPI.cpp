
String KeopsAPI::getAllPyramidsRegistered(){
  HTTPClient http;
  String payload = "";
  Serial.print("[HTTP] begin...\n");
  http.begin("https://keops.herokuapp.com/pyramids", "F5 69 8C CA 29 68 5E 47 26 38 C5 1A 18 F1 8A 6A EA 60 56 4D"); //HTTPS sha fingerprint
  Serial.print("[HTTP] GET...\n");
  int httpCode = http.GET();
  // httpCode will be negative on error
  if(httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
      if(httpCode == HTTP_CODE_OK) {
          payload = http.getString();
          Serial.println(payload);
          return payload;
      }
  } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();
  return payload;
}
