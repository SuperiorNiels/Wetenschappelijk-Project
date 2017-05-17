/*
 *4-Wetenschappelijk project 
 *lijnvolgende robot
 *tweede test
 *encoders included
 *09/03/2017
 */
 
//initialiatie globale variablen



 //interrupt pinnen
 const byte interruptA = 0;//dit is pin 2
 const byte interruptB = 1;//dit is pin 3
 //directies
 const byte FWD = 0;
 const byte BWD = 1;
 //snelheid
 const byte spd = 40;
 const byte offset = 10;
 //digital reads
  byte F,B,L,R,S;
 //encoder ticks
 int tickA = 0;
 int tickB = 0;
 const byte leftMotors=10;//pwm pinnen
 const byte rightMotors=9;//pwm pinnen
 const byte leftFrontDirection=12; // Channel 3 motor
 const byte rightFrontDirection=13; //Channel 4 motor
 const byte leftBackDirection=11;// Channel 1 motor
 const byte rightBackDirection=8; // Channel 2 motor
 const byte frtSensor=4;  //front sensor
 const byte cntSensor=5;  //center sensor
 const byte lftSensor=6;  //left sensor
 const byte rgtSensor=7;  //right sensor
 const byte stpButton=A0;  //stop button
//interrupt handlers
  void isrA(){
    tickA++;
    printf("rechterkant %d\n",tickA);
  }

  void isrB(){
    tickB++;
    printf("linkerkant %d\n",tickB);
  }
//code die in het begin wordt uitgevoerd
void setup() {
  //interrupts attachment
   attachInterrupt (interruptA, isrA, CHANGE);   
  attachInterrupt (interruptB, isrB, CHANGE);  
  //OUTPUT PWM PINNEN
  pinMode(leftMotors,OUTPUT);
  pinMode(rightMotors,OUTPUT);
  //OUTPUT pinnen
  pinMode(rightFrontDirection,OUTPUT);
  pinMode(leftBackDirection,OUTPUT);
  pinMode(leftFrontDirection,OUTPUT);
  pinMode(rightBackDirection,OUTPUT);
  Serial.begin(9600);
}
//de loop die constant wordt uitgevoerd
void loop() {
  Serial.print("linkerkant ");
  Serial.println(tickB);
  Serial.print("rechterkant ");
  Serial.println(tickA);
  tickA=0;
  tickB=0;
  while(tickA<300 && tickB<300){
    int verschil = tickA-tickB;
    Serial.println(verschil);
    if(verschil > 50){
      correctRight(spd);
    }
    else{
      forward(60);
    }
  }
  emergencyStop();
}
//functie die de motoren test door de robot links en daarna rechts te doen draaien
void forward(byte velocity) {
  digitalWrite(leftFrontDirection,0);
  digitalWrite(rightFrontDirection,0);
  digitalWrite(leftBackDirection,1);
  digitalWrite(rightBackDirection,1);
  analogWrite(leftMotors,velocity);
  analogWrite(rightMotors,velocity);
}
//functie om de robot tot stilstand te brengen
void emergencyStop() {
    analogWrite(leftMotors,0);
    analogWrite(rightMotors,0);
}
//functie om de robot links te doen bewegen
void left(byte velocity) {
  digitalWrite(leftFrontDirection,1);
  digitalWrite(rightFrontDirection,0);
  digitalWrite(leftBackDirection,0);
  digitalWrite(rightBackDirection,1);
  analogWrite(leftMotors,velocity);
  analogWrite(rightMotors,velocity);
}

//functie om de robot rechts te doen bewegen
void right(byte velocity) {
  digitalWrite(leftFrontDirection,0);
  digitalWrite(rightFrontDirection,1);
  digitalWrite(leftBackDirection,1);
  digitalWrite(rightBackDirection,0);
  analogWrite(leftMotors,velocity);
  analogWrite(rightMotors,velocity);
}

//functie om linkse afwijking op te vangen
void correctLeft(byte velocity) {
  digitalWrite(leftFrontDirection,0);
  digitalWrite(rightFrontDirection,0);
  digitalWrite(leftBackDirection,1);
  digitalWrite(rightBackDirection,1);
  analogWrite(rightMotors,velocity-offset);
  analogWrite(leftMotors,velocity+offset);
}
//functie om rechtse afwijking op te vangen
void correctRight(byte velocity) {
  digitalWrite(leftFrontDirection,0);
  digitalWrite(rightFrontDirection,0);
  digitalWrite(leftBackDirection,1);
  digitalWrite(rightBackDirection,1);
  analogWrite(leftMotors,velocity-offset);
  analogWrite(rightMotors,velocity+offset);
}
