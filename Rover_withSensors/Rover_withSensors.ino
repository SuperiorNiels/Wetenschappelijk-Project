/*
 *4-Wetenschappelijk project 
 *lijnvolgende robot
 *Eerste test
 *19/04/2017
 * Dieter Balemans v2
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
 const byte spd = 60;
 const byte offset = 30;
 int leave = 0;
 int stopB = 0;
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
  
  //Debugging
  Serial.begin(9600);

}
//de loop die constant wordt uitgevoerd
void loop() {
  int sensors[3];
  //start van de code
  checkStopButton();
  if(stopB==0){
     //start van de code
     //volgende code bepaakt welke actie moet ondernomen worden bij welke sensor stand
     readSensors(sensors);
     
     //Rechtelijn
     if((sensors[0]==0 && sensors[1]==1 && sensors[2]==0 && sensors[3]==1)){
       forward(spd);
       Serial.println("forward");
     }
     //Naar hoek
     else if((sensors[0]==0 && sensors[1]==1 && sensors[2]==0 && sensors[3]==0)){
       forward(spd-20);
       Serial.println("naar hoek");
     }
     
     //Links
     else if(sensors[0]==0 && sensors[1]==1 && sensors[2]==1 && sensors[3]==0){
       do{
        left(spd);
        readSensors(sensors);
        checkStopButton();
        Serial.println("links");
        if(sensors[3]==1 && sensors[1]==1){  //sensors[0]==0 && sensors[1]==1 && sensors[2]==0
           leave = 1;
           Serial.println("LEAVE");
        }
       }while(leave == 0 && stopB == 0);
       leave = 0;
       forward(spd);
       delay(1000);
     }
     
     //Rechts
     else if(sensors[0]==1 && sensors[1]==1 && sensors[2]==0 && sensors[3]==0){   
       do{
        right(spd);
        readSensors(sensors);
         checkStopButton();
        Serial.println("rechts");
        if(sensors[0]==1 && sensors[1]==1  && sensors[3]==1){  //sensors[0]==0 && sensors[1]==1 && sensors[2]==0
           leave = 1;
           Serial.println("LEAVE");
        }
       }while(leave == 0 && stopB == 0);
       leave = 0;
       forward(spd);
     }
     
     //Eindpunt
     else if(sensors[0]==1 && sensors[1]==0 && sensors[2]==1 && sensors[3]==1){
       emergencyStop();
       Serial.println("end");
     }
     
     //Doodlopend
     else if(sensors[0]==0 && sensors[1]==0 && sensors[2]==0 && sensors[3]==0){
       do{
        left(spd);
        readSensors(sensors);
        checkStopButton();
        Serial.println("doodlopend: terugdraaien...");
        if(sensors[3]==1){  //sensors[0]==0 && sensors[1]==1 && sensors[2]==0
           leave = 1;
           Serial.println("LEAVE");
        }
       }while(leave == 0 && stopB == 0);
       leave = 0;
       forward(spd);
     }
     
     //Afwijking Links
     else if((sensors[0]==1 && sensors[1]==1 && sensors[2]==0 && sensors[3]==1) || (sensors[0]==1 && sensors[1]==0 && sensors[2]==0 && sensors[3]==1) || (sensors[0]==0 && sensors[1]==0 && sensors[2]==1 && sensors[3]==0)){
       correctLeft(spd);
       Serial.println("afwijking L");
     }
     
     //Afwijking Rechts
     else if((sensors[0]==0 && sensors[1]==1 && sensors[2]==1 && sensors[3]==1) || (sensors[0]==0 && sensors[1]==0 && sensors[2]==1 && sensors[3]==1) || (sensors[0]==1 && sensors[1]==0 && sensors[2]==0 && sensors[3]==0)){
       correctRight(spd);
       Serial.println("afwijking R");
     }
     
     else{
       forward(spd);
       Serial.println("nothing detected go forward");
     }
  }

  Serial.print(sensors[0]);
  Serial.print(sensors[1]);
  Serial.print(sensors[2]);
  Serial.print(sensors[3]);
  Serial.println();
}

//Check Stopbutton
void checkStopButton(){
  byte S;
  S = digitalRead(stpButton);
  //zorgt voor de besturing van de robot met behulp van knoppen
  //Er wordt gekeken naar een gelijkenis met 0 omdat er pull up resistoren gebruikt werden
  if(S == 0){
    if(stopB == 0) {
        stopB = 1;
    }
    else if(stopB == 1){
        stopB = 0;
    }
  }
}

//functie dat de sensors inleest in een array, de mogelijke waarde zijn 1 of 0; een 0 is wit, een 1 is de zwarte lijn
//de array heeft vaste plaatsen voor de sensoren: 0=right, 1=center, 2=left, 3=front.
void readSensors(int*sensor){
  sensor[3] = digitalRead(frtSensor);
  sensor[2] = digitalRead(lftSensor);
  sensor[1] = digitalRead(cntSensor);
  sensor[0] = digitalRead(rgtSensor);
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
