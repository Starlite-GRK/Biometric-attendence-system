 /*
  AS608-Optical-Fingerprint-Sensor-enroll
    based on Adafruit Library

*/
#include <Adafruit_Fingerprint.h>

#include <LiquidCrystal.h>

LiquidCrystal lcd(13,12,11,10,9,8);

SoftwareSerial mySerial(2,3); // TX/RX

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

uint8_t id;

void setup()  
{
  lcd.begin(16, 2);

  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  
  Serial.begin(9600);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit Fingerprint sensor enrollment");
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
}

uint8_t readnumber(void) 
{
  uint8_t num = 0;
  
  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}

void loop()                     // run over and over again
{
  Serial.println("Ready to enroll a fingerprint!");
  Serial.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Ready to enroll");
  lcd.setCursor(0, 1);
  lcd.print("Type ID #");
  id = readnumber();
  if (id == 0) {// ID #0 not allowed, try again!
     return;
  }
  Serial.print("Enrolling ID #");
  Serial.println(id);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enrolling ID#");
  lcd.print(id);
  ledprocess(); 
  while (!  getFingerprintEnroll() );
}

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

uint8_t getFingerprintEnroll() 
{

  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Place finger");
  while (p != FINGERPRINT_OK) 
  {
    p = finger.getImage();
    switch (p) 
    {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Image taken");
      ledok();
      break;
    case FINGERPRINT_NOFINGER: lederror();
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Communication error");
      lederror();
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Imaging error");
      lederror();
      break;
    default:
      Serial.println("Unknown error");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Unknown error");
      lederror();
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) 
  {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Image converted");
      ledok();
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Image too messy");
      lederror();
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Communication error");
      lederror();
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Features fail");
      lederror();
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Invalid image");
      lederror();
      return p;
    default:
      Serial.println("Unknown error");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Unknown error");
      lederror();
      return p;
  }
  
  Serial.println("Remove finger");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) 
  {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Place same finger");
  ledprocess();
  
  while (p != FINGERPRINT_OK) 
  {
    p = finger.getImage();
    switch (p) 
    {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Image taken");
      ledok();
      break;
    case FINGERPRINT_NOFINGER:lederror();
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Communication error");
      lederror();
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Imaging error");
      lederror();
      break;
    default:
      Serial.println("Unknown error");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Unknown error");
      lederror();
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) 
  {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Image converted");
      ledok();
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Image too messy");
      lederror();
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Communication error");
      lederror();
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Feature fail");
      lederror();
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Invalid image");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Unknown error");
      lederror();
      return p;
    default:
      Serial.println("Unknown error");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Unknown error");
      lederror();
      return p;
  }
  
  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Creating model for #");
  lcd.print(id);
  p = finger.createModel();
  if (p == FINGERPRINT_OK) 
  {
    Serial.println("Prints matched!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Prints matched!");
    ledok();
  } 
  else if (p == FINGERPRINT_PACKETRECIEVEERR) 
  {
    Serial.println("Communication error");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Communication error");
    lederror();
    return p;
  } 
  else if (p == FINGERPRINT_ENROLLMISMATCH) 
  {
    Serial.println("Fingerprints did not match");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Fingerprints did not match");
    lederror();
    return p;
  } 
  else 
  {
    Serial.println("Unknown error");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Unknown error");
    lederror();
    return p;
  }   
  
  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) 
  {
    Serial.println("Stored!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Stored!");
    ledok();
  } 
  else if (p == FINGERPRINT_PACKETRECIEVEERR) 
  {
    Serial.println("Communication error");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Communication error");
    lederror();
    return p;
  } 
  else if (p == FINGERPRINT_BADLOCATION) 
  {
    Serial.println("Could not store in that location");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Bad location");
    lederror();
    return p;
  } 
  else if (p == FINGERPRINT_FLASHERR) 
  {
    Serial.println("Error writing to flash");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Flash error");
    lederror();
    return p;
  } 
  else 
  {
    Serial.println("Unknown error");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Unknown error");
    lederror();
    return p;
  }   
}
