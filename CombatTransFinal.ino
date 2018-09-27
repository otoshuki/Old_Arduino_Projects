#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include <RH_ASK.h>
#include <SPI.h> 
RH_ASK driver;
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

MPU6050 mpu;

#define INTERRUPT_PIN 2

int cnt = 0;
int pPin = A15;
int mPin = A14;
int rPin = A12;
int tPin = A11;
int lPin = A10;
int rstPin = 6;
int pushPin = 7;
int ledgPin = 8;
int ledrPin = 9;
char msg = "";
int push = 0;
bool blinkState = false;

// MPU control/status vars
bool dmpReady = false;
uint8_t mpuIntStatus;   
uint8_t devStatus;      
uint16_t packetSize;   
uint16_t fifoCount;    
uint8_t fifoBuffer[64]; 

Quaternion q;           
VectorInt16 aa;       
VectorInt16 aaReal;    
VectorInt16 aaWorld;    
VectorFloat gravity;    
float euler[3];         
float ypr[3];           

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}


void setup() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif
    
    Serial.begin(9600);
    pinMode(ledgPin, OUTPUT);
    pinMode(ledrPin, OUTPUT);
    pinMode(pushPin, INPUT);

    // initialize device
    mpu.initialize();
    pinMode(INTERRUPT_PIN, INPUT);

    // verify connection
    // load and configure the DMP
    devStatus = mpu.dmpInitialize();

    // supply your own gyro offsets here, scaled for min sensitivity
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788); // 1688 factory default for my test chip

    // make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        // turn on the DMP, now that it's ready
        mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection
        attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    }
}

void loop() {
    digitalWrite(ledrPin, HIGH);
    push = digitalRead(pushPin);
            if (push == 1)
        {
     // if programming failed, don't try to do anything
    if (!dmpReady) return;

    // wait for MPU interrupt or extra packet(s) available
    while (!mpuInterrupt && fifoCount < packetSize) {
    }

    // reset interrupt flag and get INT_STATUS byte
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();

    // get current FIFO count
    fifoCount = mpu.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        // reset so we can continue cleanly
        mpu.resetFIFO();
    // otherwise, check for DMP data ready interrupt (this should happen frequently)
    } 
    else if (mpuIntStatus & 0x02) {
        // wait for correct available data length, should be a VERY short wait
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

        // read a packet from FIFO
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        
        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        fifoCount -= packetSize;        

          digitalWrite(ledrPin, LOW);
          delay(2000);
                      int pVal = analogRead(pPin);
          pVal = map(pVal, 650, 900, 20, 0); // Point
          int mVal = analogRead(mPin);
          mVal = map(mVal, 750, 950, 0, 20); // Middle
          int rVal = analogRead(rPin);
          rVal = map(rVal, 750, 950, 0, 20); // Ring
          int tVal = analogRead(tPin);
          tVal = map(tVal, 750, 950, 0, 20); // Thumb
          int lVal = analogRead(lPin);
          lVal = map(lVal, 750, 950, 0, 20); // Little
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetAccel(&aa, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
            
          
    if (pVal>=15 & mVal<=10 & rVal<=12 & tVal>=10 & lVal<=12)// 1.Affirmative
    {
      digitalWrite(ledgPin, HIGH);
      digitalWrite(ledrPin, LOW);
      char msg = 49;
      Serial.print(msg);
      driver.send((char *)&msg, sizeof(msg));
      driver.waitPacketSent(); 
      delay(1000);
      digitalWrite(ledgPin, LOW);
    }
     if (pVal<=15 & mVal<=15 & rVal>=13 & tVal<=15 & lVal>= 14)// 2.Storm the Front!
    {
      digitalWrite(ledgPin, HIGH);
      digitalWrite(ledrPin, LOW);
      char msg = 50;
      Serial.print(msg);
      driver.send((char *)&msg, sizeof(msg));
      driver.waitPacketSent(); 
      delay(1000);
      digitalWrite(ledgPin, LOW);
    }
    if (pVal>=15 & mVal>=17 & rVal>=15 & tVal>=10 & lVal>=14)// 3.Sniper Ahead!
    {
      digitalWrite(ledgPin, HIGH);
      digitalWrite(ledrPin, LOW);
      char msg = 51;
      Serial.print(msg);
      driver.send((char *)&msg, sizeof(msg));
      driver.waitPacketSent(); 
      delay(1000);
      digitalWrite(ledgPin, LOW);
    }
    if (13<=pVal & pVal<=16 & 13<=mVal & mVal<=18 & rVal>=15 & tVal>=10 & lVal>=14)// 4.Move into Formation
    {
      digitalWrite(ledgPin, HIGH);
      digitalWrite(ledrPin, LOW);
      char msg = 52;
      Serial.print(msg);
      driver.send((char *)&msg, sizeof(msg));
      driver.waitPacketSent(); 
      delay(1000);
      digitalWrite(ledgPin, LOW);
    }
    if (pVal<=11 & mVal<=11 & rVal>=15 & tVal>=10 & lVal>=14)// 5.Area Clear!
    {
      digitalWrite(ledgPin, HIGH);
      digitalWrite(ledrPin, LOW);
      char msg = 53;
      Serial.print(msg);
      driver.send((char *)&msg, sizeof(msg));
      driver.waitPacketSent(); 
      delay(1000);
      digitalWrite(ledgPin, LOW);
    }
    if (pVal<=11 & mVal<=11 & rVal<=12 & tVal<=12 & lVal<=12 & (aaReal.y - 600) <= -5000)// 6. Retreat!
    {
      digitalWrite(ledgPin, HIGH);
      digitalWrite(ledrPin, LOW);
      char msg = 54;
      Serial.print(msg);
      driver.send((char *)&msg, sizeof(msg));
      driver.waitPacketSent(); 
      delay(1000);
      digitalWrite(ledgPin, LOW);
    }
    
    if (pVal<=11 & mVal<=11 & rVal<=12 & tVal<=12 & lVal<=12 & (aaReal.z - 3100) >= 4000)// 7. Crouch!
    {
      digitalWrite(ledgPin, HIGH);
      digitalWrite(ledrPin, LOW);
      char msg = 55;
      Serial.print(msg);
      driver.send((char *)&msg, sizeof(msg));
      driver.waitPacketSent(); 
      delay(1000);
      digitalWrite(ledgPin, LOW);
    }
    if (pVal>=15 & mVal>=17 & rVal>=15 & tVal>=10 & lVal>=14 & cnt >= 2)// 8. Hurry Up
    {
      digitalWrite(ledgPin, HIGH);
      digitalWrite(ledrPin, LOW);
      char msg = 56;
      cnt = 0;
      Serial.print(msg);
      driver.send((char *)&msg, sizeof(msg));
      driver.waitPacketSent(); 
      delay(1000);
      digitalWrite(ledgPin, LOW);
    }
    if (pVal>=15 & mVal>=17 & rVal>=15 & tVal>=10 & lVal>=14 & aaReal.x <= -6000)
    {
      cnt = cnt + 1;
      delay(500);
    }
    else
    {
      digitalWrite(ledrPin, HIGH);
      delay(500);
      digitalWrite(ledrPin, LOW);
      delay(500);
      digitalWrite(ledrPin, HIGH);
    }
    digitalWrite(ledgPin, LOW);
     digitalWrite(rstPin, HIGH);
   }
  
        }
  else if (push == 0)
  {
    delay(1000);
    digitalWrite(ledgPin, LOW);
    Serial.println(push);
    push = 0;
  } 
    
    
}
