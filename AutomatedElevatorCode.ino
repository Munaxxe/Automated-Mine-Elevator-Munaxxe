// Automated Hoist Machine - BEIE 2024
// Calling Libraries
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "HX711.h"
#include <Adafruit_Fingerprint.h>
#include <Servo.h>
#include <SoftwareSerial.h>

// 1 - 10(L) - 15(A) - 17() - 23() - 45(F)
// Variable Declarations
// Buzzer
const int buzzer = A0;         // buzzer to Analog Pin - A0

// Elevator manual Control
const int elevatorUp = A1;     // Elevator Up to Analog Pin - A1

// LEDs
const int redLED = A2;         // green-LED to Analog Pin - A2
const int greenLED = A3;          // red-LED to Analog Pin - A3

int readElevatorUpButton = 0;

// Level sensors 
const int levelOne = 4;        // IR Sensor connected to digital pin 4 - level G
const int levelTwo = 5;        // IR Sensor connected to digital pin 5 - level 1
const int levelThree = 6;      // IR Sensor connected to digital pin 6 - level 2
const int levelFour = 7;       // IR Sensor connected to digital pin 7 - level 3

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 8;
const int LOADCELL_SCK_PIN = 9;
float weight = 0;

// Main Hoist Door Motor
const int doorMotor = 10;

// Elevator Motor connections - Up/Down Movement
int enA = 11;
int in1 = 12;
int in2 = 13;

// Floor level sensors
int levelOneIR, levelTwoIR, levelThreeIR, levelFourIR; 

// Door Motor
int angle = 0;

// Weight Calibration
float weightLCD = 0;

HX711 scale; // HX711 Load Cell Module
LiquidCrystal_I2C lcd(0x27,20,4);  // LCD address 
SoftwareSerial mySerial(2, 3); // Fingerprint Module
Servo servo;

#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
#else
#define mySerial Serial1
#endif

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup(){
 
scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
Serial.begin(9600);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  // Start of Fingerprint Setup
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

  finger.getTemplateCount();

  if (finger.templateCount == 0) {
    Serial.print("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example.");
  }
  else {
    Serial.println("Waiting for valid finger...");
      Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  } 
// End Of Fingerprint Setup

// Setting up Scale
  scale.set_scale(2280.f);                      // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();               // reset the scale to 0
// End of Setting up Scale

    // Buzzer ON at Boot
    tone(buzzer, 1000); // Send 1KHz sound signal...
    delay(500);        // ...for 1 sec
    noTone(buzzer);     // Stop sound...
    delay(500);        // ...for 1sec

    lcd.init();  // Initialising the lcd     
    lcd.backlight();

    lcd.setCursor(0,0);
    delay(2000);  
    lcd.print("Calibrating.");
    delay(1000);
    lcd.setCursor(0,0);
    lcd.print("Calibrating..");
    delay(1000);
    lcd.setCursor(0,0);
    lcd.print("Calibrating...");
    delay(1000);
    lcd.clear();

    // Door Control
    servo.attach(doorMotor);
    servo.write(angle);

    pinMode(levelOne, INPUT);
    pinMode(levelTwo, INPUT);
    pinMode(levelThree, INPUT);
    pinMode(levelFour, INPUT);
    pinMode(doorMotor, OUTPUT); 
    pinMode(buzzer, OUTPUT);

    // Setting all the Motor Control pins to Outputs
    pinMode(enA, OUTPUT);
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);

    // Setting all the Elevator Manual Control Buttons
    pinMode(elevatorUp, INPUT);

    // Setting all the Warning LEDs
    pinMode(redLED, OUTPUT);
    pinMode(greenLED, OUTPUT);
    
    // Turning off motors - Initial state
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    
    lcd.clear(); 

    // Default LED power control
    analogWrite(greenLED, 255);
    analogWrite(redLED, 255);
    delay(500);
    analogWrite(greenLED, 0);
    analogWrite(redLED, 0);
}

void hoistLevel(){
     levelOneIR = digitalRead(levelOne);      // Read the state from the IR sensor - Level 1
     levelTwoIR = digitalRead(levelTwo);      // Read the state from the IR sensor - Level 2
     levelThreeIR = digitalRead(levelThree);  // Read the state from the IR sensor - Level 3
     levelFourIR = digitalRead(levelFour);    // Read the state from the IR sensor - Level 4
  
  if(levelOneIR == 0){
    lcd.setCursor(14,1);
    lcd.print("G");
    }
  else if(levelTwoIR == 0){
     lcd.setCursor(14,1);
     lcd.print("1");
    }
  else if(levelThreeIR == 0){
    lcd.setCursor(14,1);
    lcd.print("2");
    }
  else if(levelFourIR == 0){
    lcd.setCursor(14,1);
    lcd.print("3");

    } else
    lcd.setCursor(14,1);
    lcd.print("");
  }

