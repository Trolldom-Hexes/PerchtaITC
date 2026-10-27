#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <XPT2046_Touchscreen.h>

// SPI Pins
const int pinMOSI = 23;
const int pinMISO = 19;
const int pinSCK  = 18;

// Display Pins
const int TFT_CS  = 15;
const int TFT_DC  = 2;
const int TFT_RST = 4;

// Touch Pins
const int TOUCH_CS = 33;

// Backlight Pin
const int BACKLIGHT_PIN = 27;
const int BACKLIGHT_BRIGHTNESS = 128;

// I2C Pins für Gyroskop
const int SDA_PIN = 21;
const int SCL_PIN = 22;
uint8_t MPU_ADDR = 0x68;

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
XPT2046_Touchscreen touch(TOUCH_CS);

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("\n--- ILI9341 Display & Touch & Gyro Test ---");

  // Brightness control
  ledcAttach(BACKLIGHT_PIN, 5000, 8);
  ledcWrite(BACKLIGHT_PIN, BACKLIGHT_BRIGHTNESS);

  // Initialize SPI bus
  SPI.begin(pinSCK, pinMISO, pinMOSI);

  // Initialize I2C bus for Gyro
  Wire.begin(SDA_PIN, SCL_PIN);
  
  // MPU6050 aufwecken
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0);    // set to zero (wakes up the MPU-6050)
  byte error = Wire.endTransmission(true);
  
  if (error == 0) {
    Serial.println("MPU6050 erfolgreich aufgeweckt!");
  } else {
    Serial.print("Fehler beim MPU6050 Init, Fehlercode: ");
    Serial.println(error);
  }

  // Initialize Display
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);

  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.setCursor(20, 20);
  tft.println("Perchta ITC Box");
  tft.setTextColor(ILI9341_GREEN);
  tft.setCursor(20, 60);
  tft.println("Gyro laeuft permanent...");

  // Initialize Touch
  touch.begin();
  touch.setRotation(1);

  Serial.println("System bereit.");
}

void loop() {
  float gyroX = 0, gyroY = 0, gyroZ = 0;

  // 1. Gyro abfragen mit Längenprüfung
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x43); // Startregister für Gyro-Daten
  byte transError = Wire.endTransmission(false);

  if (transError == 0) {
    byte bytesReceived = Wire.requestFrom(MPU_ADDR, (uint8_t)6, (uint8_t)true);
    if (bytesReceived == 6) {
      int16_t rawGx = Wire.read() << 8 | Wire.read();
      int16_t rawGy = Wire.read() << 8 | Wire.read();
      int16_t rawGz = Wire.read() << 8 | Wire.read();

      gyroX = rawGx / 131.0;
      gyroY = rawGy / 131.0;
      gyroZ = rawGz / 131.0;
    } else {
      Serial.println("Fehler: Konnte keine 6 Bytes vom Gyro lesen!");
    }
  } else {
    Serial.println("Fehler bei I2C-Transmission zum Gyro!");
  }

  Serial.print("Gyro -> X="); Serial.print(gyroX);
  Serial.print(" Y="); Serial.print(gyroY);
  Serial.print(" Z="); Serial.println(gyroZ);

  // 2. Touch abfragen (mit deinem exakten Mapping)
  if (touch.touched()) {
    TS_Point p = touch.getPoint();

    int screenX = map(p.x, 350, 3860, 320, 0);
    int screenY = map(p.y, 280, 3840, 240, 0);
    screenX = constrain(screenX, 0, 319);
    screenY = constrain(screenY, 0, 239);

    // Auf dem Display anzeigen
    tft.fillRect(0, 120, 320, 100, ILI9341_BLACK);
    
    tft.setTextColor(ILI9341_CYAN);
    tft.setTextSize(1);
    tft.setCursor(20, 130);
    tft.print("Touch: X="); tft.print(screenX); tft.print(" Y="); tft.print(screenY);

    tft.setTextColor(ILI9341_YELLOW);
    tft.setTextSize(2);
    tft.setCursor(20, 160);
    tft.print("GX:"); tft.print(gyroX, 1);
    tft.setCursor(120, 160);
    tft.print("GY:"); tft.print(gyroY, 1);
    tft.setCursor(220, 160);
    tft.print("GZ:"); tft.print(gyroZ, 1);

    tft.fillCircle(screenX, screenY, 3, ILI9341_RED);
  }
  
  delay(150);
}