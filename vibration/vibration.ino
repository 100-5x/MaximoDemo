/*
  Read the digital output value of the vibration sensor.
  When a vibration is detected, the program will display the message 
  "Detected vibration..." on the serial monitor. Conversely, 
  if there is no vibration, the program will output "..." instead.

  Board: ESP32 Development Board
  Component: Vibration Sensor(SW-420)
*/

// Define the pin numbers for Vibration Sensor
const int sensorPin = 25;

void setup() {
  Serial.begin(9600);         // Start serial communication at 9600 baud rate
  pinMode(sensorPin, INPUT);  // Set the sensorPin as an input pin
}

void loop() {
  if (digitalRead(sensorPin)) {               // Check if there is any vibration detected by the sensor
    Serial.println("Detected vibration...");  // Print "Detected vibration..." if vibration detected
  } 
  else {
    Serial.println("Nope");  // Print "..." other wise
  }

  // Add a delay to avoid flooding the serial monitor
  delay(100);
}