/*
   precompiler declarations.
*/

#define LED 4

// GPIO Pin 4 is used.  1k resistor from pin to LED.  LED returns to gnd.

void setup() {
  pinMode(LED,OUTPUT);
}

void loop() {
  // Turn the light on
  delay(750);
  digitalWrite(LED,HIGH);
  // Turn light off
  delay(750);
  digitalWrite(LED,LOW);
}
