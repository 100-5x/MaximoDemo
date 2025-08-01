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
#define MQTT_USER "use_token_auth"
#define MQTT_PASSWORD "xdmm-W-5@PII0oA0tr"

const char* root_ca = \
"-----BEGIN CERTIFICATE-----\n"
"MIIFJzCCBA+gAwIBAgIQTPy0rxZJYSoSA6cwKv493DANBgkqhkiG9w0BAQsFADCB\n"
"hjELMAkGA1UEBhMCR0IxDzANBgNVBAcTBkxvbmRvbjEPMA0GA1UECRMGTG9uZG9u\n"
"MS4wLAYDVQQLEyVJQk0gTWF4aW1vIEFwcGxpY2F0aW9uIFN1aXRlIChQdWJsaWMp\n"
"MSUwIwYDVQQDExxwdWJsaWMubWFzLWNsZWFuLm1hcy5pYm0uY29tMB4XDTI1MDcy\n"
"OTIxNTMxNVoXDTI2MDcyOTIxNTMxNVowSzFJMEcGA1UEAxNAaW90Lm1hcy1jbGVh\n"
"bi5hcHBzLjY4ODI1MmUyNGRlOWVjMDA4NWE1ZTc4YS5hbTEudGVjaHpvbmUuaWJt\n"
"LmNvbTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAOzeId4+HAEyI33Y\n"
"X4qV5gBaE3fFp1pmQWg7ZtNM+GhqI6MlWPlViB+O/JHJUPnj38Ywm7m/tJSemIRQ\n"
"i8KVM0cKGkBcnRjHFRbaqF+4tdmbcwZAzdIiN0D6FASAMSNFpk14N+0QnEcacxEt\n"
"QpDADt+ggKchpgGdH2nBU4ME2nxOo3yfzslE9zx8otMcnaL7/Y2bqhvW7pkuNZj3\n"
"d74TB/coK+5elgzDBYOaBiTb33xxKOklTenFlVc+OAROaawjuxleDa7GEJ9ji0xl\n"
"s07oVfeH10+VcecIRp5Pq31NcDLzYXmcBpDHuvpj/tIzGkzjdEJIJhRicxBewCTh\n"
"A3QJX0cCAwEAAaOCAckwggHFMA4GA1UdDwEB/wQEAwIFoDAMBgNVHRMBAf8EAjAA\n"
"MB8GA1UdIwQYMBaAFBziZ87t5fcUYNks8Fe4EMQ5sxPsMIIBggYDVR0RBIIBeTCC\n"
"AXWCQGlvdC5tYXMtY2xlYW4uYXBwcy42ODgyNTJlMjRkZTllYzAwODVhNWU3OGEu\n"
"YW0xLnRlY2h6b25lLmlibS5jb22CQiouaW90Lm1hcy1jbGVhbi5hcHBzLjY4ODI1\n"
"MmUyNGRlOWVjMDA4NWE1ZTc4YS5hbTEudGVjaHpvbmUuaWJtLmNvbYJMKi5tZXNz\n"
"YWdpbmcuaW90Lm1hcy1jbGVhbi5hcHBzLjY4ODI1MmUyNGRlOWVjMDA4NWE1ZTc4\n"
"YS5hbTEudGVjaHpvbmUuaWJtLmNvbYJNKi5lZGdlY29uZmlnLmlvdC5tYXMtY2xl\n"
"YW4uYXBwcy42ODgyNTJlMjRkZTllYzAwODVhNWU3OGEuYW0xLnRlY2h6b25lLmli\n"
"bS5jb22CUCouZWRnZWNvbmZpZ2FwaS5pb3QubWFzLWNsZWFuLmFwcHMuNjg4MjUy\n"
"ZTI0ZGU5ZWMwMDg1YTVlNzhhLmFtMS50ZWNoem9uZS5pYm0uY29tMA0GCSqGSIb3\n"
"DQEBCwUAA4IBAQAzlCGTLX+cDzU9qteZM4Os+5ftaX9X1V4tR7Z0YyqUU+vZn83a\n"
"/O8pZZNKqT8aMOY24xryabC4NFFt5qztUG0s2vFc/aLQuqdfoct/UVaFN+uBOsu+\n"
"JZ9bncv8RVQABe+ctOFw5r3/WhPoEglbydVo7t54zBBcr8uBUgqk8QHfe8mPdD87\n"
"qOmy8Zbhx7On4bVaTyYxcDmJVcGU8G/wGjJp1F9w1BzdFTMfD40W1xPjvLnw0+qa\n"
"rES8L0CZmgQOLF8RKieupX+MxlwTT4Nkq3hHgVazRYR1YKODfX5hprye+hJpv110\n"
"ZA5EnXL6wNKuAv6wZUimpSCkZUmNLslOi+Xx\n"
"-----END CERTIFICATE-----\n";


\;
// The root CA certificate for the MQTT broker in PEM format

