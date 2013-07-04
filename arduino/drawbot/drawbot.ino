#include <SD.h>
#include <Servo.h>

#include <drawbot.h>

Drawbot d(1500, 1000, 1000, 250, 250);

void setup() {
  Serial.begin(9600);
  d.begin();
  d.setSpeed(500);
  delay(2000);
}

void loop() {
  d.svg("drawbot.svg");
  
  while(true) {}
}
