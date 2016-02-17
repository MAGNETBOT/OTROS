// Código de prueba del buzzer del magnetBot.
// Hecho por Roboteam, febrero 14, 2016. 

#define buzzPin 13


void setup() {
  
  pinMode(buzzPin, OUTPUT); // Aparentemente no es 
}

void loop() {
   tone(buzzPin, 261, 150);
   delay(1000);
}
