#include <Servo.h> 
 
Servo servo;

#include "pins.h"
#include "hardware.h"

int drawingInsertConf = 20;
int movingInsertConf = 80;

void setup() 
{ 
  drawingInsertConf = drawingInsertConf/float(100)*PLT_MAX_SERVO_ANGLE+PLT_MIN_SERVO_ANGLE;
  movingInsertConf = movingInsertConf/float(100)*PLT_MAX_SERVO_ANGLE+PLT_MIN_SERVO_ANGLE;
  Serial.begin(57600);
  Serial.println(drawingInsertConf);
  Serial.println(movingInsertConf);
  servo.attach(PIN_SERVO);
} 

void loop() 
{
	delay(PLT_PRE_SERVO_DELAY);
	for(int pos = movingInsertConf; pos>=drawingInsertConf; pos-=1)
	{
		servo.write(pos);
                Serial.println(pos);
		delay(15);
	}
	delay(PLT_POST_SERVO_DELAY);

	delay(PLT_PRE_SERVO_DELAY);
	for(int pos = drawingInsertConf; pos < movingInsertConf; pos += 1)
	{
		servo.write(pos);
                Serial.println(pos);
		delay(15);
	} 
	delay(PLT_POST_SERVO_DELAY);
}
