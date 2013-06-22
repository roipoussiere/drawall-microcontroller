#include <SD.h>
#include <Servo.h>

#include <drawbot.h>

Draw drawbot(1500, 1500, "drawbot.svg");

void setup() {
  Serial.begin(9600);
  drawbot.commencer();
  drawbot.setVitesse(500);
  delay(2000);
}

void loop() {
  drawbot.svg();
  
  while(true) {}
}
