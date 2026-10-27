#include <Wire.h>
#include <MPU6050.h>

// Create sensor object
MPU6050 mpu;

void setup() {
  // Start serial communication at 115200 baud
  Serial.begin(115200);
  while (!Serial);

  Serial.println("\n--- MPU6050 / GY-521 Detailed Test ---");

  // Initialize I2C and sensor
  Wire.begin();
  mpu.initialize();
  mpu.setSleepEnabled(false);

  Serial.println("MPU6050 successfully initialized!");
  Serial.println("----------------------------------------\n");
}

void loop() {
  int16_t ax, ay, az;
  int16_t gx, gy, gz;
  int16_t rawTemp;

  // Read raw sensor data
  mpu.getAcceleration(&ax, &ay, &az);
  mpu.getRotation(&gx, &gy, &gz);
  rawTemp = mpu.getTemperature();

  // Convert raw values to physical units (default ranges: +/- 2g, +/- 250 °/s)
  float accelX = ax / 16384.0 * 9.81;
  float accelY = ay / 16384.0 * 9.81;
  float accelZ = az / 16384.0 * 9.81;

  float gyroX = gx / 131.0;
  float gyroY = gy / 131.0;
  float gyroZ = gz / 131.0;

  float temperature = (rawTemp / 340.0) + 36.53;

  // Print formatted acceleration values
  Serial.print("Accel X: "); Serial.print(accelX, 2);
  Serial.print(", Y: "); Serial.print(accelY, 2);
  Serial.print(", Z: "); Serial.print(accelZ, 2);
  Serial.println(" m/s^2");

  // Print formatted rotation values
  Serial.print("Gyro X:  "); Serial.print(gyroX, 2);
  Serial.print(", Y: "); Serial.print(gyroY, 2);
  Serial.print(", Z: "); Serial.print(gyroZ, 2);
  Serial.println(" °/s");

  // Print internal temperature
  Serial.print("Temp:    "); Serial.print(temperature, 2);
  Serial.println(" °C");

  Serial.println("----------------------------------------");
  
  // Wait 500 ms before the next reading
  delay(500);
}