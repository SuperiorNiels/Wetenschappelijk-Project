
/*
 *4-Wetenschappelijk project 
 *lijnvolgende robot
 *Eerste test
 *19/04/2017
 * Dieter Balemans v2
 */
 
//initialiatie globale variablen
//interrupt pinnen
 const byte interruptA = 0; //dit is pin 2
 const byte interruptB = 1; //dit is pin 3
//digitale pinnen
 const byte leftMotors=10; //pwm pinnen
 const byte rightMotors=9; //pwm pinnen
 const byte leftFrontDirection=12; // Channel 3 motor
 const byte rightFrontDirection=13; //Channel 4 motor
 const byte leftBackDirection=11;// Channel 1 motor
 const byte rightBackDirection=8; // Channel 2 motor
 const byte frtSensor=4;  //front sensor
 const byte cntSensor=5;  //center sensor
 const byte lftSensor=6;  //left sensor
 const byte rgtSensor=7;  //right sensor
 const byte stpButton=A0;  //stop button
//snelheid
 const byte spd = 60;
 const byte offset = 35;
 int leave = 0;
 int stopB = 0;
//digital reads
 byte S;
//encoder ticks
 int tickA = 0;
 int tickB = 0;

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
  //interrupts attachment
  attachInterrupt (interruptA, isrA, CHANGE);   
  attachInterrupt (interruptB, isrB, CHANGE);  
  
  //Debugging
  Serial.begin(9600);

}
//de loop die constant wordt uitgevoerd
void loop() {
  int sensors[3];
  //start van de code
  checkStart();
  readSensors(sensors);
  if(stopB==1){
     //start van de code
     //volgende code bepaakt welke actie moet ondernomen worden bij welke sensor stand
     Serial.print("Direction: ");
	 readSensors(sensors);
     //Rechtelijn
     if((sensors[0]==0 && sensors[1]==1 && sensors[2]==0 && sensors[3]==1)){
         forward(spd);      
         Serial.println("forward");
     }   
	  
      //Links
      else if(sensors[0]==0 && sensors[1]==0 && sensors[2]==0 && sensors[3]==1){
             Serial.println("Links");
			 leftTurn();
       }
               
      //Rechts
      else if(sensors[0]==1 && sensors[1]==0 && sensors[2]==1 && sensors[3]==0){
             Serial.println("Rechts");
			 tickA = 0;
             tickB = 0;
			 while(tickA<220 && tickB<220){
                 forward(spd);
                 Serial.println(tickA);
                 Serial.println(tickB);
             }
			rightTurn(); 
       }
	   
	   //T-splitsing R
	   else if(sensors[0]==1 && sensors[1]==0 && sensors[2]==0 && sensors[3]==1){
		   Serial.println("T-splitsing R");    //Het algoritme dat we hier gebruiken is wallfolower dit doet de robot rechts draaien telkens wanneer hij kan.
		   rightTurn();
	   }
	   
	   //T-splitsing L
	   else if(sensors[0]==0 && sensors[1]==0 && sensors[2]==1 && sensors[3]==1){
		   Serial.println("T-splitsing L");
		   forward(spd);
	   }
	   
	   //T-splitsing C
	   else if(sensors[0]==1 && sensors[1]==0 && sensors[2]==1 && sensors[3]==1){
		   Serial.println("T-splitsing C");		//Het algoritme dat we hier gebruiken is wallfolower dit doet de robot rechts draaien telkens wanneer hij kan.
		   rightTurn();
	   }
	   
	   //Kruispunt
	   else if(sensors[0]==1 && sensors[1]==1 && sensors[2]==1 && sensors[3]==1){
		   Serial.println("Kruispunt");
		   tickA = 0;
           tickB = 0;
		   while(tickA<50 && tickB<50){
               forward(spd);
               Serial.println(tickA);
               Serial.println(tickB);
           }
		   rightTurn(); 		//Het algoritme dat we hier gebruiken is wallfolower dit doet de robot rechts draaien telkens wanneer hij kan.
	   }						//Dus op een kruispunt draait hij rechts.
  
       //Afwijking Links
       else if(sensors[0]==1 && sensors[1]==0 && sensors[2]==0 && sensors[3]==0 || sensors[0]==1 && sensors[1]==1 && sensors[2]==0 && sensors[3]==1 || sensors[0]==1 && sensors[1]==1 && sensors[2]==0 && sensors[3]==0){
           if(tickA < 1000 || tickB < 1000){
			   correctLeftShort(spd);
		   }
		   else{
			   correctLeftLong(spd);
		   }
           Serial.println("afwijking L");
       }
         
       //Afwijking Rechts
       else if(sensors[0]==0 && sensors[1]==0 && sensors[2]==1 && sensors[3]==0 || sensors[0]==0 && sensors[1]==1 && sensors[2]==1 && sensors[3]==1 || sensors[0]==0 && sensors[1]==1 && sensors[2]==1 && sensors[3]==0){
           if(tickA < 1000 || tickB < 1000){
			   correctRightShort(spd);
		   }
		   else{
			   correctRightLong(spd);
		   }
           Serial.println("afwijking R");    
       } 
	   
	   //Doodlopend
       else if(sensors[0]==0 && sensors[1]==0 && sensors[2]==0 && sensors[3]==0){
             tickA = 0;
             tickB = 0;
             while(tickA<1060 && tickB<1060){ // eventueel nog met front sensor aan implementeren 
                 right(spd);
                 Serial.println(tickA);
                 Serial.println(tickB);
             }
             tickA = 0;
             tickB = 0;
             while(tickA<50&& tickB<50){
                 forward(spd);
                 Serial.println(tickA);
                 Serial.println(tickB);
             }
             tickA = 0;
             tickB = 0;
             leave = 0;
      } 
        
       //Eindpunt
       else if(sensors[0]==1 && sensors[1]==1 && sensors[2]==1 && sensors[3]==0){
         emergencyStop();
         Serial.println("end");
       }
   
    else{
       forward(spd);
       Serial.println("nothing detected go forward");
     }
  }
  else{
    emergencyStop();
    Serial.println("stopped");
  }

  Serial.print(sensors[0]);
  Serial.print(sensors[1]);
  Serial.print(sensors[2]);
  Serial.print(sensors[3]);
  Serial.println();
}

