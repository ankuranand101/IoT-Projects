#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

Servo entryGateServo;
Servo exitGateServo;
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Adjust I2C address if needed

const int entryIRPin = 2;       // IR sensor at entry gate
const int entryServoPin = 3;    // Servo for entry gate
const int exitIRPin = 4;        // IR sensor at exit gate
const int exitServoPin = 5;     // Servo for exit gate

// Ultrasonic sensors for vacant slots
const int triggerPin1 = 11;     // Trigger pin for slot 1 ultrasonic sensor
const int echoPin1 = 12;        // Echo pin for slot 1 ultrasonic sensor
const int triggerPin2 = 10;     // Trigger pin for slot 2 ultrasonic sensor
const int echoPin2 = 13;        // Echo pin for slot 2 ultrasonic sensor

// Threshold distance to determine if a slot is vacant (in centimeters)
const int distanceThreshold = 10;

void setup() {
  entryGateServo.attach(entryServoPin);
  exitGateServo.attach(exitServoPin);

  pinMode(entryIRPin, INPUT);
  pinMode(exitIRPin, INPUT);
  pinMode(triggerPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(triggerPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  lcd.begin(16, 2);     // Set up LCD with 16 columns and 2 rows
  lcd.backlight();      // Turn on the LCD backlight

  // Initial gate positions (closed)
  entryGateServo.write(90);
  exitGateServo.write(90);

  // Display initial message
  lcd.setCursor(0, 0);
  lcd.print("Parking System");
  delay(1000);
  lcd.clear();
}

void loop() {
  // Display welcome message with Smart Parking title
  lcd.setCursor(0, 0);
  lcd.print("Welcome To");
  lcd.setCursor(0, 1);
  lcd.print("Smart Parking");
  delay(2000);
  lcd.clear();

  // Check parking availability
  bool slotAvailable = isSlotAvailable();

  if (slotAvailable) {
    // Check if a car is detected at the entry
    if (digitalRead(entryIRPin) == HIGH) {
      displayVacantSlots();
      displayEntryMessage();
      openGate(entryGateServo);
      
    }
  } else {
    displayParkingFull();
  }

  // Check if a car is detected at the exit
  if (digitalRead(exitIRPin) == HIGH) {
    openGate1(exitGateServo);
  }

  delay(500);  // Small delay to prevent rapid re-triggering
}

// Function to open the gate and automatically close it after a delay
void openGate(Servo &gateServo) {
  gateServo.write(180);  // Open gate position
  delay(3000);           // Keep gate open for 3 seconds
  gateServo.write(90);   // Close gate position
}

void openGate1(Servo &gateServo) {
  gateServo.write(0);  // Open gate position
  delay(3000);           // Keep gate open for 3 seconds
  gateServo.write(90);   // Close gate position
}

// Display a welcome message and show vacant slots
void displayEntryMessage() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Car detected!");
  lcd.setCursor(0, 1);
  lcd.print("Entering parking");
  delay(500);
}

// Display vacant slot numbers if available
void displayVacantSlots() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Vacant Slots:");

  if (getDistance(triggerPin1, echoPin1) > distanceThreshold) {
    lcd.setCursor(0, 1);
    lcd.print("1 ");
  }
  if (getDistance(triggerPin2, echoPin2) > distanceThreshold) {
    lcd.setCursor(3, 1);
    lcd.print("2 ");
  }
  delay(1000);
}

// Display message if parking is full
void displayParkingFull() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Parking is full");
  lcd.setCursor(0, 1);
  lcd.print("Sorry!");
  delay(2000);
}

// Check if any parking slot is available
bool isSlotAvailable() {
  return (getDistance(triggerPin1, echoPin1) > distanceThreshold || getDistance(triggerPin2, echoPin2) > distanceThreshold);
}

// Function to get distance from ultrasonic sensor
long getDistance(int triggerPin, int echoPin) {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  long distance = duration * 0.034 / 2;  // Convert duration to distance in cm
  return distance;
}
