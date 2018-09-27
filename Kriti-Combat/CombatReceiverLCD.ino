#include <RH_ASK.h>
#include <SPI.h>
#include <LiquidCrystal.h>
RH_ASK driver;
String str = "";
int ledPin = 13;
int num = 0;
LiquidCrystal lcd(12, 10, 5, 4, 3, 2);
void setup()
{
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  lcd.begin(16, 2);
  if (!driver.init())
  {
    Serial.println("init failed");
  }
}

void loop()
{
  lcd.clear();
  digitalWrite(ledPin, LOW);
  char buf[2];
  if (driver.recv(buf, sizeof(buf))) 
  {
    int i; 
    digitalWrite(ledPin, HIGH);
    Serial.println(buf);
    str = Serial.read();
    str = str.substring(0, 1);
    num = str.toInt();
    Serial.println(str);
  } 
  if (num == 1)
  {
    digitalWrite(ledPin, HIGH);
    Serial.println("Affirmative");
    lcd.write(Serial.read());
  }
  else if (num == 2)
  {
    digitalWrite(ledPin, HIGH);
    Serial.println("Storm the Front!");
    lcd.write(Serial.read());
  }
  else if (Serial.read() == 3)
  {
    digitalWrite(ledPin, HIGH);
    Serial.println("Sniper Ahead!");
    lcd.write(Serial.read());
  }
  else if (Serial.read() == 4)
  {
    digitalWrite(ledPin, HIGH);
    Serial.println("Move into Formation");
    lcd.write(Serial.read());
  }
  else if (Serial.read() == "5")
  {
    digitalWrite(ledPin, HIGH);
    Serial.println("Area Clear");
    lcd.write(Serial.read());
  }
  else if (Serial.read() == 6)
  {
    digitalWrite(ledPin, HIGH);
    Serial.print("Retreat");
    lcd.write(Serial.read());
  }
  else if (Serial.read() == 7)
  {
    digitalWrite(ledPin, HIGH);
    Serial.println("Crouch or Go Prone");
    lcd.write(Serial.read());
  }
  else if (Serial.read() == 8)
  {
    digitalWrite(ledPin, HIGH);
    Serial.println("Hurry Up!");
    lcd.write(Serial.read());
  }
}
