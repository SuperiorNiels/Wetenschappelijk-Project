/*
 *4-Wetenschappelijk project 
 *lijnvolgende robot
 *Eerste test
 *09/03/2017
 */
 
//initialiatie globale variablen

//digitale pinnen
 const byte leftMotors=9;//pwm pinnen
 const byte rightMotors=10;//pwm pinnen
 const byte leftDirection=12;
 const byte rightDirection=13;
 const byte fwdButton=4;
 const byte bwdButton=5;
 const byte lftButton=6;
 const byte rgtButton=7;
 const byte stpButton=8;
 //directies
 const byte FWD = 0;
 const byte BWD = 1;
 //snelheid
 const byte spd = 128;
 
//code die in het begin wordt uitgevoerd
void setup() {
  //OUTPUT PINNEN
  pinMode(leftMotors,OUTPUT);
  pinMode(rightMotors,OUTPUT);
  pinMode(leftDirection,OUTPUT);
  pinMode(rightDirection,OUTPUT);
  //INPUT PINNEN
  pinMode(fwdButton,INPUT_PULLUP);
  pinMode(bwdButton,INPUT_PULLUP);
  pinMode(lftButton,INPUT_PULLUP);
  pinMode(rgtButton,INPUT_PULLUP);
  pinMode(stpButton,INPUT_PULLUP);
  //motors testen
  testMotors();

}
//de loop die constant wordt uitgevoerd
void loop() {
  //initialisatie variablen
  byte F,B,L,R,S;
  //start van de code
  F = digitalRead(fwdButton);
  B = digitalRead(bwdButton);
  L = digitalRead(lftButton);
  R = digitalRead(rgtButton);
  S = digitalRead(stpButton);
  //zorgt voor de besturing van de robot met behulp van knoppen
  //Er wordt gekeken naar een gelijkenis met 0 omdat er pull up resistoren gebruikt werden
  if(S==0) {
    emergencyStop();
  }else if(F==0) {
    forward(spd);
  }else if(B==0) {
    backward(spd);
  }else if(L==0) {
    left(spd);
  }else if(R==0) {
    right(spd);
  }else {
    emergencyStop();
  }
  
}
//functie die de motoren test door de robot links en daarna rechts te doen draaien
void testMotors() {
  left(128);
  delay(1000);//één seconde wachten
  left(0);
  right(128);
  delay(1000);//één seconde wachten
  right(0);
 
}

//functie om de robot rechtdoor te doen bewegen
void forward(byte velocity) {
  digitalWrite(leftDirection,FWD);
  digitalWrite(rightDirection,FWD);
  analogWrite(leftMotors,velocity);
  analogWrite(rightMotors,velocity);
}

//functie om de robot achteruit te doen bewegen
void backward(byte velocity) {
  digitalWrite(leftDirection,BWD);
  digitalWrite(rightDirection,BWD);
  analogWrite(leftMotors,velocity);
  analogWrite(rightMotors,velocity);
}

//functie om de robot links te doen bewegen
void left(byte velocity) {
  digitalWrite(leftDirection,BWD);
  digitalWrite(rightDirection,FWD);
  analogWrite(leftMotors,velocity);
  analogWrite(rightMotors,velocity);
}

//functie om de robot rechts te doen bewegen
void right(byte velocity) {
  digitalWrite(leftDirection,FWD);
  digitalWrite(rightDirection,BWD);
  analogWrite(leftMotors,velocity);
  analogWrite(rightMotors,velocity);
}

//functie om de robot tot stilstand te brengen
void emergencyStop() {
    analogWrite(leftMotors,0);
    analogWrite(rightMotors,0);
}


