#include <Servo.h> 

#define PIN_SERVO 5

#define MIN_SERVO 77
#define MAX_SERVO 83

Servo s;
 
void setup() 
{ 
  s.attach(PIN_SERVO);
  Serial.begin(57600);
} 
 
void loop() 
{
  Serial.println("Mooving...");
  s.write(MIN_SERVO);
  delay(5000);

  Serial.println("Writing...");
  s.write(MAX_SERVO);
  delay(5000);
}
