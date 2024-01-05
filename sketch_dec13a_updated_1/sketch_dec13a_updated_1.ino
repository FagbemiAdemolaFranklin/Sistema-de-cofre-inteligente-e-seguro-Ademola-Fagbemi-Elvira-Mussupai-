#include <Wire.h> 
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

const byte rxPin = 9;
const byte txPin = 8;
#define PIN_BUZZER 10
String messageBuffer = "";
String message = "";
String received_data;
String bluetooth_message;
String new_password;
int j;
bool bluetooth_mode;
SoftwareSerial BTSerial(rxPin, txPin); // RX TX

void lcdPrint(String receivedMessage) {
  Serial.println(receivedMessage);
  if (receivedMessage.length() > 0) {
    if(receivedMessage.endsWith(String('\n')))
    Serial.println(receivedMessage);
      receivedMessage.remove(receivedMessage.length()-2);
  }
  if(receivedMessage.startsWith("Enroll")){
    lcd.clear();
    lcd.print("Enroll fingerprint");
    lcd.clear();
    lcd.print("Keep eye on serial monitor");
  }else if(receivedMessage.startsWith("Did not")){
    lcd.clear();
    lcd.print("False print");
  }else if (receivedMessage.equals("typing")){
    lcd.off();
    lcd.on();
    lcd.clear();
    lcd.print("New passsword");
  }else if (receivedMessage.length() < 2){
    if(j==4){
      j=0;
      lcd.setCursor(j,1);
      lcd.print(receivedMessage);
      lcd.setCursor(j,1);
      delay(1000);
      lcd.print("*");
      j++;
    }else{
      lcd.setCursor(j,1);
      lcd.print(receivedMessage);
      lcd.setCursor(j,1);
      delay(1000);
      lcd.print("*");
      j++;
      receivedMessage="";
    }
    
  }else if (receivedMessage.equals("confirming")){
    lcd.home();
    lcd.print("Confirm password");
  }else if (receivedMessage.startsWith("writing")){
    lcd.clear();
    lcd.print("Enter password");
  }else if (receivedMessage.startsWith("Access")){
    lcd.clear();
    lcd.print("Safe blocked");
    delay(2000);
    lcd.clear();
    lcd.print("Check phone");
    activateAlarm();
  }else if(receivedMessage.startsWith("Waiting")){
    lcd.clear();
    lcd.print("Place valid finger");
  }else if(receivedMessage.equals("Try again")){
    lcd.clear();
    lcd.print("Try again");
  }else if(receivedMessage.equals("Incorrect")){
    lcd.clear();
    lcd.print("Incorrect");
  }else if(receivedMessage.equals("saved")){
    lcd.clear();
    lcd.print("Saved");
  }else if(receivedMessage.equals("mismatch")){
    lcd.clear();
    lcd.print("Mismatch");
  }else if(receivedMessage.equals("No finger detected")){
    lcd.clear();
    lcd.print("Place finger");
  }else if (receivedMessage.endsWith(String("error"))){
    lcd.clear();
    lcd.print("Check scanner");
  }else if(receivedMessage.equals("Found a print match!")){
    lcd.clear();
    lcd.print("Match found");
  }else if(receivedMessage.equals("Locked")){
    lcd.clear();
    lcd.print("Locked");
  }else if(receivedMessage.equals("open door")){
    lcd.clear();
    lcd.print("Opened");
  }
  receivedMessage="";
}

void activateAlarm(){
  received_data="";
  tone(PIN_BUZZER, 490, 5000);
  delay(5000);
  noTone(PIN_BUZZER);
  bluetoothMode();
}


void bluetoothMode(){
  BTSerial.println("Safe is unsafe");
  delay(1000);
  BTSerial.println("Do you want to change password?");
  delay(1000);
  BTSerial.println("Yes or No ?");
  bluetooth_mode = true;
}

// void changePassword(){

// }

void bluetoothDecision(String decision){
  if (decision.equals("Yes")) {
    BTSerial.println("A random password would be generated, please save");
    delay(2000);  
    // BTSerial.println("Enter new password, (only 4 characters from 0-9 and end password with ';')");
    // while (BTSerial.available() > 0) {
    //   received_password_message = BTSerial.readString();
    //   if (received_password_message.length()<4 && received_password_message.) {
    //   statements
    //   }
    // }
    for (int randomCount=0; randomCount<=3; randomCount++){
      randomSeed(analogRead(A0) + analogRead(A1) + analogRead(A2) + millis());
      new_password += String(random(0,9));
      delay(10);
    }
    BTSerial.println("New password generated:");
    BTSerial.println(new_password);
    Serial.print("Valid:");
    Serial.print(new_password);
    new_password="";
  }else if (decision.equals("No")) {
    Serial.println("keep");
  }else{
    bluetooth_mode=true;
  }
}

void setup() {
  // define pin modes for tx, rx:
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  lcd.init();
  lcd.backlight();
  lcd.print("Safe system");
  BTSerial.begin(9600);
  BTSerial.println("Bluetooth connection successful");
  Serial.begin(9600);
  Serial.setTimeout(5);
}


void loop() {
  
  while (Serial.available() > 0) {
    received_data= Serial.readString();
    lcdPrint(received_data);
  }

  while (bluetooth_mode){
    while (BTSerial.available() > 0) {
      bluetooth_message = BTSerial.readString();
      bluetooth_message.trim();
      bluetoothDecision(bluetooth_message);
      bluetooth_mode = false;
      // if(bluetooth_message.endsWith(String('\n')))
      //   bluetooth_message.remove(bluetooth_message.length()-1);
    }
  }
  received_data="";
}