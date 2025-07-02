# IBM Maximo Demo Using a Hit-and-Miss Engine

---

## Objective
To put together a demo that is as interactive as possible and without having any slides to go along with it.
This demo should demonstrate how edge devices and sensors can take telemetry data, and feed that data into Maximo to fuel its insights and fleet management decisions.

---

## Intended Workflow
1. Sensor takes its reading (e.g. humidity) from the simple motor.
2. ESP32 processes this data and feeds it to IBM Maximo.
3. Maximo ingests this data to make conclusions about whether it is a safe level of humidity, and, if not, what to do about it.

---

## Timeline
Deliverable by August 6th (Maximo World)

---

## Team
### IBM
| Name            | Responsibility                  |
| --------------- | ------------------------------- |
| Ian Slater      | Project Lead                    |
| Satvika Alur    | Technical Specialist            |
| Sapna Kwatra    | Maximo Integration Specialist   |
| Livia Fingerson | Maximo Integration Assistant    |
| Scott Dial      | Project Sponsor                 |

### CDW
| Name            | Responsibility                  |
| --------------- | ------------------------------- |
| Don Doyle       | PCB & Hardware Integration Lead |


---

## Key Components

### Hit-and-Miss Engine
A late 19th century stationary internal combustion engine that is controlled by a governor to only fire at a set speed.
**Use:** This is where the sensors will be taking their readings from.

### ESP32
Microcontroller with both WiFi and Bluetooth capabilities.
**Use:** Control sensors and send their data to system.

### Sensors
- Humidity
- Temperature
- Infrared
- Vibration
- RPM (Hall Effect Sensor)
**Use:** Take readings from engine.

### Custom Printed Circuit Board
Designed using KiCad, printed and assembled by JLC PCB.
**Use:** Connect the ESP32 to all of the sensors and power source.

### Custom 3D-Printed Box
Custom 3D-Printed Box with a dovetail to attach on the back of the engine.
**Use:** Encase the PCB and make the design compact and clean.

### Maximo Environment
An advanced asset management application suite that provides end users with an intuitive and informative dashboard where they can easily track, inspect, and maintain their critical assets.
**Use:** Ingest the data from the ESP32 and display on the Maximo user dashboard.


---

## C++ Naming Convention Guide

**Project:** Oshkosh Maximo Demo  
**Date:** July 2, 2025  
**Author:** Ian Slater

### 1. General Principles

- Use clear, descriptive names that reflect the purpose of the variable, function, or class.  
- Avoid abbreviations unless they are widely understood (e.g., ESP32, PCB).  
- Use consistent casing and prefixes/suffixes to indicate type or purpose.  
- Prioritize readability and maintainability over brevity.

### 2. Naming Conventions

#### 2.1 Variables

- **Format:** `camelCase`  
- **Prefix:** Use `g_` for global variables, `s_` for static variables.  
- **Examples:**
  ```cpp
  int humidityLevel;
  float sensorReading;
  static bool s_isConnected;
  ```

#### 2.2 Constants

- **Format:** `UPPER_CASE_WITH_UNDERSCORES`  
- **Examples:**
  ```cpp
  const int MAX_HUMIDITY = 80;
  const char* const DEVICE_NAME = "ESP32";
  ```

#### 2.3 Functions

- **Format:** `camelCase`  
- **Verb-noun** structure for clarity.  
- **Examples:**
  ```cpp
  void readSensorData();
  bool isHumiditySafe(float level);
  ```

#### 2.4 Classes and Structs

- **Format:** `PascalCase`  
- **Prefix:** None  
- **Examples:**
  ```cpp
  class SensorManager;
  struct TelemetryPacket;
  ```

#### 2.5 Enums

- **Format:** `PascalCase` for enum type, `UPPER_CASE` for values.  
- **Examples:**
  ```cpp
  enum class SensorType {
      HUMIDITY,
      TEMPERATURE,
      PRESSURE
  };
  ```

#### 2.6 Files

- **Format:** `snake_case`  
- **Extension:** `.cpp` for source files, `.h` for headers.  
- **Examples:**
  ```
  sensor_manager.cpp
  telemetry_utils.h
  ```

### 3. Project-Specific Guidelines

#### 3.1 ESP32 and Sensor Integration

- Prefix ESP32-related functions with `esp32`:
  ```cpp
  void esp32Init();
  void esp32SendData();
  ```

- Prefix sensor-related functions with `sensor`:
  ```cpp
  float sensorReadHumidity();
  void sensorCalibrate();
  ```

#### 3.2 Maximo Integration

- Use `maximo` prefix for functions or classes that handle communication:
  ```cpp
  class MaximoClient;
  void maximoSendTelemetry(const TelemetryPacket& packet);
  ```

### 4. Comments and Documentation

- Use `//` for inline comments and `/** */` for function/class documentation.  
- Document all public functions and classes with purpose, parameters, and return values.

---