void loop(){ 
    lcd.setCursor(0,0);
    lcd.print("User ID: ");
    lcd.setCursor(0,1);
    lcd.print("Hoist Level: "); 
    lcd.setCursor(0,2);
    lcd.print("Level Assigned: ");
//    lcd.print("Weight: ");
    lcd.setCursor(0,3);
    lcd.print("Weight Status:");

    lcd.setCursor(9,0);
    lcd.print("          ");
    lcd.setCursor(16,2);
    lcd.print(" ");
    scaleReadings();
    
    getFingerprintID();
    
    
    delay(50);  

    
    hoistLevel();
//    triggerElevatorButton();

//    levelONE();
//    delay(1000);
//    levelTWO();
//    delay(1000);
//    levelTHREE();
//    delay(1000);

//    triggerCycle();
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
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
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    lcd.setCursor(9,0);
    lcd.print("Not Found");    
    triggerUserNotFound();
    lcd.setCursor(16,2);
    lcd.print("N");  
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  // Found a match
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);

    lcd.setCursor(9,0);
    lcd.print("");    


String fpID = String(finger.fingerID);

// Normal Weight Cycle - 1
      if(fpID == "1" && weightLCD < 3){
    lcd.setCursor(9,0);
    lcd.print("1         ");
    lcd.setCursor(16,2);
    lcd.print("1");

    delay(1000);
    doorOpenClose();
    levelONE();
    levelTWO();
    levelTHREE(); 
    }  
// Over Weight Cycle - 1
      if(fpID == "1"){
    lcd.setCursor(9,0);
    lcd.print("1         ");
    lcd.setCursor(16,2);
    lcd.print("1");
    delay(1000);
    }  

// Over Weight Cycle - 2
      if(fpID == "10"){
    lcd.setCursor(9,0);
    lcd.print("10       ");
    lcd.setCursor(16,2);
    lcd.print("2");
    delay(1000);
    }
// Normal Weight Cycle - 2    
      if(fpID == "10" && weightLCD < 3){
    lcd.setCursor(9,0);
    lcd.print("10       ");
    lcd.setCursor(16,2);
    lcd.print("2");
    delay(1000);
    doorOpenClose();
    triggerCycle();
    }
// Over Weight Cycle - 3
      if(fpID == "15"){
    lcd.setCursor(9,0);
    lcd.print("15       ");
    lcd.setCursor(16,2);
    lcd.print("2");
    delay(1000);
    }
// Normal Weight Cycle - 3   
      if(fpID == "15" && weightLCD < 3){
    lcd.setCursor(9,0);
    lcd.print("15       ");
    lcd.setCursor(16,2);
    lcd.print("2");
    delay(1000);
    doorOpenClose();
    triggerCycle();
    }

// Normal Weight Cycle - 4
      if((fpID == "67" || fpID == "88" || fpID == "57") && weightLCD < 3){
    lcd.setCursor(9,0);
    lcd.print("67       ");
    delay(1000);
    lcd.setCursor(16,2);
    lcd.print("3"); 
    levelTHREE(); 
    doorOpenClose();
    }
// Over Weight Cycle - 4
      if(fpID == "67" || fpID == "88" || fpID == "57") {
    lcd.setCursor(9,0);
    lcd.print("67       ");
    delay(1000);
    lcd.setCursor(16,2);
    lcd.print("3"); 
    }

// Normal Weight Cycle - 5
      if(fpID == "45" && weightLCD < 3){
    lcd.setCursor(9,0);
    lcd.print("45       ");
    lcd.setCursor(16,2);
    lcd.print("3");
    delay(1000);
    doorOpenClose();
    }
// Over Weight Cycle - 5
      if(fpID == "45"){
    lcd.setCursor(9,0);
    lcd.print("45       ");
    lcd.setCursor(16,2);
    lcd.print("3");
    delay(1000);
    }
// Default
      if(weightLCD >= 3){
    lcd.setCursor(15,3);
    lcd.print("O-W");
    triggerBuzzerOverWeight();
    }
  
     else{
      // Do-nothing
    }

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

// Controlling the spinning direction of the motor
void directionControl() {
  // Motor set to maximum speed
  analogWrite(enA, 128);

  // Turn on motor
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  delay(2000);
  
  // Now change motor directions
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  delay(2000);
  
  // Turn off motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
}

// Controlling the speed of the motor
void levelONE() {
  // Turn on motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  
  // Accelerate from zero to maximum speed
  for (int i = 20; i < 80; i++) {
    analogWrite(enA, i);
    delay(5);
  }
  
  // Decelerate from maximum speed to zero
  for (int i = 79; i >= 20; --i) {
    analogWrite(enA, i);
    delay(5);
  }
  
  hoistLevel();
  
  // Now turn off motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  delay(2000);
  // 2

  // Turn on motors
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  
  // Accelerate from zero to maximum speed
  for (int i = 20; i < 80; i++) {
    analogWrite(enA, i);
    delay(60);
  }
  
  // Decelerate from maximum speed to zero
  for (int i = 79; i >= 20; --i) {
    analogWrite(enA, i);
    delay(60);
  }
  
  hoistLevel();
  // Now turn off motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
}

