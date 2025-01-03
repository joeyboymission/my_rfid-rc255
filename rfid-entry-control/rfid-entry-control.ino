/*
* Project Name: RFID Entry Control
* Author: JB Mission
* Date: January 2025
* Status: On-progress
* Description: An RFID-based entry control system using Arduino that authenticates users 
*             and controls access through a servo-operated mechanism.
* Reference(s):
* [Security Access using MFRC522 RFID Reader with Arduino](https://randomnerdtutorials.com/security-access-using-mfrc522-rfid-reader-with-arduino/)
* [Arduino Servo Motor](https://github.com/Circuit-Digest/servo-tutorial.git)
* 
* Required Materials:
* 1. Arduino (Uno, Mega, Nano, Leonardo/Micro Pro Mirco)
* 2. RFID-RC2522
* 3. Micro Servo 9G NG90
* 4. I2C LCD Crystal Display
* 5. Jumper Wires (Male to Male & Female to Male any length)
* 6. Breadboard
* 7. Buzzer
*
* Libraries:
* [MFRC522](https://github.com/miguelbalboa/rfid.git)
* [Servo](https://www.arduino.cc/en/reference/servo)
* [Wire](https://www.arduino.cc/en/reference/wire)
* [LiquidCrystal_I2C](https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library.git)
* 
* Pin Configuration:
* -----------------------------------------------------------------------------------------
*             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
*             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
* Signal      Pin          Pin           Pin       Pin        Pin              Pin
* -----------------------------------------------------------------------------------------
* RFID-RC522:
* RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
* SPI SS      SDA(SS)      10            53        D10        10               10
* SPI MOSI    MOSI         11/ICSP-4     51        D11        ICSP-4          16
* SPI MISO    MISO         12/ICSP-1     50        D12        ICSP-1          14
* SPI SCK     SCK          13/ICSP-3     52        D13        ICSP-3          15
*
* BUZZER:
* Signal      Pin          7             7         D7         7                7
*
* SERVO MOTOR:
* Signal      Pin          3             3         D3         3                3
*
* I2C LCD:
* SDA         A4           A4            20        A4         2                2
* SCL         A5           A5            21        A5         3                3
* -----------------------------------------------------------------------------------------
*/

// Include Libraries
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Set the LCD address to 0x27 for a 16 chars and 2 line display

const int servoPin = 3; // Assign the pin for servo motor
Servo myServo;
 
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

const char* userName = "Jiboy";  // Assign custom userName
const int buzzer = 7; // Assign the pin for buzzer
const char* cardID = "93 66 2B DA"; // Assign the authorized card UID

void setup() {
  // RFID
  Serial.begin(9600); // Initiate a serial communication
  SPI.begin();        // Initiate  SPI bus
  mfrc522.PCD_Init(); // Initiate MFRC522
  Wire.begin();       // Initiate I2C bus
  lcd.init();         // Initiate the LCD
  lcd.backlight();
  
  // Servo motor
  myServo.attach(servoPin);
  myServo.write(0); // Set the initial position to 0 degree

  // Buzzer Alarm
  pinMode(buzzer, OUTPUT);

  // Welcome message
  Serial.println("RFID Entry Control");
  Serial.println("Author: JB Mission");
  Serial.println("Please scan a card");
  Serial.println();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("RFID Entry");
  lcd.setCursor(0, 1);
  lcd.print("By:JB Mission");
  delay(2000);
  lcd.clear(); // Clear the startup message
  
  lcd.setCursor(0, 0);
  lcd.print("Scan a card...");
}

// Morse Beep
void MorseBeep(bool result) {
  if (result) {
    // Single beep of agree
    digitalWrite(buzzer, HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);
  } else {
    // Double beep of disagree (warning!)
    digitalWrite(buzzer, HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);
    delay(100);  // Increased delay between beeps

    digitalWrite(buzzer, HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);
  }
}

// LCD Message
void lcdMssg (bool lcdOut) {
  lcd.clear();
  if (lcdOut) {
    lcd.setCursor(0, 0);
    lcd.print("Authorized!");
    lcd.setCursor(0, 1);
    lcd.print("Welcome " + String(userName));
  } else {
    lcd.setCursor (0, 0);
    lcd.print("Unauthorized!");
    lcd.setCursor(0, 1);
    lcd.print("Access Denied!");
  }
}

void loop() {
  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Read RFID card content
  String content = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  content = content.substring(1);

  if (content == cardID) {
    Serial.println("Authorized access");
    Serial.print("Welcome back ");
    Serial.println(userName);
    Serial.print("Card ID: ");
    Serial.println(content);
    Serial.println();   // Skip one line
    MorseBeep(true);
    lcdMssg(true);
    myServo.write(90);  // Open gate
    delay(3000);        // Keep open for 3 seconds
    lcd.clear();        // Clear the current message
    myServo.write(0);   // Close gate
    
    Serial.println("Please scan a card");
    Serial.println();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Scan a card...");
    myServo.write(0); // Default closed position
  }
  else {
    Serial.println("Access denied");
    Serial.println("Unauthorized ID detected!");
    Serial.print("Card ID: ");
    Serial.println(content);
    Serial.println();   // Skip one line
    MorseBeep(false);
    lcdMssg(false);
    delay(2000);
    lcd.clear();        // Clear the current message

    Serial.println("Please scan a card");
    Serial.println();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Scan a card...");
    myServo.write(0); // Default closed position
  }
}
