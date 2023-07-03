/*
  AS608-Optical-Fingerprint-Sensor-verify
    based on Adafruit Library

*/

#include <Adafruit_Fingerprint.h>

#include <LiquidCrystal.h>

LiquidCrystal lcd(13,12,11,10,9,8);

volatile int finger_status = -1;

SoftwareSerial mySerial(2, 3); // TX/RX on fingerprint sensor

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup()  
{
   lcd.begin(16, 2);

  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  
  Serial.begin(9600);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit finger detect test");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enrolling sensor");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  
  if (finger.verifyPassword()) 
  {
    Serial.println("Found fingerprint sensor!");
     lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Found");
  } 
  else 
  {
    Serial.println("Did not find fingerprint sensor :(");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Not found");
    while (1) { delay(1); }
  }

  finger.getTemplateCount();
  Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  Serial.println("Waiting for valid finger...");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Place finger");
}

void loop()                     // run over and over again
{
  ledprocess();
  finger_status = getFingerprintIDez();
  if (finger_status!=-1 and finger_status!=-2)
  {
    Serial.print("Match");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Match");
    ledok();
  } 
  else{
    if (finger_status==-2){
      for (int ii=0;ii<5;ii++){
        Serial.print("Not Match");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Not Match");
        lederror();
      }
    }
  }
  delay(50);            //don't need to run this at full speed.
}

// returns -1 if failed, otherwise returns ID #

void ledprocess()
{
  digitalWrite(7, HIGH);
  delay(500);
  digitalWrite(6, HIGH);
  delay(500);
  digitalWrite(5, HIGH);
  delay(500);
  digitalWrite(5, LOW);
  delay(500);
  digitalWrite(6, LOW);
  delay(500);
  digitalWrite(7, LOW);
  delay(500);
}

void ledok()
{
  digitalWrite(7, HIGH);
  delay(500);
  digitalWrite(7, LOW);
  delay(500);
}

void lederror()
{
  digitalWrite(5, HIGH);
  delay(500);
  digitalWrite(5, LOW);
  delay(500);
}

int getFingerprintIDez() 
{
  uint8_t p = finger.getImage();
  if (p!=2){
    Serial.println(p);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(p);
  }
  if (p != FINGERPRINT_OK)  return -1;
  
  p = finger.image2Tz();
  if (p!=2){
    Serial.println(p);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(p);
  }
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -2;
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Found ID#");
  lcd.print(finger.fingerID);
  lcd.setCursor(0, 1);
  lcd.print("Confidence:");
  lcd.print(finger.confidence);
  ledok();
  return finger.fingerID; 
}
