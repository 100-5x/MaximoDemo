#include <Arduino.h>
#include <WiFiMulti.h> 

#define WIFI_SSID "Ian" // name of wifi network to connect to
#define WIFI_PASSWORD "somethingeasier1" // wifi network password

WiFiMulti wifiMulti; // creating instance

void setup() {
  Serial.begin(921600);
  pinMode(LED_BUILTIN, OUTPUT); // switches on when wifi is connecteed


  // blocking part. no code executed until board connects to wifi
  while (wifiMulti.run() != WL_CONNECTED) {
    Serial.println("trying to connect...");
    wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD); // access point. 
    // can add multiple access points - starts from first until a connection is made
 
    delay(100); // keeps executing until connected
  }
  Serial.println("Connected");
}

void loop() {
  digitalWrite(LED_BUILTIN, WiFi.status() == WL_CONNECTED); // continuously checks if wifi is connected; displays using LED
}

