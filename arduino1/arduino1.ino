/***************************************************
  This is an example sketch for our optical Fingerprint sensor

  Designed specifically to work with the Adafruit BMP085 Breakout
  ----> http://www.adafruit.com/products/751

  These displays use TTL Serial to communicate, 2 pins are required to
  interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/


#include <Adafruit_Fingerprint.h>
# include <Keypad.h>
# include <Servo.h>
#include "HUSKYLENS.h"
#include "SoftwareSerial.h"
HUSKYLENS huskylens;
SoftwareSerial mySerial(A3, A2); // RX, TX
//HUSKYLENS green line >> Pin 10; blue line >> Pin 11
void printResult(HUSKYLENSResult result);



#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
// For UNO and others without hardware serial, we must use software serial...
// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
// Set up the serial port to use softwareserial..
SoftwareSerial mySerial(A4, A5);

#else
// On Leonardo/M0/etc, others with hardware serial, use hardware serial!
// #0 is green wire, #1 is white
#define mySerial Serial1

#endif


Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
int red_led = 11;
int green_led = 12;
int relay_pin = 10;
String master_password = "1234";
String inserted_password;
String inserted_new_password_1;
String inserted_new_password_2;
String my_password;
String master_key_code;
String received_message;
int biometric_try = 0;
int code_try = 0;
bool servo_state = false;
bool is_typing = false;
bool doorState;
bool enrollFingerButton;
int counter = 0;
Servo servo;
const byte rows = 4;
const byte columns = 4;
byte rowPins[rows] = {2,3,4,5};
byte colPins[columns] = {6,7,8,9};
char keys[rows][columns] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, columns);
String old_message;
void checkPrintState(String old_param){ // function to print on serial monitor.
  String new_message = old_param;
  if (old_message != new_message){
    if(old_param.endsWith(String('\n')))
      old_param.remove(old_param.length()-2);
    Serial.println(new_message); // this is to make the connection to other arduino so as to print whatever is being typed/sent on the lcd.
    old_message=old_param;
  }
}


void setPassword(){
  char new_key = keypad.getKey();
  if(inserted_new_password_1.length() < 4) { //checks the length of the declared variable inserted_new_password_1
    is_typing = true;
    checkPrintState("typing");
    if (new_key){
      String new_key_string(new_key);
      Serial.println(new_key);
      delay(1000);
      inserted_new_password_1 += new_key_string;
    }
  }
  else if(inserted_new_password_1.length() == 4){
    for (int i = 0; i<=2;){
      i++;
    }
    is_typing=true;
    checkPrintState("confirming"); //  this is a signal to the lcd on arduino 2, expected to show confirming password on lcd.
    if(new_key && inserted_new_password_2.length() < 4){
      inserted_new_password_2 += String(new_key);
      Serial.println(new_key); // each characters of the confirm password is sent to the second arduino so as to be printed on the screen.
    }else if(inserted_new_password_2.length() == 4 && inserted_new_password_1.equals(inserted_new_password_2)) {
      delay(2000);
      checkPrintState("saved");  // Saved should be displayed on lcd.
      delay(2000);
      my_password = inserted_new_password_1;
      loop();
    }else if (inserted_new_password_2.length() == 4 && inserted_new_password_1 != inserted_new_password_2){
      delay(1000);
      checkPrintState("mismatch");  // Mismatch should be displayed on lcd screen
      delay(2000);
      inserted_new_password_1 ="";
      inserted_new_password_2 ="";
      setPassword();
    }
    
  }
}


void openDoor(String inputed_key) {
  inserted_password += inputed_key;
  if(inserted_password.length() == 4) {
    if(inserted_password.equals(my_password)){
      activateSolenoid(); // a function that with all features of the door at an open state followed by a close state.
    }else if(inserted_password != my_password){
      inserted_password="";
      delay(2000);
      checkPrintState("Incorrect"); 
      delay(3000);
      checkPrintState("Try again"); 
      code_try+=1;
      delay(2000);
      loop();
    }
  }
}

void lockDoor(){
  delay(2000);
  digitalWrite(relay_pin, LOW);
  digitalWrite(green_led, LOW);
  digitalWrite(red_led, HIGH);   
  doorState = false;
  biometric_try=0;   
  code_try=0;
  checkPrintState("Locked");
  delay(2000);
  inserted_password="";
}

void activateSolenoid(){   // code for supplying current to the relay to activate the solenoid.
  delay(2000);
  checkPrintState("open door");  // open door displayed on the screen.
  doorState = true;
  digitalWrite(green_led, HIGH);
  digitalWrite(red_led, LOW);
  digitalWrite(relay_pin, HIGH);
  delay(2000);
  lockDoor();
}

void setup()
{
  pinMode(red_led, OUTPUT);
  pinMode(green_led, OUTPUT);
  pinMode(relay_pin, OUTPUT);
  digitalWrite(red_led, HIGH);
  digitalWrite(relay_pin, LOW);
  Serial.begin(9600);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit finger detect test");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  delay(5);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }

  Serial.println(F("Reading sensor parameters"));
  finger.getParameters();
  Serial.print(F("Status: 0x")); Serial.println(finger.status_reg, HEX);
  Serial.print(F("Sys ID: 0x")); Serial.println(finger.system_id, HEX);
  Serial.print(F("Capacity: ")); Serial.println(finger.capacity);
  Serial.print(F("Security level: ")); Serial.println(finger.security_level);
  Serial.print(F("Device address: ")); Serial.println(finger.device_addr, HEX);
  Serial.print(F("Packet len: ")); Serial.println(finger.packet_len);
  Serial.print(F("Baud rate: ")); Serial.println(finger.baud_rate);

  finger.getTemplateCount();

  if (finger.templateCount == 0) {
    Serial.print("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example.");
  }
  else {
    Serial.println("Waiting for valid finger...");
    Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
    delay(3000);
  }
}

void loop()                     // run over and over again
{
  while(my_password.length()<4){
    setPassword();
  }
  while (my_password.length() ==4 && biometric_try <= 2) {
    getFingerprintID();
  }
  while (biometric_try == 3) {
    if (code_try <= 2) {
      checkPrintState("writing"); // lcd must print enter password
      char inserted_key = keypad.getKey();
      String stringified_inserted_key(inserted_key);
      if(inserted_key) {
        Serial.println(inserted_key);
        openDoor(stringified_inserted_key);
      }
    }
    while (code_try == 3) {
      checkPrintState("Access denied");
      while (code_try == 3 && !receivedValidMessage()) {
        if (received_message.startsWith("Valid:")){
          received_message = received_message.substring(6,10);
          Serial.print(received_message);
          my_password=received_message;
          lockDoor();
        }else if (received_message.equals("keep")){
          Serial.print(received_message);
          lockDoor();
        }
      }
      
    }
    
  }
  delay(50);            //don't ned to run this at full speed.
  
  
}

bool receivedValidMessage() {
    while (Serial.available() > 0) {
        received_message = Serial.readString();
        if (received_message.length() > 0) {
            if (received_message.endsWith(String('\n'))) {
                received_message.remove(received_message.length() - 2);
            }

            if (received_message.equals("Access denied")) {
                // Handle "Access denied" message
              return true;
            }
        }
    }
    return false;
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      delay(2000);
      checkPrintState("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      delay(2000);
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      delay(2000);
      Serial.println("Imaging error");
      return p;
    default:
      delay(2000);
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      delay(2000);
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      delay(2000);
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      delay(2000);
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      delay(2000);
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    delay(2000);
    Serial.println("Found a print match!");
    delay(1000);
    activateSolenoid();
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    delay(1000);
    Serial.println("Did not find a match");
    delay(2000);
    biometric_try +=1;
    return p;
  } else {
    delay(2000);
    Serial.println("Unknown error");
    return p;
  }

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);

  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID;
}
