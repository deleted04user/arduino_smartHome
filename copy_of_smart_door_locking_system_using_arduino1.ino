//132 RishiSetpal ServoMotor
#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Servo.h>

Servo myservo;
LiquidCrystal lcd(A0, A1, A2, A3, A4, 11);
#define Password_Lenght 7 // Give enough room for six chars + NULL char
int pos = 0;    // variable to store the servo position
char Data[Password_Lenght]; // 6 is the number of chars it can hold + the null char = 7
char Master[Password_Lenght] = "777777";
byte data_count = 0, master_count = 0;

bool Pass_is_good;

char customKey;
const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
bool door = true;
byte rowPins[ROWS] = {1, 2, 3, 4}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 6, 7}; //connect to the column pinouts of the keypad
Keypad customKeypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS); //initialize an instance of class NewKeypad


void setup(){
  
  pinMode(8, INPUT); //PIR
  pinMode(10, INPUT); //ForceSensor
  pinMode(13, OUTPUT); //Buzzer
  pinMode(A5, INPUT); //Gas
  pinMode(12, INPUT); //Push
  
  
  Serial.begin(9600);

  myservo.attach(9);
  ServoClose();
  lcd.begin(16, 2);
  lcd.print(" Door Lock Sys");
  lcd.setCursor(0, 1);
  lcd.print("--Mini Project--");
  delay(3000);
  lcd.clear();
  
}

void loop(){
  //PIR
  int sensorState = digitalRead(8);
  Serial.println(sensorState);
  if (sensorState == HIGH) {
    digitalWrite(13, HIGH);
  } else {
    digitalWrite(13, LOW);
  }
  
  //Force
  int force = digitalRead(10);
  digitalWrite(13, force);

  //Gas
  int gasSensor = analogRead(A5);
  Serial.println(gasSensor);

  if (gasSensor > 500) {
  	digitalWrite(13, HIGH);
  }else {
    digitalWrite(13, LOW);
  }
  
  
  int var = digitalRead(12);
  if (var==HIGH){
  	Open();
  }
  
  
}

void clearData(){
  while (data_count != 0){ // This can be used for any array size,
    Data[data_count--] = 0; //clear array for new data
  }
  return;
}

void ServoOpen(){
  for (pos = 180; pos >= 0; pos -= 5) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}

void ServoClose(){
  for (pos = 0; pos <= 180; pos += 5) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}

void Open(){
  
  
  lcd.setCursor(0, 0);
  lcd.print(" Enter Password ");
  customKey = customKeypad.getKey();
  if (customKey) 
  {
    Data[data_count] = customKey; 
    lcd.setCursor(data_count, 1); 
    lcd.print(Data[data_count]); 
    data_count++; 
  }
  if (data_count == Password_Lenght - 1) 
  {
    if (!strcmp(Data, Master)) 
    {
      
      lcd.clear();
      ServoOpen();
      lcd.print(" Door is Open");
      
      delay(20000); 
      lcd.clear();
      lcd.print(" Door is Closed");
      ServoClose();
      delay(10000); 
      clearData();
      
    }else{
      lcd.clear();
      lcd.print("  Wrong Password");
      delay(1000);
      clearData();
    }
    }
}