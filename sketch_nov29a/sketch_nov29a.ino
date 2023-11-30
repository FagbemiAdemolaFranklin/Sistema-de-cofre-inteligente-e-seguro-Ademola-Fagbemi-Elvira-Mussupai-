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
# include <LiquidCrystal.h>
# include <Keypad.h>
# include <Servo.h>

uint8_t id;
int red_led = 11;
int green_led = 12;
int relay_pin = 10;
String master_password = "1234";
String inserted_password;
String inserted_new_password_1;
String inserted_new_password_2;
String my_password;
String master_key_code;
int biometric_try = 0;
int code_try = 0;
bool servo_state = false;
bool is_typing = false;
int count = 0;
Servo servo;
LiquidCrystal lcd(A0,A1,A2,A3,A4,A5);
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


// #if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
// // For UNO and others without hardware serial, we must use software serial...
// // pin #2 is IN from sensor (GREEN wire)
// // pin #3 is OUT from arduino  (WHITE wire)
// // Set up the serial port to use softwareserial..
// SoftwareSerial mySerial(2, 3);

// #else
// // On Leonardo/M0/etc, others with hardware serial, use hardware serial!
// // #0 is green wire, #1 is white
// #define mySerial Serial1

// #endif


// Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

// void enrollFingerprintSetup()
// {
//   delay(100);
//   Serial.println("\n\nAdafruit Fingerprint sensor enrollment");

//   // set the data rate for the sensor serial port
//   finger.begin(57600);

//   if (finger.verifyPassword()) {
//     Serial.println("Found fingerprint sensor!");
//   } else {
//     Serial.println("Did not find fingerprint sensor :(");
//     while (1) { delay(1); }
//   }

//   Serial.println(F("Reading sensor parameters"));
//   finger.getParameters();
//   Serial.print(F("Status: 0x")); Serial.println(finger.status_reg, HEX);
//   Serial.print(F("Sys ID: 0x")); Serial.println(finger.system_id, HEX);
//   Serial.print(F("Capacity: ")); Serial.println(finger.capacity);
//   Serial.print(F("Security level: ")); Serial.println(finger.security_level);
//   Serial.print(F("Device address: ")); Serial.println(finger.device_addr, HEX);
//   Serial.print(F("Packet len: ")); Serial.println(finger.packet_len);
//   Serial.print(F("Baud rate: ")); Serial.println(finger.baud_rate);
// }

// uint8_t readnumber(void) {
//   uint8_t num = 0;

//   while (num == 0) {
//     while (! Serial.available());
//     num = Serial.parseInt();
//   }
//   return num;
// }

// void loopFingerprintSetup()                     // run over and over again
//   {
//     Serial.println("Ready to enroll a fingerprint!");
//     Serial.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");
//     id = readnumber();
//     if (id == 0) {// ID #0 not allowed, try again!
//       return;
//     }
//     Serial.print("Enrolling ID #");
//     Serial.println(id);

//     while (!  getFingerprintEnroll() );
//   }

// uint8_t getFingerprintEnroll() {
  
//   int p = -1;
//   Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
//   while (p != FINGERPRINT_OK) {
//     p = finger.getImage();
//     switch (p) {
//     case FINGERPRINT_OK:
//       Serial.println("Image taken");
//       break;
//     case FINGERPRINT_NOFINGER:
//       Serial.println(".");
//       break;
//     case FINGERPRINT_PACKETRECIEVEERR:
//       Serial.println("Communication error");
//       break;
//     case FINGERPRINT_IMAGEFAIL:
//       Serial.println("Imaging error");
//       break;
//     default:
//       Serial.println("Unknown error");
//       break;
//     }
//   }

//   // OK success!

//   p = finger.image2Tz(1);
//   switch (p) {
//     case FINGERPRINT_OK:
//       Serial.println("Image converted");
//       break;
//     case FINGERPRINT_IMAGEMESS:
//       Serial.println("Image too messy");
//       return p;
//     case FINGERPRINT_PACKETRECIEVEERR:
//       Serial.println("Communication error");
//       return p;
//     case FINGERPRINT_FEATUREFAIL:
//       Serial.println("Could not find fingerprint features");
//       return p;
//     case FINGERPRINT_INVALIDIMAGE:
//       Serial.println("Could not find fingerprint features");
//       return p;
//     default:
//       Serial.println("Unknown error");
//       return p;
//   }