// MIIFJzCCBA+gAwIBAgIQTPy0rxZJYSoSA6cwKv493DANBgkqhkiG9w0BAQsFADCB
// hjELMAkGA1UEBhMCR0IxDzANBgNVBAcTBkxvbmRvbjEPMA0GA1UECRMGTG9uZG9u
// MS4wLAYDVQQLEyVJQk0gTWF4aW1vIEFwcGxpY2F0aW9uIFN1aXRlIChQdWJsaWMp
// MSUwIwYDVQQDExxwdWJsaWMubWFzLWNsZWFuLm1hcy5pYm0uY29tMB4XDTI1MDcy
// OTIxNTMxNVoXDTI2MDcyOTIxNTMxNVowSzFJMEcGA1UEAxNAaW90Lm1hcy1jbGVh
// bi5hcHBzLjY4ODI1MmUyNGRlOWVjMDA4NWE1ZTc4YS5hbTEudGVjaHpvbmUuaWJt
// LmNvbTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAOzeId4+HAEyI33Y
// X4qV5gBaE3fFp1pmQWg7ZtNM+GhqI6MlWPlViB+O/JHJUPnj38Ywm7m/tJSemIRQ
// i8KVM0cKGkBcnRjHFRbaqF+4tdmbcwZAzdIiN0D6FASAMSNFpk14N+0QnEcacxEt
// QpDADt+ggKchpgGdH2nBU4ME2nxOo3yfzslE9zx8otMcnaL7/Y2bqhvW7pkuNZj3
// d74TB/coK+5elgzDBYOaBiTb33xxKOklTenFlVc+OAROaawjuxleDa7GEJ9ji0xl
// s07oVfeH10+VcecIRp5Pq31NcDLzYXmcBpDHuvpj/tIzGkzjdEJIJhRicxBewCTh
// A3QJX0cCAwEAAaOCAckwggHFMA4GA1UdDwEB/wQEAwIFoDAMBgNVHRMBAf8EAjAA
// MB8GA1UdIwQYMBaAFBziZ87t5fcUYNks8Fe4EMQ5sxPsMIIBggYDVR0RBIIBeTCC
// AXWCQGlvdC5tYXMtY2xlYW4uYXBwcy42ODgyNTJlMjRkZTllYzAwODVhNWU3OGEu
// YW0xLnRlY2h6b25lLmlibS5jb22CQiouaW90Lm1hcy1jbGVhbi5hcHBzLjY4ODI1
// MmUyNGRlOWVjMDA4NWE1ZTc4YS5hbTEudGVjaHpvbmUuaWJtLmNvbYJMKi5tZXNz
// YWdpbmcuaW90Lm1hcy1jbGVhbi5hcHBzLjY4ODI1MmUyNGRlOWVjMDA4NWE1ZTc4
// YS5hbTEudGVjaHpvbmUuaWJtLmNvbYJNKi5lZGdlY29uZmlnLmlvdC5tYXMtY2xl
// YW4uYXBwcy42ODgyNTJlMjRkZTllYzAwODVhNWU3OGEuYW0xLnRlY2h6b25lLmli
// bS5jb22CUCouZWRnZWNvbmZpZ2FwaS5pb3QubWFzLWNsZWFuLmFwcHMuNjg4MjUy
// ZTI0ZGU5ZWMwMDg1YTVlNzhhLmFtMS50ZWNoem9uZS5pYm0uY29tMA0GCSqGSIb3
// DQEBCwUAA4IBAQAzlCGTLX+cDzU9qteZM4Os+5ftaX9X1V4tR7Z0YyqUU+vZn83a
// /O8pZZNKqT8aMOY24xryabC4NFFt5qztUG0s2vFc/aLQuqdfoct/UVaFN+uBOsu+
// JZ9bncv8RVQABe+ctOFw5r3/WhPoEglbydVo7t54zBBcr8uBUgqk8QHfe8mPdD87
// qOmy8Zbhx7On4bVaTyYxcDmJVcGU8G/wGjJp1F9w1BzdFTMfD40W1xPjvLnw0+qa
// rES8L0CZmgQOLF8RKieupX+MxlwTT4Nkq3hHgVazRYR1YKODfX5hprye+hJpv110
// ZA5EnXL6wNKuAv6wZUimpSCkZUmNLslOi+Xx




// GPIO for Vibration sensor
const int SENSOR_PIN_VIBRATION = 25;
// GPIO for DS18B20 Temperature Sensor
const int SENSOR_PIN_TEMPERATURE = 4;

// MQTT topic for publishing temperature data
const char* mqttTopic = "iot-2/evt/temperature/fmt/json";


WiFiClientSecure wifiClient;
wifiClient.setCACert(root_ca);
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

  // wifiClient.setInsecure(); # this disables verification
  wifiClient.setCACert(root_ca); // use this to verify the server certificate

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
  if (sensorReadVibration()){

    // JSON payload for temperature reading
    float temp = sensorReadTemperature();
    String payload = String("{\"temp\";") + String(temp, 2) + String("}");

    // publish the temperature reading to the MQTT broker
    Serial.print("Publishing temperature: ");
    Serial.println(payload);
    mqttClient.publish(mqttTopic, payload.c_str());
  }

  // Add a delay to avoid flooding the serial monitor
  delay(100);
}
