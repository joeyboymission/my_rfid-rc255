# RFID Entry Controller

A simple access control system that integrates an Arduino development board, RFID scanner, and servo motor to authorize entry based on valid RFID card verification.

## Required Materials
1. Arduino Board (Compatible with Uno, Mega, Nano, Leonardo/Pro Micro)
2. RFID-RC522 Scanner
3. Micro Servo Motor SG90
4. I2C LCD Display
5. Jumper Wires (Male-to-Male & Female-to-Male)
6. Breadboard
7. Buzzer

## Required Libraries
- [MFRC522](https://github.com/miguelbalboa/rfid.git) - For RFID scanner
- [Servo](https://www.arduino.cc/en/reference/servo) - For servo motor control
- [Wire](https://www.arduino.cc/en/reference/wire) - For I2C communication
- [LiquidCrystal_I2C](https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library.git) - For LCD display

## Pin Configuration

### RFID-RC522
| Signal      | RFID Pin  | Arduino Uno/101 | Arduino Mega | Arduino Nano v3 | Arduino Leonardo/Micro | Arduino Pro Micro |
|-------------|-----------|----------------|--------------|-----------------|----------------------|------------------|
| RST/Reset   | RST       | 9              | 5            | D9             | RESET/ICSP-5         | RST              |
| SPI SS      | SDA(SS)   | 10             | 53           | D10            | 10                   | 10               |
| SPI MOSI    | MOSI      | 11/ICSP-4      | 51           | D11            | ICSP-4               | 16               |
| SPI MISO    | MISO      | 12/ICSP-1      | 50           | D12            | ICSP-1               | 14               |
| SPI SCK     | SCK       | 13/ICSP-3      | 52           | D13            | ICSP-3               | 15               |

### Other Components
| Component    | Signal | Arduino Uno | Arduino Mega | Arduino Nano | Arduino Leonardo | Arduino Pro Micro |
|-------------|---------|-------------|------------|--------------|------------------|------------------|
| Buzzer      | Signal  | 7           | 7          | D7           | 7                | 7                |
| Servo Motor | Signal  | 3           | 3          | D3           | 3                | 3                |
| I2C LCD     | SDA     | A4          | 20         | A4           | 2                | 2                |
|             | SCL     | A5          | 21         | A5           | 3                | 3                |

## Working Principle
When an RFID card or tag is presented to the scanner, the system verifies if the card's information matches the registered data (either hardcoded or stored in a database). Upon successful authorization, the servo motor activates to open the gate. If unauthorized, the gate remains closed and access is denied.