//Turn left
void leftTurn(){
	tickA = 0;
    tickB = 0;
    while(tickA<530 && tickB<530){
        left(spd);
        Serial.println(tickA);
        Serial.println(tickB);
    }
    tickA = 0;
    tickB = 0;
    while(tickA<300 && tickB<300){
        forward(spd);
        Serial.println(tickA);
        Serial.println(tickB);
    }
    tickA = 0;
    tickB = 0;
    leave = 0;
}

//Turn right
void rightTurn(){
	tickA = 0;
    tickB = 0;
    while(tickA<530 && tickB<530){
        right(spd);
        Serial.println(tickA);
        Serial.println(tickB);
    }
    tickA = 0;
    tickB = 0;
    while(tickA<100 && tickB<100){
        forward(spd);
        Serial.println(tickA);
        Serial.println(tickB);
    }
    tickA = 0;
    tickB = 0;
    leave = 0;
}

//Check Stopbutton
void checkStart(){
  byte S;
  S = analogRead(stpButton);
  //zorgt voor de besturing van de robot met behulp van knoppen
  //Er wordt gekeken naar een gelijkenis met 0 omdat er pull up resistoren gebruikt werden
  if(S < 50){
        stopB = 1;
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
void correctLeftShort(byte velocity) {
  digitalWrite(leftFrontDirection,0);
  digitalWrite(rightFrontDirection,0);
  digitalWrite(leftBackDirection,1);
  digitalWrite(rightBackDirection,1);
  analogWrite(rightMotors,velocity-offset);
  analogWrite(leftMotors,velocity+offset);
}
//functie om rechtse afwijking op te vangen
void correctRightShort(byte velocity) {
  digitalWrite(leftFrontDirection,0);
  digitalWrite(rightFrontDirection,0);
  digitalWrite(leftBackDirection,1);
  digitalWrite(rightBackDirection,1);
  analogWrite(leftMotors,velocity-offset);
  analogWrite(rightMotors,velocity+offset);
}

//functie om linkse afwijking op te vangen
void correctLeftLong(byte velocity) {
  digitalWrite(leftFrontDirection,0);
  digitalWrite(rightFrontDirection,0);
  digitalWrite(leftBackDirection,1);
  digitalWrite(rightBackDirection,1);
  analogWrite(rightMotors,velocity);
  analogWrite(leftMotors,velocity+offset);
}
//functie om rechtse afwijking op te vangen
void correctRightLong(byte velocity) {
  digitalWrite(leftFrontDirection,0);
  digitalWrite(rightFrontDirection,0);
  digitalWrite(leftBackDirection,1);
  digitalWrite(rightBackDirection,1);
  analogWrite(leftMotors,velocity);
  analogWrite(rightMotors,velocity+offset);
}

//functie om de robot tot stilstand te brengen
void emergencyStop() {
    analogWrite(leftMotors,0);
    analogWrite(rightMotors,0);
}
