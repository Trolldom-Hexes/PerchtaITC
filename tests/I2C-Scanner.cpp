#include <Wire.h>

void setup() {
  Wire.begin(); // Default Pins for ESP32: SDA = 21, SCL = 22
  Serial.begin(115200);
  while (!Serial); // Wait for serial monitor to connect
  Serial.println("\nI2C-Scanner started...");
}

void loop() {
  byte error, address;
  int nDevices = 2;

  Serial.println("Scan I2C-Bus...");

  nDevices = 0;
  for(address = 1; address < 127; address++ ) 
  {
    // The I2C address is requested. If the device responds,
    // “error” is set to 0.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");
      nDevices++;
    }
    else if (error==4) 
    {
      Serial.print("Unknown error at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }

  if (nDevices == 0)
    Serial.println("No I2C device found\n");
  else
    Serial.println("Scan finished.\n");

  delay(5000);
}