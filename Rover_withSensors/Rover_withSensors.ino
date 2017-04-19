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
 const byte leftFrontDirection=12;
 const byte rightFrontDirection=13;
 const byte leftBackDirection=2;
 const byte rightBackDirection=3;
 const byte frtSensor=4;  //front sensor
 const byte cntSensor=5;  //center sensor
 const byte lftSensor=6;  //left sensor
 const byte rgtSensor=7;  //right sensor
 const byte stpButton=8;  //stop button
 //directies
 const byte FWD = 0;
 const byte BWD = 1;
 //snelheid
 const byte spd = 50;
 const byte offset = 50;
 //digital reads
  byte F,B,L,R,S;
 
//code die in het begin wordt uitgevoerd
void setup() {
  //OUTPUT PWM PINNEN
  pinMode(leftMotors,OUTPUT);
  pinMode(rightMotors,OUTPUT);
  //OUTPUT pinnen
  pinMode(rightFrontDirection,OUTPUT);
  pinMode(leftBackDirection,OUTPUT);
  pinMode(leftFrontDirection,OUTPUT);
  pinMode(rightBackDirection,OUTPUT);
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
  byte S;
  int *sensors[3];
  //start van de code

  S = digitalRead(stpButton);
  //zorgt voor de besturing van de robot met behulp van knoppen
  //Er wordt gekeken naar een gelijkenis met 0 omdat er pull up resistoren gebruikt werden
  if(S==0) {
    emergencyStop();
  }

  
  //start van de code

  //volgende code bepaakt welke actie moet ondernomen worden bij welke sensor stand
  
  *sensors = readSensors();
  //Rechtelijn
  if(sensors[1] && sensors[3]){
    forward(spd);
  }
  //Links
  if(sensors[1] && sensors[2]){
    left(spd);
  }
  //Rechts
  if(sensors[0] && sensors[1]){
    right(spd);
  }
  //Eindpunt
  if(sensors[0] && sensors[2] && sensors[3]){
    emergencyStop();
  }
  //Doodlopend
  if(sensors[3]){
    left(spd);
  }
  //Afwijking Links
  if(sensors[0]){
    correctLeft(spd);
  }
  //Afwijking Rechts
  if(sensors[2]){
    correctRight(spd);
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
  left(spd);
  delay(1000);//één seconde wachten
  left(0);
  right(spd);
  delay(1000);//één seconde wachten
  right(0);
 
}

//functie om de robot rechtdoor te doen bewegen
void forward(byte velocity) {
  digitalWrite(leftFrontDirection,0);
  digitalWrite(rightFrontDirection,0);
  digitalWrite(leftBackDirection,1);
  digitalWrite(rightBackDirection,1);
  analogWrite(leftMotors,velocity);
  analogWrite(rightMotors,velocity);
}

//functie om de robot achteruit te doen bewegen
void backward(byte velocity) {
  digitalWrite(leftFrontDirection,1);
  digitalWrite(rightFrontDirection,1);
  digitalWrite(leftBackDirection,0);
  digitalWrite(rightBackDirection,0);
  analogWrite(leftMotors,velocity);
  analogWrite(rightMotors,velocity);
}

//functie om de robot links te doen bewegen
void left(byte velocity) {
  digitalWrite(leftFrontDirection,0);
  digitalWrite(rightFrontDirection,1);
  digitalWrite(leftBackDirection,1);
  digitalWrite(rightBackDirection,0);
  analogWrite(leftMotors,velocity);
  analogWrite(rightMotors,velocity);
}

//functie om de robot rechts te doen bewegen
void right(byte velocity) {
  digitalWrite(leftFrontDirection,1);
  digitalWrite(rightFrontDirection,0);
  digitalWrite(leftBackDirection,0);
  digitalWrite(rightBackDirection,1);
  analogWrite(leftMotors,velocity);
  analogWrite(rightMotors,velocity);
}

//functie om linkse afwijking op te vangen
void correctLeft(byte velocity) {
  digitalWrite(leftFrontDirection,0);
  digitalWrite(rightFrontDirection,0);
  digitalWrite(leftBackDirection,1);
  digitalWrite(rightBackDirection,1);
  analogWrite(leftMotors,velocity);
  analogWrite(rightMotors,velocity+offset);
}
//functie om rechtse afwijking op te vangen
void correctRight(byte velocity) {
  digitalWrite(leftFrontDirection,0);
  digitalWrite(rightFrontDirection,0);
  digitalWrite(leftBackDirection,1);
  digitalWrite(rightBackDirection,1);
  analogWrite(leftMotors,velocity+offset);
  analogWrite(rightMotors,velocity);
}

//functie om de robot tot stilstand te brengen
void emergencyStop() {
    analogWrite(leftMotors,0);
    analogWrite(rightMotors,0);
}

