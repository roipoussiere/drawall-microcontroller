#include <SD.h>
#include <Servo.h>

#include <drawbot.h>

Draw drawbot(1500, 1000, 1000, 250, 500);

void setup() {
  Serial.begin(9600);
  drawbot.commencer();
  drawbot.setVitesse(500);
  delay(2000);
}

void loop() {
  drawbot.svg("drawbot.svg");
  
  while(true) {}
}
