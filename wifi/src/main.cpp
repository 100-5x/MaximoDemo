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

#define MQTT_SERVER "d56f5dde05f8433485c6294bdda269b9.s1.eu.hivemq.cloud"
#define MQTT_PORT 8883 // use 8883 for TLS
#define MQTT_USER "esp32"
#define MQTT_PASSWORD "MaximoDemo2025"

const char* root_ca = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIDrzCCApegAwIBAgIQCDvgVpBCRrGhdWrJWZHHSjANBgkqhkiG9w0BAQUFADBh\n" \
"MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n" \
"d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD\n" \
"QTAeFw0wNjExMTAwMDAwMDBaFw0zMTExMTAwMDAwMDBaMGExCzAJBgNVBAYTAlVT\n" \
"MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\n" \
"b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IENBMIIBIjANBgkqhkiG\n" \
"9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4jvhEXLeqKTTo1eqUKKPC3eQyaKl7hLOllsB\n" \
"CSDMAZOnTjC3U/dDxGkAV53ijSLdhwZAAIEJzs4bg7/fzTtxRuLWZscFs3YnFo97\n" \
"nh6Vfe63SKMI2tavegw5BmV/Sl0fvBf4q77uKNd0f3p4mVmFaG5cIzJLv07A6Fpt\n" \
"43C/dxC//AH2hdmoRBBYMql1GNXRor5H4idq9Joz+EkIYIvUX7Q6hL+hqkpMfT7P\n" \
"T19sdl6gSzeRntwi5m3OFBqOasv+zbMUZBfHWymeMr/y7vrTC0LUq7dBMtoM1O/4\n" \
"gdW7jVg/tRvoSSiicNoxBN33shbyTApOB6jtSj1etX+jkMOvJwIDAQABo2MwYTAO\n" \
"BgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUA95QNVbR\n" \
"TLtm8KPiGxvDl7I90VUwHwYDVR0jBBgwFoAUA95QNVbRTLtm8KPiGxvDl7I90VUw\n" \
"DQYJKoZIhvcNAQEFBQADggEBAMucN6pIExIK+t1EnE9SsPTfrgT1eXkIoyQY/Esr\n" \
"hMAtudXH/vTBH1jLuG2cenTnmCmrEbXjcKChzUyImZOMkXDiqw8cvpOp/2PV5Adg\n" \
"06O/nVsJ8dWO41P0jmP6P6fbtGbfYmbW0W5BjfIttep3Sp+dWOIrWcBAI+0tKIJF\n" \
"PnlUkiaY4IBIqDfv8NZ5YBberOgOzW6sRBc4L0na4UU+Krk2U886UAb3LujEV0ls\n" \
"YSEY1QSteDwsOoBrp+uvFRTp2InBuThs4pFsiv9kuXclVzDAGySj4dzp30d8tbQk\n" \
"CAUw7C29C79Fv1C5qfPrmAESrciIxpg0X40KPMbp1ZWVbd4=\n" \
"-----END CERTIFICATE-----\n";

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
