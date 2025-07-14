// for wifi
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
// for temperature
#include <OneWire.h>
#include <DallasTemperature.h>

#define WIFI_SSID "Ian" // name of wifi network to connect to
#define WIFI_PASSWORD "somethingeasier1" // wifi network password

WiFiMulti wifiMulti; // creating instance

// GPIO for Vibration sensor
const int SENSOR_PIN_VIBRATION = 25;

// GPIO for DS18B20 Temperature Sensor
const int SENSOR_PIN_TEMPERATURE = 4;     
// Setup a oneWire instance to communicate with any OneWire devices (temperature)
OneWire oneWire(SENSOR_PIN_TEMPERATURE);
// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

bool connectToWifi() {
  Serial.println("trying to connect...");
  if (wifiMulti.run() == WL_CONNECTED) {
    Serial.println("Connected");
    return true;
  } else {
    return false;
  }
}

bool sensorReadVibration() {
  /**
   * @brief   Reads vibration detection reading from SW-420
   * @return  bool: true if vibration is detected
   */
  if (digitalRead(SENSOR_PIN_VIBRATION)) {    // Check if there is any vibration detected by the sensor
    Serial.println("Detected vibration.");
    return true;
  } else {
    Serial.println("No vibration detected...");
    return false;
  }
}

void sensorReadTemperature() {
  /**
   * @brief   Reads temperature from DS18B20
   */
  Serial.println("TEMPERATURE");
  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  float temperatureF = sensors.getTempFByIndex(0);
  Serial.print(temperatureC);
  Serial.println("ºC");
  Serial.print(temperatureF);
  Serial.println("ºF");
  Serial.println();
}

void scanNetworks() {
  /**
   * @brief   Scans for available WiFi networks
   */
  int n = WiFi.scanNetworks();
  Serial.println("Found networks:");
  for (int i = 0; i < n; i++) {
    Serial.print(WiFi.SSID(i));
    Serial.print(" (Channel ");
    Serial.print(WiFi.channel(i));
    Serial.println(")");
  }
}

void setup() {
  Serial.begin(921600);                       // Start serial communication at 9600 baud rate
  pinMode(SENSOR_PIN_VIBRATION, INPUT);       // Set the vibrationSensorPin as an input pin
  WiFi.setSleep(false);                       // disable wifi sleep mode
  wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);  // access point
  Serial.setDebugOutput(true);                // Enable ESP32 WiFi debug logs


  // Blocking: code will not proceed until WiFi is connected to
  while (!connectToWifi()) {
    scanNetworks();
    delay(100);
  }

  // Start the DS18B20 sensor
  sensors.begin();
}

void loop() {
  // Current functionality: takes temperature every time it detects a vibration
  if (sensorReadVibration()) sensorReadTemperature();

  // Add a delay to avoid flooding the serial monitor
  delay(100);
}
