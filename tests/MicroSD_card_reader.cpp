#include <SPI.h>
#include <SD.h>

// Define SPI pins for the MicroSD card module (VSPI default on ESP32)
const int pinCS   = 5;   // Chip Select
const int pinMOSI = 23;  // Master Out Slave In
const int pinMISO = 19;  // Master In Slave Out
const int pinSCK  = 18;  // Serial Clock

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("\n--- MicroSD Card Initialization Test ---");

  // Initialize the SPI bus with the defined pins
  SPI.begin(pinSCK, pinMISO, pinMOSI, pinCS);

  // Initialize the SD card
  if (!SD.begin(pinCS)) {
    Serial.println("Initialization failed! Please check your wiring or format the SD card as FAT32.");
    return;
  }
  
  Serial.println("MicroSD card initialized successfully.");

  // Check card type
  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("No SD card attached.");
    return;
  }

  Serial.print("SD Card Type: ");
  if (cardType == CARD_MMC) {
    Serial.println("MMC");
  } else if (cardType == CARD_SD) {
    Serial.println("SDSC");
  } else if (cardType == CARD_SDHC) {
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }

  // Check card size
  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %llu MB\n", cardSize);

  // Run a quick write and read test
  writeFile("/itc_test.txt", "Perchta ITC Box - SD Card operational.\n");
  readFile("/itc_test.txt");
  
  Serial.println("----------------------------------------\n");
}

void loop() {
  // Nothing to do in loop for this test
}

void writeFile(const char *path, const char *message) {
  Serial.printf("Writing to file: %s\n", path);

  File file = SD.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing.");
    return;
  }
  
  if (file.print(message)) {
    Serial.println("File written successfully.");
  } else {
    Serial.println("Write failed.");
  }
  file.close();
}

void readFile(const char *path) {
  Serial.printf("Reading from file: %s\n", path);

  File file = SD.open(path);
  if (!file) {
    Serial.println("Failed to open file for reading.");
    return;
  }

  Serial.print("File content: ");
  while (file.available()) {
    Serial.write(file.read());
  }
  file.close();
}