//   Serial.println("Remove finger");
//   delay(2000);
//   p = 0;
//   while (p != FINGERPRINT_NOFINGER) {
//     p = finger.getImage();
//   }
//   Serial.print("ID "); Serial.println(id);
//   p = -1;
//   Serial.println("Place same finger again");
//   while (p != FINGERPRINT_OK) {
//     p = finger.getImage();
//     switch (p) {
//     case FINGERPRINT_OK:
//       Serial.println("Image taken");
//       break;
//     case FINGERPRINT_NOFINGER:
//       Serial.print(".");
//       break;
//     case FINGERPRINT_PACKETRECIEVEERR:
//       Serial.println("Communication error");
//       break;
//     case FINGERPRINT_IMAGEFAIL:
//       Serial.println("Imaging error");
//       break;
//     default:
//       Serial.println("Unknown error");
//       break;
//     }
//   }

//   // OK success!

//   p = finger.image2Tz(2);
//   switch (p) {
//     case FINGERPRINT_OK:
//       Serial.println("Image converted");
//       break;
//     case FINGERPRINT_IMAGEMESS:
//       Serial.println("Image too messy");
//       return p;
//     case FINGERPRINT_PACKETRECIEVEERR:
//       Serial.println("Communication error");
//       return p;
//     case FINGERPRINT_FEATUREFAIL:
//       Serial.println("Could not find fingerprint features");
//       return p;
//     case FINGERPRINT_INVALIDIMAGE:
//       Serial.println("Could not find fingerprint features");
//       return p;
//     default:
//       Serial.println("Unknown error");
//       return p;
//   }

//   // OK converted!
//   Serial.print("Creating model for #");  Serial.println(id);

//   p = finger.createModel();
//   if (p == FINGERPRINT_OK) {
//     Serial.println("Prints matched!");
//   } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
//     Serial.println("Communication error");
//     return p;
//   } else if (p == FINGERPRINT_ENROLLMISMATCH) {
//     Serial.println("Fingerprints did not match");
//     return p;
//   } else {
//     Serial.println("Unknown error");
//     return p;
//   }

//   Serial.print("ID "); Serial.println(id);
//   p = finger.storeModel(id);
//   if (p == FINGERPRINT_OK) {
//     Serial.println("Stored!");
//   } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
//     Serial.println("Communication error");
//     return p;
//   } else if (p == FINGERPRINT_BADLOCATION) {
//     Serial.println("Could not store in that location");
//     return p;
//   } else if (p == FINGERPRINT_FLASHERR) {
//     Serial.println("Error writing to flash");
//     return p;
//   } else {
//     Serial.println("Unknown error");
//     return p;
//   }
//   return true;
// // }
// void enrollNewFingerprint() {
//   Serial.write("Enrolling");
//   Serial.write("Keep eyes on serial monitor"); 
//     /***************************************************
//     This is an example sketch for our optical Fingerprint sensor

//     Designed specifically to work with the Adafruit BMP085 Breakout
//     ----> http://www.adafruit.com/products/751

//     These displays use TTL Serial to communicate, 2 pins are required to
//     interface
//     Adafruit invests time and resources providing this open source code,
//     please support Adafruit and open-source hardware by purchasing
//     products from Adafruit!

//     Written by Limor Fried/Ladyada for Adafruit Industries.
//     BSD license, all text above must be included in any redistribution
//   ****************************************************/

//   // enrollFingerprintSetup();
//   loopFingerprintSetup();
// }

