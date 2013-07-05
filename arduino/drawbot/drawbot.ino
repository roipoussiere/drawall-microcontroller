/*
This work is licensed under the Creative Commons Attribution
- Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions
3.0 France License. To view a copy of this license, visit
http://creativecommons.org/licenses/by-nc-sa/3.0/fr/.
*/

#include <SD.h>
#include <Servo.h>

#include <drawbot.h>

Drawbot d(1500, 1000, 1000, 250, 250);

void setup() {
  d.begin();
  d.setSpeed(500);
  delay(2000);
}

void loop() {
  d.svg("drawbot.svg");
  d.end();
}
