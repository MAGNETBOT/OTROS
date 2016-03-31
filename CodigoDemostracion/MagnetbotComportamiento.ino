#include <l293Full.h>
#include <sharpIR.h>
#include <srf04.h>
#include <Servo.h>
#include <Util.h>
#include <Coroutines.h>

#define LOGIC1     27
#define LOGIC2     12
#define LOGIC3     29
#define LOGIC4     13
#define ENABLE1    33
#define ENABLE2    31
#define DC         myMotor.duty_cycle
#define sharpPin   15
#define ECHOPIN    45                            
#define TRIGGERPIN 43
#define servoPin   11
#define servoEPin  10
#define buzzPin    2
#define ledPin     23
#define electroPin     22

l293Full myMotor(LOGIC1,LOGIC2,LOGIC3,LOGIC4,ENABLE1,ENABLE2);
srf04 mySrf04(TRIGGERPIN,ECHOPIN);
sharpIR mySharp(sharpPin);
double dSharp=0.0;
Servo myservo;
Servo myEservo;

//int pos = 0;
double dSrf04=0.0;
bool servoSides[3];
bool sonidoSides[3];
int ultimoGiro; // 0: derecha, 1: izquierda, 2: frente.

Coroutines<10> coroutines;
bool brazo;

void testBrazo(COROUTINE_CONTEXT(coroutine)){
  BEGIN_COROUTINE;
    coroutine.wait(2000);
    COROUTINE_YIELD;
    
    moveServo(false);
    coroutine.wait(1000);
    COROUTINE_YIELD;
    
    digitalWrite(electroPin,HIGH);
    digitalWrite(ledPin, HIGH);
    coroutine.wait(1500);
    COROUTINE_YIELD;

    moveServo(true);
    coroutine.wait(500);
    COROUTINE_YIELD;
    
    digitalWrite(electroPin,LOW);
    digitalWrite(ledPin, LOW);
    brazo = true;
    //grab();
    COROUTINE_YIELD;
  END_COROUTINE;
}

void setup() {
  // put your setup code here, to run once:
  myMotor.dutyMax1 = 142;
  myMotor.dutyMax2 = 150;
  myservo.attach(servoPin);
  myEservo.attach(servoEPin);
  myEservo.write(75);
  myservo.write(92);
  myMotor.motorsUp();
  dSharp=mySharp.measure();
  dSrf04=mySrf04.measure();
  pinMode(ledPin, OUTPUT);
  pinMode(electroPin, OUTPUT);
  Serial.begin(9600);
  servoSides[0] = true;   // derecha
  servoSides[1] = true;   // izquierda
  servoSides[2] = true;   // Frente
  sonidoSides[0] = true;   // derecha
  sonidoSides[1] = true;   // izquierda
  sonidoSides[2] = true;   // Frente
  ultimoGiro = 2; // Frente
  
  brazo = false;
  coroutines.start(testBrazo);
}

void loop() {
  coroutines.update();
  // put your main code here, to run repeatedly:
  dSrf04 = mySrf04.measure();
  dSharp=mySharp.measure();
  Serial.print("SHARP: ");
  Serial.print(dSharp);
  Serial.print(" ");
  Serial.print("Ultrasonido: ");
  Serial.print(dSrf04);
  Serial.println(" ");
  
  if(dSrf04 <= 6.5 && dSharp >= 18.0){
    servoSides[2] = true;
    sonidoSides[2] = true;
    myMotor.forward(DC);
    //digitalWrite(ledPin, LOW);
  } else {

    // Verificando el frente del robot, sharp como ultrasonido
    if (dSrf04 > 6.5){
      sonidoSides[2] = false;
    }
    if (dSharp < 18.0){
      servoSides[2] = false;  
    }
    
    //digitalWrite(ledPin, HIGH);
    myMotor.brake(DC);
    myservo.write(10);
    delay(1000);
    if (mySharp.measure() >= 18.0){
      servoSides[0] = true;
    }
    else{
      servoSides[0] = false;
    }
    myservo.write(92);
    delay(1000);
    myservo.write(170);
    delay(1000);
    if (mySharp.measure() >= 18.0){
      servoSides[1] = true;
    }else{
      servoSides[1] = false;  
    }
    
    myservo.write(92);
    delay(500);
    if (servoSides[0] && servoSides[1] && ultimoGiro == 0 && sonidoSides[0] && sonidoSides[1]){
      if (!sonidoSides[2]){
        myMotor.reverse(DC);
        delay(750);
        myMotor.brake(DC);
      }

      myMotor.spinLeft(DC);
      servoSides[0] = servoSides[2];
      servoSides[2] = servoSides[1];
      servoSides[1] = true;

      //tone(buzzPin,250,1000);
      sonidoSides[0] = sonidoSides[2];
      sonidoSides[2] = sonidoSides[1];
      sonidoSides[1] = true;
      ultimoGiro = 2;
      delay(1100);
    }else if (servoSides[0] && sonidoSides[0]){
      if (!sonidoSides[2]){
        myMotor.reverse(DC);
        delay(750);
        myMotor.brake(DC);
      }
      
      myMotor.spinRight(DC);
      servoSides[1] = servoSides[2];
      servoSides[2] = servoSides[0];
      servoSides[0] = true;

      //tone(buzzPin,250,1000);
      sonidoSides[1] = sonidoSides[2];
      sonidoSides[2] = sonidoSides[0];
      sonidoSides[0] = true;
      ultimoGiro = 0;
      delay(1100);
    } else if (servoSides[1] && sonidoSides[1]){
      if (!sonidoSides[2]){
        myMotor.reverse(DC);
        delay(750);
        myMotor.brake(DC);
      }
      
      myMotor.spinLeft(DC);
      servoSides[0] = servoSides[2];
       servoSides[2] = servoSides[1];
      servoSides[1] = true;

      sonidoSides[0] = sonidoSides[2];
      sonidoSides[2] = sonidoSides[1];
      sonidoSides[1] = true;
      ultimoGiro = 1;
      delay(1100);
    } else {
      if (!sonidoSides[2]){
        myMotor.reverse(DC);
        delay(750);
        myMotor.brake(DC);
      }
      
      myMotor.spinLeft(DC);
      servoSides[2] = true;
      servoSides[1] = false;
      servoSides[0] = false;

      sonidoSides[0] = sonidoSides[1];
      sonidoSides[2] = true;
      sonidoSides[1] = sonidoSides[0];
      ultimoGiro=4;
      delay(2200);
    }
  }

  // Levantar el brazo y depositar tornillos.
  if (brazo){
    //myMotor.brake(DC);
    brazo= false;
    //grab();
    //delay(1000);
    coroutines.start(testBrazo);
  }
}
