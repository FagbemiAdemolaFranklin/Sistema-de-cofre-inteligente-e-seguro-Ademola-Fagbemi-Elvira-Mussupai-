#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(7,6,2,3,4,5);

const byte rxPin = 9;
const byte txPin = 8;
#define PIN_BUZZER 10
SoftwareSerial BTSerial(rxPin, txPin); // RX TX

void lcdPrint(String receivedMessage) {

  while (receivedMessage) {
    if (receivedMessage.startsWith("Enroll")){
      lcd.clear();
      lcd.println("Enroll fingerprint");
      lcd.clear();
      lcd.println("Keep eye on serial monitor");
      break;
    }else if (receivedMessage.equals("is_typing")){
      lcd.clear();
      lcd.print("Enter passsword");
      break;
    }else if (receivedMessage.length() < 2){
      for(int j = 0; j<=3; j++){
        lcd.setCursor(j,1);
        lcd.print(receivedMessage);
      }
      break;
    }else if (receivedMessage.startsWith("is confirming")){
      lcd.clear();
      lcd.println("Confirm password");
      break;
    }else if (receivedMessage.startsWith("Enter")){
      lcd.clear();
      lcd.println("Enter password");
      break;
    }else if (receivedMessage.startsWith("Activate")){
      activateAlarm();
      break;
    }else{
      lcd.clear();
      lcd.println(receivedMessage);
      break;
    }
  }
}

void activateAlarm(){
  digitalWrite(PIN_BUZZER, HIGH);
  delay(5000);
  digitalWrite(PIN_BUZZER, LOW);
  bluetoothMode();
  
}


void bluetoothMode(){
 while(BTSerial.available() > 0) {
   BTSerial.println("Safe is unsafe");
   delay(1000);
   BTSerial.println("Do you want to change password?");
   delay(1000);
   BTSerial.println("Yes or No ?");
   String bluetooth_message  = BTSerial.readString();
   if (bluetooth_message.equals("Yes")){
     Serial.write("change-password");
   }else if (bluetooth_message.equals("No")) {
      Serial.write("dont-change");
   }
 }
}


void setup() {
  // define pin modes for tx, rx:
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  BTSerial.begin(9600);
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(PIN_BUZZER, OUTPUT);
}

String messageBuffer = "";
String message = "";

void loop() {
  while (Serial.available() > 0) {
    String received_data = Serial.readString();
    lcdPrint(received_data);
  }
}
