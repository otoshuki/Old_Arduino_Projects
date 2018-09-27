/*Kameng Team - Robotics Club Hackathon */

//defining the input pins for A
int enA = 11;
int in1 = 12;
int in2 = 13;

//defining the input pins for B
int enB = 5;
int in3 = 6;
int in4 = 7;

void setup() {
  //Initialising the Serial
  Serial.begin(9600);
  
  //Setting the INPUT pins
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  
  //Setting the OUTPUT pins
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

   //Turning the A motor off before starting
  analogWrite(enA, LOW);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);

  //Turning the B motor off before starting
  analogWrite(enB, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void loop() {
  //Reading the values from IR Sensor Array
  int ir1 = analogRead(A0);
  int ir2 = analogRead(A1);
  int ir3 = analogRead(A2);
  int ir4 = analogRead(A3);

  

  //Forward
  if (ir4 > 600  && ir1 > 200)
  {
    analogWrite(enA, 70);
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(enB, 70);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    Serial.println("Forward");
  }
 /* //Forward 2nd Case
  if (ir4 > 600 &&  ir1 > 200)
  {
    analogWrite(enA, 80);
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(enB, 105);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  }
  */  
  //Sharp Right 
  else if (ir4 < 600  && ir1 > 200)
  {
    Serial.println("Right");
    analogWrite(enA, 100);
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(enB, 100);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  }
 
  //Sharp Left
  else if (ir4 > 600  && ir1 < 200)
  {
    Serial.println("Left");
    analogWrite(enA, 100);
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    analogWrite(enB, 100);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  }
  
 /* 
  //Soft Right
  else if (ir4 > 600 && ir3 > 200 && ir2 < 200 && ir1 < 200)
  {
    analogWrite(enA, 80);
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(enB, 60);
    digitalWrit12e(in3, LOW);
    digitalWrite(in4, HIGH);
  }
  //Soft Left
  else if (ir4 < 600 && ir3 < 200 && ir2 > 200 && ir1 > 200)
  {
    analogWrite(enA, 60);
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    analogWrite(enB, 80);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  }
*/
  //Stop
  else if (ir4 < 600 && ir1 < 200)
  {
    analogWrite(enA, 80);
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(enB, 90);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    
  }
}
