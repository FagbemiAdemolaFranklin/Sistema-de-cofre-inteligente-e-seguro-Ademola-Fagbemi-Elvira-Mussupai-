# include <LiquidCrystal.h>
# include <Keypad.h>
#include <Servo.h>

String master_password = "1234";
String inserted_password;
String inserted_new_password_1;
String inserted_new_password_2;
String my_password;
String master_key_code;
int j = 0;
int k = 0; 
int l = 0;
bool servo_state = false;
bool is_typing = false;

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

void setPassword(){
  char new_key = keypad.getKey();
  if(inserted_new_password_1.length() < 4) {
    if(new_key){
      j++;
      lcd.setCursor(j,1);
      lcd.write(new_key);
      delay(100);
      lcd.clear();
    }
    lcd.home();
    lcd.write("New password");
    is_typing = true;
    String new_password(new_key);
    inserted_new_password_1 += new_password;
    lcd.home();
  }else if(inserted_new_password_1.length() == 4){
    lcd.home();
    lcd.write("Confirm password");
    if(new_key){
      k++;
      lcd.setCursor(k,1);
      lcd.write(new_key);
      delay(100);
      lcd.clear();
      inserted_new_password_2 += String(new_key);
      Serial.println(inserted_new_password_2);
      if(inserted_new_password_2.length() == 4 && inserted_new_password_1.equals(inserted_new_password_2)) {
        lcd.clear();
        lcd.write("Saved");
        delay(1000);
        my_password = inserted_new_password_1;
        loop();
      }else if (inserted_new_password_2.length() == 4 && inserted_new_password_1 != inserted_new_password_2){
        lcd.clear();  
        lcd.write("Mismatch");
        delay(2000);
        servo_state=false;
        inserted_new_password_1 ="";
        inserted_new_password_2 ="";
        lcd.clear();
        setPassword();
      }
    }
  }
}

void setServo(int angle) {
  servo.write(angle);
}


void openDoor(String inputed_key) {
  inserted_password += inputed_key;
  if(inserted_password.length() == 4) {
    if(inserted_password.equals(my_password)){
      setServo(0);
      lcd.clear();
      lcd.write("Opened");
      servo_state = true;
      delay(1000);
    }else if(inserted_password != my_password){
      inserted_password="";
      lcd.clear();
      lcd.write("Incorrect");
      delay(1000);
      lcd.clear();
      lcd.write("Try again");
      delay(1000);
      loop();
    }
  }
  Serial.println(inserted_password);
}

void lockDoor(){
  int servo_current_degree = servo.read();
  if (servo_current_degree != 90) {
    servo.write(90);
    lcd.home();
    lcd.print("Locked");
    delay(2000);
    servo_state = false;
    inserted_password="";
    loop();
  }
  Serial.println(servo_current_degree);
}
void changePassword(String master_key_inputed_code) {
  master_key_code += master_key_inputed_code;
  if(master_key_code.length() == 4) {
    if(master_key_code.equals(master_password)){
      setPassword();
    }else{
      lcd.home();
      lcd.write("Master password incorrect");
    }
  }
}
void setup(){
  servo.attach(10);
  servo.write(90);
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.clear();
  lcd.write("Welcome");
  delay(2000);
  lcd.home();
  lcd.write("* - change pin");
  delay(2000);
  lcd.clear();
  lcd.write("# - lock");
  delay(2000);
  lcd.clear();
}

void loop() {
  char check_key = keypad.getKey();
  String stringified_check_key(check_key);
  while(!servo_state && my_password.length() < 4) {
    setPassword();
  }
  while(!servo_state && my_password.length() > 0) {
    lcd.home();
  	lcd.print("Enter password");
    char inserted_key = keypad.getKey();
    String stringified_inserted_key(inserted_key);
    if(inserted_key) {
      l++;
      lcd.setCursor(l,1);
      lcd.write(inserted_key);
      delay(100);
      lcd.clear();
      openDoor(stringified_inserted_key);
    }
  }
  while(servo_state && stringified_check_key == "#"){
    lockDoor();
    stringified_check_key ="";
  }
  while(!servo_state && stringified_check_key == "*"){
    lcd.home();
  	lcd.print("Master password");
    lcd.setCursor(0,1);
    char inserted_key_2 = keypad.getKey();
    String stringified_inserted_key_2(inserted_key_2);
    if(inserted_key_2) {
      changePassword(stringified_inserted_key_2);
    }
  }
}
