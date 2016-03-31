#include <Util.h>
#include <Coroutines.h>

void testEspera(COROUTINE_CONTEXT(coroutine)){
  BEGIN_COROUTINE;
    coroutine.wait(2000);
    COROUTINE_YIELD;
  END_COROUTINE;
}

bool espera;

void moveServo(bool down){
  int pos = 0;
  if (down){
    //pos = inicio;
    for (pos = 6; pos <= 75; pos += 5) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      myEservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(40);                       // waits 15ms for the servo to reach the position
    }
  } else {
    //pos = fin;
    for (pos = 75; pos >= 6; pos -= 5) { // goes from 180 degrees to 0 degrees
      myEservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(40);                       // waits 15ms for the servo to reach the position
    }
  }
}

void grab(){
  moveServo(false);
  delay(1000);
  digitalWrite(electroPin,HIGH);
  delay(1500);
  moveServo(true);
  delay(1500);
  digitalWrite(electroPin,LOW);  
}

