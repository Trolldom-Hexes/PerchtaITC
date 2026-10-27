#include <driver/i2s.h>

// I2S Pin-Definitionen für den MAX98357A
const int pinBCLK = 27; // Bit Clock
const int pinLRC  = 26; // Word Select / Left-Right Clock
const int pinDOUT = 25; // Data Out

// Audio-Parameter
const int sampleRate = 16000; // 16 kHz Abtastrate
const int frequency = 440;    // 440 Hz Ton (Kammerton A)
const int amplitude = 10000;  // Lautstärke

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("\n--- MAX98357A Audio Test (ESP32 Native) ---");

  // I2S Konfiguration
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = sampleRate,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = 0,
    .dma_buf_count = 8,
    .dma_buf_len = 64,
    .use_apll = false
  };

  // I2S Pin-Konfiguration
  i2s_pin_config_t pin_config = {
    .bck_io_num = pinBCLK,
    .ws_io_num = pinLRC,
    .data_out_num = pinDOUT,
    .data_in_num = I2S_PIN_NO_CHANGE
  };

  // I2S installieren und starten
  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pin_config);
  i2s_zero_dma_buffer(I2S_NUM_0);

  Serial.println("MAX98357A initialisiert. Spiele Testton ab...");
  Serial.println("----------------------------------------\n");
}

void loop() {
  // Sinus-Ton generieren und über I2S an den Verstärker senden
  int bufferSize = 512;
  int16_t samples[bufferSize];
  
  for (int i = 0; i < sampleRate; i += bufferSize / 2) {
    for (int j = 0; j < bufferSize; j += 2) {
      float t = (float)(i + j / 2) / sampleRate;
      int16_t sample = amplitude * sin(2.0 * PI * frequency * t);
      
      samples[j] = sample;     //werter Kanal
      samples[j + 1] = sample; // Rechter Kanal
    }
    
    size_t bytesWritten;
    i2s_write(I2S_NUM_0, samples, sizeof(samples), &bytesWritten, portMAX_DELAY);
  }

  delay(500);
}