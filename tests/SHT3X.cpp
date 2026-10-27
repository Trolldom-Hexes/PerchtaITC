#include <Wire.h>
#include "Adafruit_SHT31.h"

// Create sensor object
Adafruit_SHT31 sht31 = Adafruit_SHT31();

void setup() {
  // Start serial communication at 115200 baud
  Serial.begin(115200);
  while (!Serial) {
    delay(10); // Wait until the serial monitor is ready (important for ESP32)
  }

  Serial.println("\n--- SHT3X Sensor Detailed Test ---");

  // Initialize I2C bus (Default pins for ESP32: SDA = GPIO 21, SCL = GPIO 22)
  Wire.begin();

  // Start sensor and check address (Default is 0x44)
  if (!sht31.begin(0x44)) {
    Serial.println("ERROR: Could not find a valid SHT31 sensor! Please check wiring.");
    while (1) {
      delay(1000);
    }
  }

  Serial.println("SHT31 sensor successfully initialized!");
  
  // Test additional features (Sensor heater, disabled by default)
  Serial.print("Heater enabled: ");
  Serial.println(sht31.isHeaterEnabled() ? "YES" : "NO");

  // Activate Heater
  sht31.heater(true);
  Serial.print("Heater active: ");
  Serial.println(sht31.isHeaterEnabled() ? "YES" : "NO");

  delay(5000);

  // Deactivate Heater
  sht31.heater(false);
  Serial.print("Heater active after deactivation: ");
  Serial.println(sht31.isHeaterEnabled() ? "YES" : "NO");
  
  Serial.println("----------------------------------------\n");
}

void loop() {
  // Read temperature and humidity values
  float t = sht31.readTemperature();
  float h = sht31.readHumidity();

  // Check if temperature reading is valid
  if (!isnan(t)) { // Checks if 't' is not NaN (Not a Number)
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.println(" °C");
  } else {
    Serial.println("ERROR reading temperature!");
  }

  // Check if humidity reading is valid
  if (!isnan(h)) {
    Serial.print("Humidity:    ");
    Serial.print(h);
    Serial.println(" % rH");
    
    // Dew point calculation (Magnus approximation formula)
    double a = 17.27;
    double b = 237.7;
    double alpha = ((a * t) / (b + t)) + log(h / 100.0);
    double dewPoint = (b * alpha) / (a - alpha);
    
    Serial.print("Dew Point:   ");
    Serial.print(dewPoint);
    Serial.println(" °C");
  } else {
    Serial.println("ERROR reading humidity!");
  }

  Serial.println("----------------------------------------");
  
  // Wait 2 seconds before the next measurement
  delay(2000);
}