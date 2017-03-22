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
 const byte frtSensor=4;  //front sensor
 const byte cntSensor=5;  //center sensor
 const byte lftSensor=6;  //left sensor
 const byte rgtSensor=7;  //right sensor
 const byte stpButton=8;  //stop button
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
  pinMode(frtSensor,INPUT);
  pinMode(cntSensor,INPUT);
  pinMode(lftSensor,INPUT);
  pinMode(rgtSensor,INPUT);
  pinMode(stpButton,INPUT_PULLUP);
  //motors testen
  testMotors();

}
//de loop die constant wordt uitgevoerd
void loop() {
  //initialisatie variablen
  byte S;
  int *sensors[3];
  //start van de code

  //volgende code bepaakt welke actie moet ondernomen worden bij welke sensor stand
  
  *sensors = readSensors();
  //Rechtelijn
  if(sensors[1] && sensors[3]){
    forward(128);
  }
  //Links
  if(sensors[1] && sensors[2]){
    left(128);
  }
  //Rechts
  if(sensors[0] && sensors[1]){
    right(128);
  }
  //Eindpunt
  if(sensors[0] && sensors[2] && sensors[3]){
    emergencyStop();
  }
  //Doodlopend
  if(sensors[3]){
    left(255);
  }
  //Afwijking Links
  if(sensors[0]){
    correctLeft(128);
  }
  //Afwijking Rechts
  if(sensors[2]){
    correctRight(128);
  }

  
  //knopbesturing: zorgt voor de besturing van de robot met behulp van knoppen
  S = digitalRead(stpButton);
  //Er wordt gekeken naar een gelijkenis met 0 omdat er pull up resistoren gebruikt werden
  if(S==0) {
    emergencyStop();
  }
  
}

//functie dat de sensors inleest in een array, de mogelijke waarde zijn 1 of 0; een 1 is wit, een 0 is de zwarte lijn
//de array heeft vaste plaatsen voor de sensoren: 3=front, 2=left, 1=center, 0=right
int * readSensors(){
  static int sensor[3];
  sensor[0] = digitalRead(rgtSensor);
  int temp = digitalRead(cntSensor);
  if(temp==HIGH){
    sensor[1] = 0;
  }
  else{
    sensor[1] = 1;
  }
  sensor[2] = digitalRead(lftSensor);
  sensor[3] = digitalRead(frtSensor);
  return sensor;
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
//functie om linkse afwijking op te vangen
void correctLeft(byte velocity) {
  digitalWrite(leftDirection,FWD);
  digitalWrite(rightDirection,FWD);
  analogWrite(leftMotors,velocity-50);
  analogWrite(rightMotors,velocity);
}
//functie om rechtse afwijking op te vangen
void correctRight(byte velocity) {
  digitalWrite(leftDirection,FWD);
  digitalWrite(rightDirection,FWD);
  analogWrite(leftMotors,velocity);
  analogWrite(rightMotors,velocity-50);
}
//functie om de robot tot stilstand te brengen
void emergencyStop() {
    analogWrite(leftMotors,0);
    analogWrite(rightMotors,0);
}


