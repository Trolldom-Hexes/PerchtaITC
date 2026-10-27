#include <Wire.h>
#include <QMC5883LCompass.h>

// Create compass object
QMC5883LCompass compass;

void setup() {
  // Start serial communication at 115200 baud
  Serial.begin(115200);
  while (!Serial);

  Serial.println("\n--- GY-273 QMC5883L Magnetometer Test ---");

  // Initialize the compass sensor
  compass.init();
  
  // Optional: Set scaling range (e.g., 2, 8 Gauss) or measuring mode if needed
  // compass.setRange(2);

  Serial.println("QMC5883L successfully initialized!");
  Serial.println("----------------------------------------\n");
}

void loop() {
  // Read raw magnetic values and compass heading
  compass.read();

  int x = compass.getX();
  int y = compass.getY();
  int z = compass.getZ();
  int azimuth = compass.getAzimuth(); // Heading in degrees (0 - 360)

  // Print formatted magnetic field values
  Serial.print("Mag X: "); Serial.print(x);
  Serial.print("\tY: "); Serial.print(y);
  Serial.print("\tZ: "); Serial.print(z);
  Serial.print("\tHeading: "); Serial.print(azimuth);
  Serial.println(" °");

  Serial.println("----------------------------------------");

  // Wait 250 ms before the next reading
  delay(250);
}