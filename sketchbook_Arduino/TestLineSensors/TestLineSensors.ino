// Code for the QRE1113 Digital board
// From http://bildr.org/2011/06/qre1113-arduino/
// Modified on 18/11/2014 for DraWall

// Outputs and inputs via the serial terminal

#define LIMIT 1500

int sensors_pin = 3; // connected to digital 3

void setup(){
  Serial.begin(57600);
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
  pinMode(sensors_pin, OUTPUT);
  digitalWrite(sensors_pin, HIGH);  
  delayMicroseconds(10);
  pinMode(sensors_pin, INPUT);

  long time = micros();

  // Time how long the input is HIGH, but quit after 3ms as nothing happens after that
  while (digitalRead(sensors_pin) == HIGH && micros() - time < 3000);

  return (micros() - time) > LIMIT;
}
