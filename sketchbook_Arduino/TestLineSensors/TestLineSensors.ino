// Code for the QRE1113 Digital board
// From http://bildr.org/2011/06/qre1113-arduino/
// Modified on 18/11/2014 for DraWall

// Outputs and inputs via the serial terminal

#include "pins.h"
#include "hardware.h"

void setup(){
  Serial.begin(PLT_SERIAL_BAUDS);
}

void loop(){
  Serial.println("Looping...");

  if (!readQD()) {
    Serial.println("End of belt is reached!");
    Serial.println("Put something on Serial terminal to continue.");
    while(Serial.read() <= 0);
  }
}

int readQD(){
  // Returns value from the QRE1113 
  // Lower numbers mean more refleacive
  // More than 3000 means nothing was reflected.
  pinMode(PIN_LIMIT_SWITCHES, OUTPUT);
  digitalWrite(PIN_LIMIT_SWITCHES, HIGH);  
  delayMicroseconds(10);
  pinMode(PIN_LIMIT_SWITCHES, INPUT);

  long time = micros();

  // Time how long the input is HIGH, but quit after 3ms as nothing happens after that
  while (digitalRead(PIN_LIMIT_SWITCHES) == HIGH && micros() - time < 3000);

  return (micros() - time) > PLT_SENSOR_LIMIT;
}
