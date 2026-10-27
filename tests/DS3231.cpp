#include <Wire.h>
#include <RTClib.h>

// Create RTC object
RTC_DS3231 rtc;

void setup() {
  // Start serial communication at 115200 baud
  Serial.begin(115200);
  while (!Serial);

  Serial.println("\n--- DS3231 RTC Test ---");

  // Initialize I2C and RTC
  Wire.begin();
  
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC! Check wiring.");
    while (1);
  }

  // Check if the RTC lost power and set the date/time to the compilation time
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting the time to compile time!");
    // This line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  Serial.println("DS3231 successfully initialized!");
  Serial.println("----------------------------------------\n");
}

void loop() {
  // Read current date and time
  DateTime now = rtc.now();
  float tempC = rtc.getTemperature();

  // Print formatted date and time
  Serial.print("Date: ");
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" | Time: ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  
  // Print temperature measured by the RTC chip
  Serial.print(" | Temp: ");
  Serial.print(tempC);
  Serial.println(" °C");

  Serial.println("----------------------------------------");

  // Wait 1 second before the next reading
  delay(1000);
}