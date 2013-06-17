/*
This work is licensed under the Creative Commons Attribution - Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions 3.0 France License. To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/3.0/fr/.
*/
    
#include <SD.h>
#include <Servo.h>

#include <Drawbot.h>

Draw drawbot(1000, 750, "logo1.svg");

void setup() {
  Serial.begin(57600);
  drawbot.commencer();
  drawbot.setVitesse(20);
  delay(2000);
}

void loop() {
  drawbot.svg();

  drawbot.centrer();
  drawbot.alimenter(false);
  
  while(true) {}
}