void levelTWO() {
  // Turn on motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  
  // Accelerate from zero to maximum speed
  for (int i = 20; i < 80; i++) {
    analogWrite(enA, i);
    delay(8);
  }
  
  // Decelerate from maximum speed to zero
  for (int i = 79; i >= 20; --i) {
    analogWrite(enA, i);
    delay(8);
  }
  hoistLevel();
  // Now turn off motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  delay(2000);
  // 2

  // Turn on motors
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  
  // Accelerate from zero to maximum speed
  for (int i = 20; i < 80; i++) {
    analogWrite(enA, i);
    delay(60);
  }
  
  // Decelerate from maximum speed to zero
  for (int i = 79; i >= 20; --i) {
    analogWrite(enA, i);
    delay(60);
  }

  hoistLevel();
  // Now turn off motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
}

void levelTHREE() {
  // Turn on motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  
  // Accelerate from zero to maximum speed
  for (int i = 20; i < 80; i++) {
    analogWrite(enA, i);
    delay(13);
  }
  
  // Decelerate from maximum speed to zero
  for (int i = 79; i >= 20; --i) {
    analogWrite(enA, i);
    delay(13);
  }

  hoistLevel();
  // Now turn off motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  delay(2000);
  // 2

  // Turn on motors
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  
  // Accelerate from zero to maximum speed
  for (int i = 20; i < 80; i++) {
    analogWrite(enA, i);
    delay(60);
  }
  
  // Decelerate from maximum speed to zero
  for (int i = 79; i >= 20; --i) {
    analogWrite(enA, i);
    delay(60);
  }
  
  hoistLevel();
  // Now turn off motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
}

void triggerBuzzerOverWeight(){
    // Beep Cycle
    tone(buzzer, 1000); // Send 1KHz sound signal...
    delay(500);        // ...for 1 sec
    noTone(buzzer);     // Stop sound...
    delay(500);        // ...for 1sec
  }

void triggerUserNotFound(){
    // First Beep Cycle
    tone(buzzer, 1000); // Send 1KHz sound signal...
    delay(1000);        // ...for 1 sec
    noTone(buzzer);     // Stop sound...
    delay(1000);        // ...for 1sec
    // Second Beep Cycle
    tone(buzzer, 1000); // Send 1KHz sound signal...
    delay(1000);        // ...for 1 sec
    noTone(buzzer);     // Stop sound...
    delay(1000);        // ...for 1sec
  }

void scaleReadings() {
    weight = (scale.get_units(10),2);
    weightLCD = (String(scale.get_units(10),2).toInt());   
    float scaledValue = map(weightLCD, 0.3, 1023, 0, 10);
    
    Serial.println(weightLCD);
    Serial.println(scaledValue);  

  if(weightLCD >= 3 ){
    lcd.setCursor(15,3);
    lcd.print("O-W");
    
    triggerBuzzerOverWeight();
    analogWrite(greenLED, 0);
    analogWrite(redLED, 255);
    delay(500);
    }
  if(weightLCD < 3){
    lcd.setCursor(15,3);
    lcd.print("N  ");
    
    analogWrite(greenLED, 255);
    analogWrite(redLED, 0);
    delay(500);
    }
  delay(50);
  }


void triggerElevatorButton(){
  readElevatorUpButton = analogRead(elevatorUp);
  
  if(readElevatorUpButton == 255){
  triggerBuzzerOverWeight();
    }
  if(readElevatorUpButton != 255){
  
    }
    delay(50);
  }

void doorOpenClose(){
  // Open
    for(angle = 0; angle < 110; angle++)  
  {                                  
    servo.write(angle);               
    delay(15);                   
  } 

  delay(2000);
   // Close
  for(angle = 110; angle > 0; angle--)    
  {                                
    servo.write(angle);           
    delay(15);       
  }  
 }

void triggerCycle(){
  // Turn on motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  
  // Accelerate from zero to maximum speed
  for (int i = 20; i < 80; i++) {
    analogWrite(enA, i);
    delay(8);
  }
  
  // Decelerate from maximum speed to zero
  for (int i = 79; i >= 20; --i) {
    analogWrite(enA, i);
    delay(8);
  }
  hoistLevel();
  // Now turn off motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  delay(4000);
  // Stop at level 2 
  // Move from level 2 and go to level three
  // Turn on motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  
  // Accelerate from zero to maximum speed
  for (int i = 20; i < 80; i++) {
    analogWrite(enA, i);
    delay(8);
  }
  
  // Decelerate from maximum speed to zero
  for (int i = 79; i >= 20; --i) {
    analogWrite(enA, i);
    delay(8);
  }
  
  hoistLevel();
  
  // Now turn off motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  delay(2000);
// Move from level 3 up to Ground Level
  // Turn on motors
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  
  // Accelerate from zero to maximum speed
  for (int i = 20; i < 80; i++) {
    analogWrite(enA, i);
    delay(60);
  }
  
  // Decelerate from maximum speed to zero
  for (int i = 79; i >= 20; --i) {
    analogWrite(enA, i);
    delay(60);
  }
  
  hoistLevel();
  // Now turn off motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  } 