void setPassword(){
  char new_key = keypad.getKey();
  if(inserted_new_password_1.length() < 4) {
    // if(new_key){
    //   j++;
    //   lcd.setCursor(j,1);
    //   lcd.write(new_key);
    //   delay(100);
    //   lcd.clear();
    // }
    // lcd.home();
    // lcd.write("New password");
    is_typing = true;
    
    Serial.println("is_typing"); // this is to make the connection open for setting password so as to print whatever is being typed on the lcd.
    String new_key_string(new_key);
    Serial.println(new_key);
    inserted_new_password_1 += new_key_string;
  }else if(inserted_new_password_1.length() == 4){
    // lcd.home();
    // lcd.write("Confirm password");
    Serial.println("is confirming password"); //  this is a signal to the lcd on arduino 2, expected to show confirming password on lcd.
    if(new_key){
      // k++;
      // lcd.setCursor(k,1);
      // lcd.write(new_key);
      // delay(100);
      // lcd.clear();
      inserted_new_password_2 += String(new_key);
      // Serial.println(inserted_new_password_2);
      Serial.println(new_key); // each chharacter of the confirm password is sent to the second arduino so as to be printed on the screen.
      if(inserted_new_password_2.length() == 4 && inserted_new_password_1.equals(inserted_new_password_2)) {
        Serial.write("Saved"); // Saved should be displayed on lcd.
        delay(1000);
        my_password = inserted_new_password_1;
        loop();
      }else if (inserted_new_password_2.length() == 4 && inserted_new_password_1 != inserted_new_password_2){
        Serial.write("Mismatch"); // Mismatch should be displayed on lcd screen
        delay(2000);
        servo_state=false;
        inserted_new_password_1 ="";
        inserted_new_password_2 ="";
        setPassword();
      }
    }
  }
}

void openDoor(String inputed_key) {
  inserted_password += inputed_key;
  Serial.println("Writing"); // print each chracters inserted
  if(inserted_password.length() == 4) {
    if(inserted_password.equals(my_password)){
      servo.write(0);
      delay(3000);
      // lcd.clear();
      // lcd.write("Opened");
      Serial.println("Open Door"); // open door displayed on the screen.
      
      digitalWrite(green_led, HIGH);
      delay(2000);
      digitalWrite(red_led, LOW);
      servo_state = true;
      delay(1000);
    }else if(inserted_password != my_password){
      inserted_password="";
      // lcd.clear();
      // lcd.write("Incorrect");
      Serial.println("Incorrect");
      delay(1000);
      // lcd.clear();
      // lcd.write("Try again");
      Serial.write("Try again");
      delay(1000);
      loop();
    }
  }
}

void lockDoor(){
  int servo_current_degree = servo.read();
  if (servo_current_degree != 90) {
    servo.write(90);
    // lcd.home();
    // lcd.print("Locked");
    Serial.write("Locked"); //lcd must print locked
    delay(2000);
    digitalWrite(relay_pin, LOW);
    servo_state = false;
    inserted_password="";
    loop();
  }
}

void setup()
{
  pinMode(red_led, OUTPUT);
  pinMode(green_led, OUTPUT);
  pinMode(relay_pin, OUTPUT);
  digitalWrite(red_led, HIGH);
  servo.write(90);
  delay(3000);
  // digitalWrite(relay_pin, LOW);
  Serial.begin(9600);
  
  // while (!Serial);  // For Yun/Leo/Micro/Zero/...
  // delay(100);
  // Serial.println("\n\nAdafruit finger detect test");
  
  // // set the data rate for the sensor serial port
  // finger.begin(57600);
  // delay(5);
  // if (finger.verifyPassword()) {
  //   Serial.println("Found fingerprint sensor!");
  // } else {
  //   Serial.println("Did not find fingerprint sensor :(");
  //   while (1) { delay(1); }
  // }

  // Serial.println(F("Reading sensor parameters"));
  // finger.getParameters();
  // Serial.print(F("Status: 0x")); Serial.println(finger.status_reg, HEX);
  // Serial.print(F("Sys ID: 0x")); Serial.println(finger.system_id, HEX);
  // Serial.print(F("Capacity: ")); Serial.println(finger.capacity);
  // Serial.print(F("Security level: ")); Serial.println(finger.security_level);
  // Serial.print(F("Device address: ")); Serial.println(finger.device_addr, HEX);
  // Serial.print(F("Packet len: ")); Serial.println(finger.packet_len);
  // Serial.print(F("Baud rate: ")); Serial.println(finger.baud_rate);

  // finger.getTemplateCount();

  // if (finger.templateCount == 0) {
  //   Serial.print("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example.");
  //   Serial.write("Enroll fingerprint");
  //   enrollNewFingerprint();
  //   setPassword();
  // }
  // else {
  //   Serial.write("Waiting for valid finger...");
  //   Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  //   setPassword();
  // }
}

