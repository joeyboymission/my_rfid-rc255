#include <Servo.h>

Servo myServo;
const int servoPin = 3;  // D3 pin for Arduino Nano servo control

void setup() {
  Serial.begin(9600);
  myServo.attach(servoPin);
  myServo.write(0);  // Set initial position to 0 degrees
  Serial.println("Servo Motor Tester");
  Serial.println("Initialized at 0 degrees");
  Serial.println("Please Enter angle (0-180 degrees):");
}

void loop() {
  if (Serial.available() > 0) {
    int inputAngle = Serial.parseInt();
    
    // Clear any remaining characters in buffer
    while(Serial.available() > 0) {
      Serial.read();
    }
    
    // Validate input
    if (inputAngle >= 0 && inputAngle <= 180) {
      myServo.write(inputAngle);
      Serial.print("Moving servo to: ");
      Serial.println(inputAngle);
    } else {
      Serial.println("Error: Please enter a value between 0 and 180 degrees");
    }
    
    Serial.println("Enter new angle (0-180 degrees):");
    delay(15);
  }
}
