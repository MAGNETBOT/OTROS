// Código de prueba del LED del magnetBot.
// Hecho por Roboteam, febrero 14, 2016.m 

#define ledPin 23


void setup() {
  
  pinMode(ledPin, OUTPUT);
}

void loop() {
  digitalWrite(ledPin, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait for a second
  digitalWrite(ledPin, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);              // wait for a second
}
