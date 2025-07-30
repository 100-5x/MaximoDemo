#include <Arduino.h>
// for wifi
#include <WiFi.h>
#include <WiFiClientSecure.h>
// for mqtt
#include <PubSubClient.h>
// for temperature
#include <OneWire.h>
#include <DallasTemperature.h>

#define WIFI_SSID "Ian" // name of wifi network to connect to
#define WIFI_PASSWORD "Ian's super secret password" // wifi network password

#define MQTT_SERVER "oshkoshid.messaging.iot.mas-clean.apps.688252e24de9ec0085a5e78a.am1.techzone.ibm.com"
#define MQTT_PORT 8883 // use 8883 for TLS
#define MQTT_USER "esp32-maximo"
#define MQTT_PASSWORD "gQ+u9er&RSJstMmz2z"

// GPIO for Vibration sensor
const int SENSOR_PIN_VIBRATION = 25;
// GPIO for DS18B20 Temperature Sensor
const int SENSOR_PIN_TEMPERATURE = 4;

WiFiClientSecure wifiClient;
PubSubClient mqttClient(wifiClient);

// Setup a oneWire instance to communicate with any OneWire devices (temperature)
OneWire oneWire(SENSOR_PIN_TEMPERATURE);
// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

void connectToWifi() {
  /**
   * @brief   Connects to WiFi network using WiFiMulti
   * @return  bool: true if connected to WiFi, false otherwise
   */
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi Connected");
}

void connectToMQTT() {
  /**
   * @brief   Connects to MQTT broker
   * @details This function will block until a connection is established
   */
  while(!mqttClient.connected()) {
    Serial.println("Attempting MQTT Connection...");
    if (mqttClient.connect("ESP32Client", MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.print("MQTT Failed, rc=");
      Serial.println(mqttClient.state());
      delay(5000);
    }
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

float sensorReadTemperature() {
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

  return temperatureC;
}

void mqttPublishTemperature(float tempC) {
  if (mqttClient.publish("esp32/temperature", String(tempC).c_str())) {
    Serial.println("Temperature published successfully");
  } else {
    Serial.println("Failed to publish temperature");
  }
}

// For testing
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

  wifiClient.setInsecure();

  Serial.setDebugOutput(true);                // Enable ESP32 WiFi debug logs

  // Blocking: code will not proceed until WiFi is connected to
  connectToWifi();

  // Initialize MQTT
  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);

  // Start the DS18B20 sensor
  sensors.begin();
}

void loop() {
  // maintain MQTT connection
  if (!mqttClient.connected()) {
    connectToMQTT();
  }
  mqttClient.loop();

  // Current functionality: takes temperature every time it detects a vibration
  if (sensorReadVibration()) mqttPublishTemperature(sensorReadTemperature());

  // Add a delay to avoid flooding the serial monitor
  delay(100);
}
