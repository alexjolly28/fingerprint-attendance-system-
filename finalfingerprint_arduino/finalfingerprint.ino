#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
int flag=0;
char lcbuf[16];
int admin=0;
long key=0;
int a1=334;
SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      digitalWrite(9,HIGH);
      //Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      digitalWrite(8,HIGH);
      //Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      digitalWrite(8,HIGH);
      //Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      digitalWrite(8,HIGH);
      //Serial.println("Imaging error");
      return p;
    default:
      digitalWrite(8,HIGH);
      //Serial.println("Unknown error");
      return p;
  }
 p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
    digitalWrite(9,HIGH);
      //Serial.println("Image converted");
      delay(100);
      break;
    case FINGERPRINT_IMAGEMESS:
      digitalWrite(8,HIGH);
      //Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
    digitalWrite(8,HIGH);
      //Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      digitalWrite(8,HIGH);
      //Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      digitalWrite(8,HIGH);
      //Serial.println("Could not find fingerprint features");
      return p;
    default:
      //Serial.println("Unknown error");
      digitalWrite(8,HIGH);
      return p;
  }
    p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    //Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    digitalWrite(8,HIGH);
    //Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    //Serial.println("Did not find a match");
    digitalWrite(8,HIGH);
    return p;
  } else {
    //Serial.println("Unknown error");
    digitalWrite(8,HIGH);
    return p;
  }   
  
  // found a match!

//        Serial.print("#"); 
        //Serial.print(finger.fingerID); 
        fun(finger.fingerID);
        //Serial.println();
        digitalWrite(10,HIGH);
//        timex();
        delay(500);
}
void setup()  
{
  pinMode(8,OUTPUT);     //RED LED
  pinMode(9,OUTPUT);     //ORANGE LED
  pinMode(10,OUTPUT);    //GREEN LED
  while (!Serial);
  Serial.begin(115200);
//  Serial.println("FISAT® FingerPrint Attendance System v1.0");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
//    Serial.println("Found fingerprint sensor!");
  } else {
//    Serial.println("Did not find fingerprint sensor :(");
    while (1);
  }
//  Serial.println("Waiting for valid finger...");
}

void loop()                     // run over and over again
{
  digitalWrite(8,LOW);
  digitalWrite(9,LOW);
  digitalWrite(10,LOW);

  getFingerprintID();
  
  delay(50);            //don't ned to run this at full speed.
}

void fun(int a)
{
  if(a!=a1)
  {
  if(a>=70)
  {
    admin=1;
    if(flag==0)
    key=keygen();
    flag=!flag;
    sendjson(a);
  }
  else
  {
    admin=0;
    if(flag)
    sendjson(a);
  }
  a1=a;
}
}
void sendjson(int fin) 
 {
  String payload = "{";
  payload += "\"serialno\":\"";
  sprintf(lcbuf,"%d",113);
  payload += lcbuf; 

  payload += "\",";
  
  payload += "\"key\":\"";
  
  payload += String(key);
  payload += "\"";

  payload += "\",";
  
  payload += "\"admin\":\"";
  sprintf(lcbuf,"%d",admin);
  payload += lcbuf;
  payload += "\"";
  
  payload += "\",";
  
  payload += "\"finger\":\"";
  sprintf(lcbuf,"%d",fin);
  payload += lcbuf;
  payload += "\"";
  
  payload += "\"process\":\"";
  sprintf(lcbuf,"%d",flag);
  payload += lcbuf;
  payload += "\"";
  payload += "}";
  
  char gpData[150];
  payload.toCharArray( gpData, 150 );
  Serial.println( gpData );
 }
long keygen()
{
  long ht=0;
  ht+=random(1,9);
  ht*=10;
  ht+=random(9);
  ht*=10;
  ht+=random(9);
  ht*=10;
  ht+=random(9);
  ht*=10;
  ht+=random(9);
  return ht;
}
  