void loop()                     // run over and over again
{
  if (my_password.length() < 4){
    setPassword();
  }else if(my_password.length() == 4){
    while (code_try <= 2) {
    // lcd.home();
    // lcd.print("Enter password");
      Serial.println("Enter password"); // lcd must print enter password
      char inserted_key = keypad.getKey();
      String stringified_inserted_key(inserted_key);
      if(inserted_key) {
        // l++;
        // lcd.setCursor(l,1);
        // lcd.write(inserted_key);
        // delay(100);
        // lcd.clear();
        openDoor(stringified_inserted_key);
        if (Serial.readString().equals("Try again")){
          code_try += 1;
        }else {
          code_try = 0;
          break;
        }
    }
    
  }
  while (code_try == 3) {
    counter ++;
    if (counter == 1) {
      Serial.write("Activate alarm"); // activate alarm
    }
    
    String received_message = Serial.readString();
    if (received_message.equals("change-password")){
      setPassword();
    }else if (received_message.equals("dont-change")) {
      code_try = 0;
    }
  }
}
  
  // while (biometric_try <= 2) {
  //   getFingerprintID();
  //   delay(50);            //don't ned to run this at full speed.
  //   if (getFingerprintID() == true) {
  //     biometric_try = 0;
  //     break;
  //   }else{
  //     biometric_try += 1;
  //   }
  // }
  
}

// uint8_t getFingerprintID() {
//   uint8_t p = finger.getImage();
//   switch (p) {
//     case FINGERPRINT_OK:
//       Serial.println("Image taken");
//       break;
//     case FINGERPRINT_NOFINGER:
//       Serial.println("No finger detected");
//       return p;
//     case FINGERPRINT_PACKETRECIEVEERR:
//       Serial.println("Communication error");
//       return p;
//     case FINGERPRINT_IMAGEFAIL:
//       Serial.println("Imaging error");
//       return p;
//     default:
//       Serial.println("Unknown error");
//       return p;
//   }

//   // OK success!

//   p = finger.image2Tz();
//   switch (p) {
//     case FINGERPRINT_OK:
//       Serial.println("Image converted");
//       break;
//     case FINGERPRINT_IMAGEMESS:
//       Serial.println("Image too messy");
//       return p;
//     case FINGERPRINT_PACKETRECIEVEERR:
//       Serial.println("Communication error");
//       return p;
//     case FINGERPRINT_FEATUREFAIL:
//       Serial.println("Could not find fingerprint features");
//       return p;
//     case FINGERPRINT_INVALIDIMAGE:
//       Serial.println("Could not find fingerprint features");
//       return p;
//     default:
//       Serial.println("Unknown error");
//       return p;
//   }

//   // OK converted!
//   p = finger.fingerSearch();
//   if (p == FINGERPRINT_OK) {
//     Serial.println("Found a print match!");
//     Serial.write("Found a print match!");
//     digitalWrite(red_led, LOW);
//     digitalWrite(green_led, HIGH);
//     digitalWrite(relay_pin, HIGH);
//     delay(2000);
//     digitalWrite(relay_pin, LOW);
//     digitalWrite(red_led, HIGH);
//     digitalWrite(green_led, LOW);
//     return true;
//   } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
//     Serial.println("Communication error");
//     Serial.write("Communication error");
//     return p;
//   } else if (p == FINGERPRINT_NOTFOUND) {
//     Serial.println("Did not find a match");
//     Serial.write("Did not find a match");
//     return p;
//   } else {
//     Serial.println("Unknown error");
//     Serial.write("Unknown error");
//     return p;
//   }

//   // found a match!
//   Serial.print("Found ID #"); Serial.print(finger.fingerID);
//   Serial.print(" with confidence of "); Serial.println(finger.confidence);

//   return finger.fingerID;
// }

// // returns -1 if failed, otherwise returns ID #
// int getFingerprintIDez() {
//   uint8_t p = finger.getImage();
//   if (p != FINGERPRINT_OK)  return -1;

//   p = finger.image2Tz();
//   if (p != FINGERPRINT_OK)  return -1;

//   p = finger.fingerFastSearch();
//   if (p != FINGERPRINT_OK)  return -1;

//   // found a match!
//   Serial.print("Found ID #"); Serial.print(finger.fingerID);
//   Serial.print(" with confidence of "); Serial.println(finger.confidence);
//   return finger.fingerID;
// }
