#include "SPI.h"
#include "MFRC522.h"
#include <Servo.h>
#define SS_PIN 10
#define RST_PIN 9
#include <LiquidCrystal.h>
int m=0,n=0;
Servo myservo;
int pos = 0;
MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
const int rs = 0, en = 6, d4 = 5, d5 = 4, d6 = 2, d7 = 1;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
void setup() {
Serial.begin(9600);
SPI.begin();
rfid.PCD_Init();
pinMode(A0, OUTPUT);
pinMode(A1, OUTPUT);
pinMode(A2, INPUT_PULLUP);
pinMode(A3, INPUT_PULLUP);

lcd.begin(16, 2);
myservo.attach(3);
myservo.write(6);
}
void loop() {
m= analogRead(A3);
n= analogRead(A2);
int val_1= map(m, 250 , 850 , 0 , 100);
int val_2= map(n, 250, 850, 0, 100);

if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
return;
// Serial.print(F("PICC type: "));
MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
// Serial.println(rfid.PICC_GetTypeName(piccType));
// Check is the PICC of Classic MIFARE type
if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
Serial.println(F("Your tag is not of type MIFARE Classic."));
return;
}
String strID = "";
for (byte i = 0; i < 4; i++) {
strID +=
(rfid.uid.uidByte[i] < 0x10 ? "0" : "") + String(rfid.uid.uidByte[i], HEX) + (i!=3 ? ":" : ""); } strID.toUpperCase(); Serial.print("Tap card key: "); Serial.println(strID); 
if(strID.indexOf("63:AA:A8:1A")>=0)
{
Serial.println("card accepted");
digitalWrite(A0, HIGH);
lcd.setCursor(0, 1);
lcd.print("Speed Limit= ");
lcd.print(val_2);
delay(100);
lcd.clear();
digitalWrite(A0, LOW);

myservo.write(100);              // tell servo to go to position in variable 'pos'
delay(5000);                   // waits 15ms for the servo to reach the position
myservo.write(6);              // tell servo to go to position in variable 'pos'
delay(15);                       // waits 15ms for the servo to reach the position


}
else
{
digitalWrite(A0, LOW);
}
if(strID.indexOf("63:AA:A8:1A")>=0)
{
Serial.println("Token Accepted");
digitalWrite(A1, HIGH);
lcd.setCursor(0, 1);
lcd.print("Speed Limit= ");
lcd.print(val_1);
delay(500);
lcd.clear();
digitalWrite(A1, LOW);


}
else
{
digitalWrite(A1, LOW);
}

rfid.PICC_HaltA();
rfid.PCD_StopCrypto1();
}
