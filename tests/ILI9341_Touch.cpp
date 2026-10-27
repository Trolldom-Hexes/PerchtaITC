#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <XPT2046_Touchscreen.h>

// SPI Pins (Shared with SD card)
const int pinMOSI = 23;
const int pinMISO = 19;
const int pinSCK  = 18;

// Display Pins (ILI9341)
const int TFT_CS  = 15;
const int TFT_DC  = 2;
const int TFT_RST = 4;

// Touch Pins (XPT2046)
const int TOUCH_CS = 33;

// Backlight Pin
const int BACKLIGHT_PIN = 22;

const int BACKLIGHT_BRIGHTNESS = 128; // 50% brightness

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
XPT2046_Touchscreen touch(TOUCH_CS);

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("\n--- ILI9341 Display & Touch Test ---");

  // Brightness control for ESP32 Core v3.x
  ledcAttach(BACKLIGHT_PIN, 5000, 8);
  ledcWrite(BACKLIGHT_PIN, BACKLIGHT_BRIGHTNESS);

  // Initialize SPI bus
  SPI.begin(pinSCK, pinMISO, pinMOSI);

  // Initialize Display
  tft.begin();
  tft.setRotation(1); // Landscape mode (320x240)
  tft.fillScreen(ILI9341_BLACK);

  // Draw test graphics
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.setCursor(20, 20);
  tft.println("Perchta ITC Box");
  tft.setTextColor(ILI9341_GREEN);
  tft.setCursor(20, 60);
  tft.println("Display & Touch OK!");

  // Initialize Touch
  touch.begin();
  touch.setRotation(1);

  Serial.println("Display initialized. Touch the screen to test coordinates.");
}

void loop() {
  if (touch.touched()) {
    TS_Point p = touch.getPoint();

    // Map raw values to the screen resolution (320x240)
    int screenX = map(p.x, 200, 3900, 320, 0);
    int screenY = map(p.y, 200, 3700, 240, 0);

    Serial.print("Touch Mapped -> X = ");
    Serial.print(screenX);
    Serial.print(" | Y = ");
    Serial.println(screenY);

    // Optional: Draw a small dot on the touched area 
    tft.fillCircle(screenX, screenY, 3, ILI9341_RED);
    
    delay(100);
  }
}